#define _CRT_SECURE_NO_WARNINGS

#include "FBXLoaderManager.h"
#include <fbxsdk.h>
#include <fstream>
#include <comdef.h>

static std::ofstream writeStream;

FBXLoaderManager::FBXLoaderManager()
{
	mp_FbxManager = nullptr;
	mp_FbxIOSettings = nullptr;
	mp_FbxImporter = nullptr;
	mp_FbxScene = nullptr;
	mp_FbxRootNode = nullptr;
}

FBXLoaderManager::~FBXLoaderManager()
{
}

void FBXLoaderManager::Initilize(wchar_t* _FbxFileName)
{

#if _DEBUG
	_ASSERT_EXPR(_FbxFileName, L"FBXLoader - Passed In File Name Can NOT Be Null!");
#endif

	ms_FbxFileName = ms_FilePath;
	m_NumTabs = 0;

	_bstr_t wideToChar(_FbxFileName);
	ms_FileOutName = wideToChar;
	ms_FbxFileName.append(wideToChar);
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

	if (!mp_FbxImporter->Initialize(ms_FbxFileName.c_str(), -1, mp_FbxManager->GetIOSettings()))
	{
#if _DEBUG
		std::wstring message = L"Fbx Importer failed to initilize With the following error message: ";
		const char* charErrorMessage = mp_FbxImporter->GetStatus().GetErrorString();
		size_t errorMessageLength = strlen(charErrorMessage) + 1;
		wchar_t* errorMessage = new wchar_t[errorMessageLength];
		mbstowcs(errorMessage, charErrorMessage, errorMessageLength);
		message.append(errorMessage);
		_ASSERT_EXPR(0, message.c_str());
		delete[] errorMessage;
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

	if (mp_FbxRootNode)
	{
		int numChildren = mp_FbxRootNode->GetChildCount();
		for (int child = 0; child < numChildren; child++)
			PrintNodes(mp_FbxRootNode->GetChild(child));
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
	int deleteFrom = 0;
	while (ms_FileOutName.at(deleteFrom) != '.')
		deleteFrom++;

	if (deleteFrom > 0)
		ms_FileOutName.erase(deleteFrom);
}