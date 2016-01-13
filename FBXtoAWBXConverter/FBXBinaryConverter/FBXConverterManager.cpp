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
}

FBXLoaderManager::~FBXLoaderManager()
{
	Uninitilize();
}

void FBXLoaderManager::Uninitilize()
{
	m_DepthLevel = 0;
	if (writeStream.is_open())
		writeStream.close();
}

void FBXLoaderManager::Initilize(const wchar_t* _FbxFileName)
{
	Uninitilize();


#if _DEBUG
	_ASSERT_EXPR(_FbxFileName, L"FBXLoader - Passed In File Name Can NOT Be Null!");
#endif

	ms_FbxFileName = ms_AWBXFileName = _FbxFileName;
	ParseOutFileName();

	mp_FbxManager = FbxManager::Create();

#if _DEBUG
	_ASSERT_EXPR(mp_FbxManager, L"FBXLoader - FbxManager failed to create!");
#endif

	mp_FbxGeoConverter = new FbxGeometryConverter(mp_FbxManager);
	mp_FbxIOSettings = FbxIOSettings::Create(mp_FbxManager, IOSROOT);

#if _DEBUG
	_ASSERT_EXPR(mp_FbxIOSettings, L"FBXLoader - FbxIOSettings failed to create!");
#endif

	mp_FbxManager->SetIOSettings(mp_FbxIOSettings);
	mp_FbxImporter = FbxImporter::Create(mp_FbxManager, "");

#if _DEBUG
	_ASSERT_EXPR(mp_FbxImporter, L"FBXLoader - FbxImporter failed to create!");
#endif

	char* deleteMe = new char[ms_FbxFileName.size()+1];
	deleteMe[ms_FbxFileName.size()] = '\0';
	wcstombs(deleteMe, ms_FbxFileName.c_str(), ms_FbxFileName.size());
	if (!mp_FbxImporter->Initialize(deleteMe, -1, mp_FbxManager->GetIOSettings()))
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
		delete[] deleteMe;
		return;
	}
	delete[] deleteMe;

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
		writeStream.open(ms_AWBXFileName.c_str(), std::ios_base::binary | std::ios_base::trunc);

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
			if (!_node->GetMesh()->IsTriangleMesh())
			{
				mp_FbxGeoConverter->Triangulate(_node->GetNodeAttributeByIndex(attribute), false);
			}
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

void FBXLoaderManager::ParseOutFileName()
{
	int letterCount = 0;
	int deleteAt = 0;
	size_t stringSize = ms_AWBXFileName.size();
	while (stringSize > letterCount)
	{
		if (ms_AWBXFileName.at(letterCount) == L'.')
			deleteAt = letterCount;
		letterCount++;
	}

	if (deleteAt > 0)
	{
		ms_AWBXFileName.erase(deleteAt);
		ms_AWBXFileName.append(L".AWBX");
	}
}





void FBXLoaderManager::HandleMesh(fbxsdk::FbxNode* _node)
{
	FbxMesh* nodeMesh = _node->GetMesh();

	////////////////////////////////////////////////////////////*******************////////////////////TODO: Austin is stupid !
	_node->EvaluateGlobalTransform();

	// Write Out Mesh Header Data
	DataType meshDataType = MeshData;
	writeStream.write((char*)&meshDataType, sizeof(DataType));
	const char* meshNamePointer = nodeMesh->GetName();
	int16_t nameSize = (int16_t)strlen(meshNamePointer);
	writeStream.write((char*)&nameSize, sizeof(int16_t));
	writeStream.write(meshNamePointer, nameSize);



	unsigned int m_numVerts = nodeMesh->GetControlPointsCount();
	VBuffer* m_VertBuffer = new VBuffer[m_numVerts];

	unsigned int m_numIndexes = nodeMesh->GetPolygonVertexCount();
	unsigned int* m_IndexBuffer = new unsigned int[m_numIndexes];
	memcpy_s(m_IndexBuffer, m_numIndexes * sizeof(unsigned int), nodeMesh->GetPolygonVertices(), m_numIndexes  * sizeof(unsigned int));

	auto versBuffer = nodeMesh->GetControlPoints();

	// For each polygon in the input mesh
	for (int iPolygon = 0; iPolygon < nodeMesh->GetPolygonCount(); iPolygon++)
	{
		// For each vertex in the polygon
		for (unsigned iPolygonVertex = 0; iPolygonVertex < 3; iPolygonVertex++)
		{
			int fbxCornerIndex = nodeMesh->GetPolygonVertex(iPolygon, iPolygonVertex);

			// Get vertex position
			fbxsdk::FbxVector4 fbxVertex = versBuffer[fbxCornerIndex];
			m_VertBuffer[fbxCornerIndex].m_Position[0] = (float)fbxVertex.mData[0];
			m_VertBuffer[fbxCornerIndex].m_Position[1] = (float)fbxVertex.mData[1];
			m_VertBuffer[fbxCornerIndex].m_Position[2] = (float)fbxVertex.mData[2];

			// Get normal
			fbxsdk::FbxVector4 fbxNormal;
			nodeMesh->GetPolygonVertexNormal(iPolygon, iPolygonVertex, fbxNormal);
			fbxNormal.Normalize();
			m_VertBuffer[fbxCornerIndex].m_Normal[0] = (float)fbxNormal.mData[0];
			m_VertBuffer[fbxCornerIndex].m_Normal[1] = (float)fbxNormal.mData[1];
			m_VertBuffer[fbxCornerIndex].m_Normal[2] = (float)fbxNormal.mData[2];

			// Get texture coordinate
			fbxsdk::FbxVector2 fbxUV = fbxsdk::FbxVector2(0.0, 0.0);
			fbxsdk::FbxLayerElementUV* fbxLayerUV = nodeMesh->GetLayer(0)->GetUVs();
			if (fbxLayerUV)
			{
				int iUVIndex = 0;
				switch (fbxLayerUV->GetMappingMode())
				{
				case fbxsdk::FbxLayerElement::eByControlPoint:
					iUVIndex = fbxCornerIndex;
					break;

				case fbxsdk::FbxLayerElement::eByPolygonVertex:
					iUVIndex = nodeMesh->GetTextureUVIndex(iPolygon, iPolygonVertex);
					break;
				}
				fbxUV = fbxLayerUV->GetDirectArray().GetAt(iUVIndex);
				m_VertBuffer[fbxCornerIndex].m_Diffuse[1] = (float)fbxUV.mData[0];
				m_VertBuffer[fbxCornerIndex].m_Diffuse[0] = (float)fbxUV.mData[1];
			}												
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