#define _CRT_SECURE_NO_WARNINGS

#include "FBXConverterManager.h"
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
	/*m_VertBuffer = nullptr;
	m_IndexBuffer = nullptr;*/
}

FBXLoaderManager::~FBXLoaderManager()
{
	Uninitilize();
}

void FBXLoaderManager::Uninitilize()
{
	/*if (m_VertBuffer)
		delete[] m_VertBuffer;
	if (m_IndexBuffer)
		delete[] m_IndexBuffer;*/

	m_NumTabs = 0;
	m_DepthLevel = 0;
	/*m_numVerts = 0;
	m_numIndexes = 0;*/

	if (writeStream.is_open())
		writeStream.close();
}

void FBXLoaderManager::Initilize(wchar_t* _FbxFileName)
{

#if _DEBUG
	_ASSERT_EXPR(_FbxFileName, L"FBXLoader - Passed In File Name Can NOT Be Null!");
#endif

	ms_FbxFileName = ms_FileOutName = ms_FilePath;
	_bstr_t wideToChar(_FbxFileName);
	ms_FileOutName.append(wideToChar);
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
		if (writeStream.is_open())
			writeStream.close();
		writeStream.open(ms_FileOutName, std::ios_base::binary | std::ios_base::trunc);

		if (writeStream.is_open())
		{
			writeStream.write("AWBX", 4);
			int numChildren = mp_FbxRootNode->GetChildCount();
			for (int child = 0; child < numChildren; child++)
				HandleNode(mp_FbxRootNode->GetChild(child));

			writeStream.close();
		}
	}

	mp_FbxManager->Destroy();
}

void FBXLoaderManager::HandleNode(FbxNode* _node)
{
	m_DepthLevel++;
	int numNodeAttributes = _node->GetNodeAttributeCount();
	for (int attribute = 0; attribute < numNodeAttributes; attribute++)
	{
		FbxNodeAttribute::EType nodeAttributeType = _node->GetNodeAttributeByIndex(attribute)->GetAttributeType();
		switch (nodeAttributeType)
		{
		case FbxNodeAttribute::eUnknown: break;
		case FbxNodeAttribute::eNull: break;
		case FbxNodeAttribute::eMarker: break;
		case FbxNodeAttribute::eSkeleton: break;
		case FbxNodeAttribute::eMesh:
		{
			mp_FbxGeoConverter->Triangulate(_node->GetNodeAttributeByIndex(attribute), false);
			HandleMesh(_node);
			break;
		}
		case FbxNodeAttribute::eNurbs: break;
		case FbxNodeAttribute::ePatch: break;
		case FbxNodeAttribute::eCamera: break;
		case FbxNodeAttribute::eCameraStereo: break;
		case FbxNodeAttribute::eCameraSwitcher: break;
		case FbxNodeAttribute::eLight: break;
		case FbxNodeAttribute::eOpticalReference: break;
		case FbxNodeAttribute::eOpticalMarker: break;
		case FbxNodeAttribute::eNurbsCurve: break;
		case FbxNodeAttribute::eTrimNurbsSurface: break;
		case FbxNodeAttribute::eBoundary: break;
		case FbxNodeAttribute::eNurbsSurface: break;
		case FbxNodeAttribute::eShape: break;
		case FbxNodeAttribute::eLODGroup: break;
		case FbxNodeAttribute::eSubDiv: break;
		case FbxNodeAttribute::eCachedEffect: break;
		case FbxNodeAttribute::eLine: break;
		default: break;
		}
	}

	int numChildren = _node->GetChildCount();
	for (int child = 0; child < numChildren; child++)
		HandleNode(_node->GetChild(child));

	m_DepthLevel--;
}

#pragma region Old Code
void FBXLoaderManager::PrintNodes(FbxNode* _node)
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
#pragma endregion

void FBXLoaderManager::ParseOutFileName()
{
	int letterCount = 0;
	int deleteAt = 0;
	size_t stingSize = ms_FileOutName.size();
	while (stingSize > letterCount)
	{
		if (ms_FileOutName.at(letterCount) == '.')
			deleteAt = letterCount;
		letterCount++;
	}

	if (deleteAt > 0)
		ms_FileOutName.erase(deleteAt);

	ms_FileOutName.append(".AWBX");
}

void FBXLoaderManager::HandleMesh(fbxsdk::FbxNode* _node)
{
	FbxMesh* nodeMesh = _node->GetMesh();

	// Write Out Mesh Header Data
	DataType meshDataType = MeshData;
	writeStream.write((char*)&meshDataType, sizeof(DataType));
	const char* meshNamePointer = nodeMesh->GetName();
	int16_t nameSize = (int16_t)strlen(meshNamePointer);
	writeStream.write((char*)&nameSize, sizeof(int16_t));
	writeStream.write(meshNamePointer, nameSize);

	FbxVector4 currVector;
	FbxVector4 currNormal;
	int polyCount = nodeMesh->GetPolygonCount();
	int polySize;

	unsigned int m_numVerts = polyCount * 3;
	VBuffer* m_VertBuffer = new VBuffer[m_numVerts];

	unsigned int m_numIndexes = nodeMesh->GetPolygonVertexCount();
	unsigned int* m_IndexBuffer = new unsigned int[m_numIndexes];
	memcpy_s(m_IndexBuffer, m_numIndexes * sizeof(unsigned int), nodeMesh->GetPolygonVertices(), m_numIndexes  * sizeof(unsigned int));

	for (int poly = 0; poly < polyCount; poly++)
	{
		polySize = nodeMesh->GetPolygonSize(poly);
		for (int subPoly = 0; subPoly < polySize; subPoly++)
		{
			currVector = nodeMesh->GetControlPointAt(nodeMesh->GetPolygonVertex(poly, subPoly));
			m_VertBuffer[poly].m_Position[0] = (float)currVector[0];
			m_VertBuffer[poly].m_Position[1] = (float)currVector[1];
			m_VertBuffer[poly].m_Position[2] = (float)currVector[2];

			nodeMesh->GetPolygonVertexNormal(poly, subPoly, currNormal);
			currNormal.Normalize();
			m_VertBuffer[poly].m_Normal[0] = (float)currNormal[0];
			m_VertBuffer[poly].m_Normal[1] = (float)currNormal[1];
			m_VertBuffer[poly].m_Normal[2] = (float)currNormal[2];

		}
	}

	// Write Out Vertex Header
	VertexHeader newVertHeader;
	newVertHeader.m_DataByteSize = m_numVerts * sizeof(VBuffer);
	writeStream.write((char*)&newVertHeader.m_DataType, sizeof(DataType));
	writeStream.write((char*)&newVertHeader.m_DataByteSize, sizeof(unsigned int));

	// Write Out Vertex Buffer
	writeStream.write((char*)m_VertBuffer, newVertHeader.m_DataByteSize);

	delete[] m_VertBuffer;
	m_VertBuffer = nullptr;

	// Write Out Index Header
	IndexHeader newIndexHeader;
	newIndexHeader.m_DataByteSize = m_numIndexes * sizeof(unsigned int);
	writeStream.write((char*)&newIndexHeader.m_DataType, sizeof(DataType));
	writeStream.write((char*)&newIndexHeader.m_DataByteSize, sizeof(unsigned int));

	// Write Out Index Buffer
	writeStream.write((char*)m_IndexBuffer, newIndexHeader.m_DataByteSize);

	delete[] m_IndexBuffer;
	m_IndexBuffer = nullptr;
}