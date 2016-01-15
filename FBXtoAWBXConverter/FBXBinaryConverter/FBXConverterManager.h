#pragma once
#include <iostream>
#include <string>
#include <fstream>

namespace fbxsdk
{
	class FbxManager;
	class FbxIOSettings;
	class FbxImporter;
	class FbxScene;
	class FbxNode;
	class FbxNodeAttribute;
	class FbxString;
	class FbxGeometryConverter;
	class FbxAMatrix;
};

enum DataType : int8_t { VertexBuffer, IndexBuffer, Texture, MeshData };


struct BinaryFileHeaderData
{
	int8_t m_FileVarification[4]{ 'A', 'W', 'B', 'X' };
};

struct MeshHeader
{
	DataType m_DataType = MeshData;
	//unsigned int m_MeshDataByteSize;
};

struct VertexHeader
{
	DataType m_DataType = VertexBuffer;
	unsigned int m_DataByteSize;
};

struct IndexHeader
{
	DataType m_DataType = IndexBuffer;
	unsigned int m_DataByteSize;
};

struct TextureHeader
{
	DataType m_DataType = Texture;
	unsigned int m_DataByteSize;
};

struct __declspec(align(16)) VBuffer
{
	float m_Position[3]{ 0,0,0 };
	float m_Normal[3]{ 0,1,0 };
	float m_Diffuse[4]{ 1,0,1,1 };
};

struct __declspec(align(16)) TBuffer
{
	float m_UV[2]{ 0.5f, 0.5f };
	float m_Tangent[4]{0,0,1,1};
};

struct Vertex
{
	VBuffer m_vertexData;
	TBuffer m_textureData;

	bool operator==(const Vertex& _rhs);
};

class FBXLoaderManager
{
private:
	// Importer / Scene
	fbxsdk::FbxManager* mp_FbxManager;
	fbxsdk::FbxIOSettings* mp_FbxIOSettings;
	fbxsdk::FbxImporter* mp_FbxImporter;
	fbxsdk::FbxScene* mp_FbxScene;
	fbxsdk::FbxNode* mp_FbxRootNode;
	fbxsdk::FbxGeometryConverter* mp_FbxGeoConverter;
	std::wstring ms_FbxFileName;
	std::wstring ms_AWBXFileName;
	std::wstring ms_ActualFileName;

	std::ofstream writeStream;

	int m_DepthLevel;
	unsigned int m_VBufferByteSize;
	unsigned int m_TBufferByteSize;
	float m_numNodes;
	float m_nodesProcessed;

	// Private Functions 
	void Uninitilize();
	void HandleNode(fbxsdk::FbxNode* _node);
	void HandleMesh(fbxsdk::FbxNode* _node);
	void ParseOutFileName();

public:

	FBXLoaderManager();
	~FBXLoaderManager();

	void Initilize(const wchar_t* _FbxFileName);
};

