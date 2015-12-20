#include "FBXLoaderManager.h"
#include <fbxsdk.h>
#include <fstream>
#include <iostream>


static std::ofstream writeStream;

FBXLoaderManager::FBXLoaderManager()
{
	mp_FbxManager = nullptr;
	mp_FbxIOSettings = nullptr;
	mp_FbxImporter = nullptr;
	mp_FbxScene = nullptr;
	mp_FbxRootNode = nullptr;
	mp_FbxFileName = nullptr;
	mp_FileOutName = nullptr;
}

FBXLoaderManager::~FBXLoaderManager()
{
	if (mp_FileOutName)
		delete[] mp_FileOutName;
}

void FBXLoaderManager::Initilize(char* _FbxFileName)
{
	if (mp_FileOutName)
		delete[] mp_FileOutName;
	mp_FileOutName = nullptr;
	mp_FbxFileName = nullptr;
	m_NumTabs = 0;

#if _DEBUG
	_ASSERT_EXPR(_FbxFileName, L"FBXLoader - Passed In File Name Can NOT Be Null!");
#endif

	mp_FbxFileName = _FbxFileName;
	ParseOutFileName();

	mp_FbxManager = FbxManager::Create();

#if _DEBUG
	_ASSERT_EXPR(mp_FbxManager, L"FBXLoader - FbxManager failed to create!");
#endif

	mp_FbxIOSettings = FbxIOSettings::Create(mp_FbxManager, IOSROOT);

#if _DEBUG
	_ASSERT_EXPR(mp_FbxIOSettings, L"FBXLoader - FbxIOSettings failed to create!");
#endif

	mp_FbxManager->SetIOSettings(mp_FbxIOSettings);

	mp_FbxImporter = FbxImporter::Create(mp_FbxManager, "");

#if _DEBUG
	_ASSERT_EXPR(mp_FbxImporter, L"FBXLoader - FbxImporter failed to create!");
#endif


	if (!mp_FbxImporter->Initialize(mp_FbxFileName, -1, mp_FbxManager->GetIOSettings()))
	{
#if _DEBUG
		_ASSERT_EXPR(0, "Fbx Importer failed to initilize With the following error message: ");
		_ASSERT_EXPR(0, mp_FbxImporter->GetStatus().GetErrorString());
#endif
		return;
	}

	mp_FbxScene = FbxScene::Create(mp_FbxManager, "For_External_Programs");

#if _DEBUG
	_ASSERT_EXPR(mp_FbxScene, L"FBXLoader - FbxScene failed to create!");
#endif

	mp_FbxImporter->Import(mp_FbxScene);
	mp_FbxImporter->Destroy();

	mp_FbxRootNode = mp_FbxScene->GetRootNode();

	if (mp_FileOutName)
	{
		writeStream = std::ofstream(mp_FileOutName);
		if (writeStream.is_open())
		{
			if (mp_FbxRootNode)
			{
				int numChildren = mp_FbxRootNode->GetChildCount();
				for (int child = 0; child < numChildren; child++)
					PrintNodes(mp_FbxRootNode->GetChild(child));
			}
			writeStream.close();
		}
#if _DEBUG
		else
			_ASSERT_EXPR(0, L"Could Not Open File For Output!");
#endif
	}

	mp_FbxManager->Destroy();
}

void FBXLoaderManager::PrintNodes(fbxsdk::FbxNode* _node)
{
	PrintTabs();

	const char* nodesName = _node->GetName();
	FbxDouble3 nodeTranslation = _node->LclTranslation.Get();
	FbxDouble3 nodeRotation = _node->LclRotation.Get();
	FbxDouble3 nodeScale = _node->LclScaling.Get();

	printf("<Node Name='%s' Translation='(%f, %f, %f)' Rotation='(%f, %f, %f)' Scale='(%f, %f, %f)'>\n",
		nodesName,
		nodeTranslation[0], nodeTranslation[1], nodeTranslation[2],
		nodeRotation[0], nodeRotation[1], nodeRotation[2],
		nodeScale[0], nodeScale[1], nodeScale[2]);

	m_NumTabs++;

	int numNodeAttributes = _node->GetNodeAttributeCount();
	for (int attribute = 0; attribute < numNodeAttributes; attribute++)
		PrintAttribute(_node->GetNodeAttributeByIndex(attribute));

	int numChildren = _node->GetChildCount();
	for (int child = 0; child < numChildren; child++)
		PrintNodes(_node->GetChild(child));

	m_NumTabs--;
	PrintTabs();
	printf("</Node>\n");
}

void FBXLoaderManager::PrintAttribute(FbxNodeAttribute* _nodeAttribute)
{
	if (!_nodeAttribute)
		return;

	FbxString typeName = GetAttributeTypeName(_nodeAttribute);
	FbxString attributeName = _nodeAttribute->GetName();
	PrintTabs();

	printf("<Attribute Type='%s' Name='%s'/>\n", typeName.Buffer(), attributeName.Buffer());
}

FbxString FBXLoaderManager::GetAttributeTypeName(FbxNodeAttribute* _nodeAttribute)
{
	switch (_nodeAttribute->GetAttributeType())
	{
	case fbxsdk::FbxNodeAttribute::eUnknown: return "Unidentified";
	case fbxsdk::FbxNodeAttribute::eNull: return "Null";
	case fbxsdk::FbxNodeAttribute::eMarker: return "Marker";
	case fbxsdk::FbxNodeAttribute::eSkeleton: return "Skeleton";
	case fbxsdk::FbxNodeAttribute::eMesh: return "Mesh";
	case fbxsdk::FbxNodeAttribute::eNurbs: return "Nurbs";
	case fbxsdk::FbxNodeAttribute::ePatch: return "Patch";
	case fbxsdk::FbxNodeAttribute::eCamera: return "Camera";
	case fbxsdk::FbxNodeAttribute::eCameraStereo: return "Camera Stereo";
	case fbxsdk::FbxNodeAttribute::eCameraSwitcher: return "Camera Switcher";
	case fbxsdk::FbxNodeAttribute::eLight: return "Light";
	case fbxsdk::FbxNodeAttribute::eOpticalReference: return "Optical Reference";
	case fbxsdk::FbxNodeAttribute::eOpticalMarker: return "Optical Marker";
	case fbxsdk::FbxNodeAttribute::eNurbsCurve: return "Nurbs Curve";
	case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface: return "Trim Nurbs Surface";
	case fbxsdk::FbxNodeAttribute::eBoundary: return "Boundary";
	case fbxsdk::FbxNodeAttribute::eNurbsSurface: return "Nurbs Surface";
	case fbxsdk::FbxNodeAttribute::eShape: return "Shape";
	case fbxsdk::FbxNodeAttribute::eLODGroup: return "LOD Group";
	case fbxsdk::FbxNodeAttribute::eSubDiv: return "SubDiv";
	case fbxsdk::FbxNodeAttribute::eCachedEffect: return "Cached Effect";
	case fbxsdk::FbxNodeAttribute::eLine: return "Line";
	default: return "Unkown";
	}
}

void FBXLoaderManager::PrintTabs()
{
	for (int tab = 0; tab < m_NumTabs; tab++)
		printf("\t");
}

void FBXLoaderManager::ParseOutFileName()
{
	int totalLength = 1;
	int subLength = 0;
	int lastSlash = 0;

	char* currChar = mp_FbxFileName;
	while (*currChar != '\0')
	{
		if (*currChar == '\\')
			lastSlash = totalLength;
		totalLength++;
		currChar++;

		if (totalLength > 255)
		{
			mp_FileOutName = nullptr;
			return;
		}
	}

	currChar = mp_FbxFileName + lastSlash;
	while (*currChar != '\0' || *currChar != '.')
	{
		subLength++;
		currChar++;
	}

	if (subLength < 1)
	{
		mp_FileOutName = nullptr;
		return;
	}

	mp_FileOutName = new char[subLength + 5];
	memcpy(mp_FileOutName, mp_FbxFileName + lastSlash, subLength);
	memcpy(mp_FileOutName, ".xml", 4);
	mp_FileOutName[subLength + 4] = '\0';
}