#define _CRT_SECURE_NO_WARNINGS
#define FLIP_UV_Y false
#define PROGRESSTEXT false

#include "FBXConverterManager.h"
#include <fbxsdk.h>
#include <comdef.h>
#include <vector>

unsigned int m_VertexByteSize = sizeof(Vertex);

FBXLoaderManager::FBXLoaderManager()
{
	mp_FbxManager = nullptr;
	mp_FbxIOSettings = nullptr;
	mp_FbxImporter = nullptr;
	mp_FbxScene = nullptr;
	mp_FbxRootNode = nullptr;

	m_VBufferByteSize = sizeof(VBuffer);
	m_TBufferByteSize = sizeof(TBuffer);

	m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	m_MainProgress = { 0,0 };
	m_SecondaryProgress = { 0,1 };
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

	char* deleteMe = new char[ms_FbxFileName.size() + 1];
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
	FbxAxisSystem changeHandedness(FbxAxisSystem::eDirectX);
	changeHandedness.ConvertScene(mp_FbxScene);

#if _DEBUG
	_ASSERT_EXPR(mp_FbxScene, L"FBXLoader - FbxScene failed to create!");
#endif

	mp_FbxImporter->Import(mp_FbxScene);
	mp_FbxImporter->Destroy();

	mp_FbxRootNode = mp_FbxScene->GetRootNode();

#if PROGRESSTEXT
	m_numNodes = mp_FbxScene->GetNodeCount();
	m_nodesProcessed = 1;
#endif

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
#if PROGRESSTEXT
	SetConsoleCursorPosition(m_ConsoleHandle, m_MainProgress);
	std::wprintf(L"Processing filename %ls  Node %i of %i      ", ms_ActualFileName.c_str(), m_nodesProcessed, m_numNodes);
	m_nodesProcessed++;
#endif
}

void FBXLoaderManager::ParseOutFileName()
{
	int letterCount = 0;
	int startindex = 0;
	int deleteAt = 0;
	size_t stringSize = ms_AWBXFileName.size();
	while (stringSize > letterCount)
	{
		if (ms_AWBXFileName.at(letterCount) == L'.')
			deleteAt = letterCount;
		if (ms_AWBXFileName.at(letterCount) == L'\\')
			startindex = letterCount;
		letterCount++;
	}

	if (deleteAt > 0)
	{
		ms_AWBXFileName.erase(deleteAt);
		ms_ActualFileName = (ms_AWBXFileName.c_str() + startindex + 1);
		ms_AWBXFileName.append(L".AWBX");
	}
}

void FBXLoaderManager::HandleMesh(fbxsdk::FbxNode* _node)
{
	FbxMesh* nodeMesh = _node->GetMesh();

	// Write Out Mesh Header Data
	DataType meshDataType = MeshData;
	writeStream.write((char*)&meshDataType, sizeof(DataType));

	// TODO: Implement parent index tracker for mesh hierarchy
	//unsigned int parentIndex;

	fbxsdk::FbxAMatrix globalMatrix = _node->EvaluateGlobalTransform();
	writeStream.write((char*)&globalMatrix, sizeof(fbxsdk::FbxDouble4) * 4);

	std::vector<Vertex> uniqueVerticies;
	unsigned int m_numUniqueVerts = 0;

	unsigned int m_numIndexes = nodeMesh->GetPolygonVertexCount();
	unsigned int* m_IndexBuffer = new unsigned int[m_numIndexes];

	auto versBuffer = nodeMesh->GetControlPoints();

	// For each polygon in the input mesh
	int nodeMeshPolyCount = nodeMesh->GetPolygonCount();
	int currVert = 0;
	for (int iPolygon = 0; iPolygon < nodeMeshPolyCount; iPolygon++)
	{
		_ASSERT_EXPR(3 == nodeMesh->GetPolygonSize(iPolygon), L"Polygon does not have exactly 3 verts!");
		// For each vertex in the polygon
		for (unsigned iPolygonVertex = 0; iPolygonVertex < 3; iPolygonVertex++)
		{

#if PROGRESSTEXT
			SetConsoleCursorPosition(m_ConsoleHandle, m_SecondaryProgress);
			std::wprintf(L"Processing Poly %i of %i Polygons          ", iPolygon, nodeMeshPolyCount);
#endif

			int fbxCornerIndex = nodeMesh->GetPolygonVertex(iPolygon, iPolygonVertex);
			Vertex newVertex;

#pragma region Get Verticies

			fbxsdk::FbxVector4 fbxVertex = versBuffer[fbxCornerIndex];
			newVertex.m_vertexData.m_Position[0] = -(float)fbxVertex[0];
			newVertex.m_vertexData.m_Position[1] = (float)fbxVertex[1];
			newVertex.m_vertexData.m_Position[2] = (float)fbxVertex[2];

#pragma endregion

#pragma region Get Normals

			fbxsdk::FbxVector4 fbxNormal;
			nodeMesh->GetPolygonVertexNormal(iPolygon, iPolygonVertex, fbxNormal);
			fbxNormal.Normalize();
			newVertex.m_vertexData.m_Normal[0] = (float)fbxNormal[0];
			newVertex.m_vertexData.m_Normal[1] = (float)fbxNormal[1];
			newVertex.m_vertexData.m_Normal[2] = -(float)fbxNormal[2];

#pragma endregion

#pragma region Get UVs

			fbxsdk::FbxVector2 fbxUV = fbxsdk::FbxVector2(0.0, 0.0);
			fbxsdk::FbxLayerElementUV* elementUV = nodeMesh->GetLayer(0)->GetUVs();
			if (elementUV)
			{
				int iUVIndex = fbxCornerIndex;
				switch (elementUV->GetMappingMode())
				{
				case fbxsdk::FbxLayerElement::eByControlPoint:
				{
					switch (elementUV->GetReferenceMode())
					{
					case FbxGeometryElement::eIndexToDirect:
						iUVIndex = elementUV->GetIndexArray().GetAt(fbxCornerIndex);
						break;
					default: break;
					}
					break;
				}
				case fbxsdk::FbxLayerElement::eByPolygonVertex:
				{
					switch (elementUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
						iUVIndex = nodeMesh->GetTextureUVIndex(iPolygon, iPolygonVertex);
						break;
					default: break;
					}
					break;
				}
				default: break;
				}
				fbxUV = elementUV->GetDirectArray().GetAt(iUVIndex);
				newVertex.m_textureData.m_UV[0] = (float)fbxUV[0];
				newVertex.m_textureData.m_UV[1] = (float)fbxUV[1];

				if (FLIP_UV_Y)
					newVertex.m_textureData.m_UV[1] = 1.0f - (float)fbxUV[1];
			}

#pragma endregion

#pragma region Get Tangents

			FbxVector4 Tangent;
			FbxGeometryElementTangent* elementTangent = nodeMesh->GetElementTangent(0);
			if (elementTangent)
			{
				int tangentIndex = fbxCornerIndex;
				FbxLayerElement::EMappingMode tangentMapMode = elementTangent->GetMappingMode();
				if (tangentMapMode == FbxGeometryElement::eByPolygonVertex)
				{
					switch (elementTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						tangentIndex = currVert;
						break;
					case FbxGeometryElement::eIndexToDirect:
						tangentIndex = elementTangent->GetIndexArray().GetAt(currVert);
						break;
					default: break;
					}
				}
				else if (tangentMapMode == FbxGeometryElement::eByControlPoint)
				{
					switch (elementTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eIndexToDirect:
						tangentIndex = elementTangent->GetIndexArray().GetAt(fbxCornerIndex);
						break;
					default: break;
					}
				}
				Tangent = elementTangent->GetDirectArray().GetAt(tangentIndex);
				newVertex.m_textureData.m_Tangent[0] = (float)Tangent[0];
				newVertex.m_textureData.m_Tangent[1] = (float)Tangent[1];
				newVertex.m_textureData.m_Tangent[2] = (float)Tangent[2];
				newVertex.m_textureData.m_Tangent[3] = (float)Tangent[3];
			}

#pragma endregion

			bool unique = true;
			for (unsigned int uniqueVert = 0; uniqueVert < m_numUniqueVerts; uniqueVert++)
			{
				if (uniqueVerticies[uniqueVert] == newVertex)
				{
					m_IndexBuffer[currVert] = uniqueVert;
					unique = false;
					break;
				}
			}

			if (unique)
			{
				uniqueVerticies.push_back(newVertex);
				m_IndexBuffer[currVert] = m_numUniqueVerts;
				m_numUniqueVerts++;
			}

			currVert++;
	}//End each Vert
}//End Each Poly

	VBuffer* m_VertBuffer = new VBuffer[m_numUniqueVerts];
	TBuffer* m_TextureBuffer = new TBuffer[m_numUniqueVerts];

	for (unsigned int currVert = 0; currVert < m_numUniqueVerts; currVert++)
	{
		memcpy(&m_VertBuffer[currVert], &uniqueVerticies[currVert].m_vertexData, m_VBufferByteSize);
		memcpy(&m_TextureBuffer[currVert], &uniqueVerticies[currVert].m_textureData, m_TBufferByteSize);
	}

#pragma region Write out Data Buffers

#pragma region Vertex Data

	VertexHeader newVertHeader;
	newVertHeader.m_DataByteSize = m_numUniqueVerts * m_VBufferByteSize;
	writeStream.write((char*)&newVertHeader.m_DataType, sizeof(DataType));
	writeStream.write((char*)&newVertHeader.m_DataByteSize, sizeof(unsigned int));
	writeStream.write((char*)m_VertBuffer, newVertHeader.m_DataByteSize);

	delete[] m_VertBuffer;
	m_VertBuffer = nullptr;

#pragma endregion

#pragma region Index Data

	IndexHeader newIndexHeader;
	newIndexHeader.m_DataByteSize = m_numIndexes * sizeof(unsigned int);
	writeStream.write((char*)&newIndexHeader.m_DataType, sizeof(DataType));
	writeStream.write((char*)&newIndexHeader.m_DataByteSize, sizeof(unsigned int));
	writeStream.write((char*)m_IndexBuffer, newIndexHeader.m_DataByteSize);

	delete[] m_IndexBuffer;
	m_IndexBuffer = nullptr;

#pragma endregion

#pragma region Texture Data

	TextureHeader newTextureHeader;
	newTextureHeader.m_DataByteSize = m_numUniqueVerts * m_TBufferByteSize;
	writeStream.write((char*)&newTextureHeader.m_DataType, sizeof(DataType));
	writeStream.write((char*)&newTextureHeader.m_DataByteSize, sizeof(unsigned int));
	writeStream.write((char*)m_TextureBuffer, newTextureHeader.m_DataByteSize);

	delete[] m_TextureBuffer;
	m_TextureBuffer = nullptr;

#pragma endregion

#pragma endregion

}

bool Vertex::operator==(const Vertex& _rhs)
{
	//return !memcmp(this, &_rhs, m_VertexByteSize);

	if (this->m_vertexData.m_Position[0] != _rhs.m_vertexData.m_Position[0] ||
		this->m_vertexData.m_Position[1] != _rhs.m_vertexData.m_Position[1] ||
		this->m_vertexData.m_Position[2] != _rhs.m_vertexData.m_Position[2] ||
		this->m_textureData.m_UV[0] != _rhs.m_textureData.m_UV[0] ||
		this->m_textureData.m_UV[1] != _rhs.m_textureData.m_UV[1] ||
		this->m_vertexData.m_Normal[0] != _rhs.m_vertexData.m_Normal[0] ||
		this->m_vertexData.m_Normal[1] != _rhs.m_vertexData.m_Normal[1] ||
		this->m_vertexData.m_Normal[2] != _rhs.m_vertexData.m_Normal[2] ||
		this->m_textureData.m_Tangent[0] != _rhs.m_textureData.m_Tangent[0] ||
		this->m_textureData.m_Tangent[1] != _rhs.m_textureData.m_Tangent[1] ||
		this->m_textureData.m_Tangent[2] != _rhs.m_textureData.m_Tangent[2] ||
		this->m_textureData.m_Tangent[3] != _rhs.m_textureData.m_Tangent[3] ||
		this->m_vertexData.m_Diffuse[0] != _rhs.m_vertexData.m_Diffuse[0] ||
		this->m_vertexData.m_Diffuse[1] != _rhs.m_vertexData.m_Diffuse[1] ||
		this->m_vertexData.m_Diffuse[2] != _rhs.m_vertexData.m_Diffuse[2] ||
		this->m_vertexData.m_Diffuse[3] != _rhs.m_vertexData.m_Diffuse[3])
		return false;
	return true;
}