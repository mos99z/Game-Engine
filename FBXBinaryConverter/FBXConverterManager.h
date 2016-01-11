#pragma once
#include <iostream>
#include <string>

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
};

enum DataType : int8_t { VertexBuffer, IndexBuffer, Texture, MeshData };


struct BinaryFileHeaderData
{
	int8_t m_FileVarification[4]{ 'A', 'W', 'B', 'X' };
};

struct MeshHeader
{
	DataType m_DataType = MeshData;
	int16_t m_MeshNameSize;
	int8_t* m_MeshName;
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
	float m_Normal[3]{ 0,0,1 };
	float m_Diffuse[4]{ 1,0,1,1 };
};

class FBXLoaderManager
{
private:
	std::string ms_FilePath = "..\\Assets\\FBXs\\";

	// Importer / Scene
	fbxsdk::FbxManager* mp_FbxManager;
	fbxsdk::FbxIOSettings* mp_FbxIOSettings;
	fbxsdk::FbxImporter* mp_FbxImporter;
	fbxsdk::FbxScene* mp_FbxScene;
	fbxsdk::FbxNode* mp_FbxRootNode;
	fbxsdk::FbxGeometryConverter* mp_FbxGeoConverter;
	std::string ms_FbxFileName;
	std::string ms_FileOutName;
	int m_NumTabs;

	int m_DepthLevel;

	/*int m_numVerts;
	int m_numIndexes;
	VBuffer* m_VertBuffer;
	unsigned int* m_IndexBuffer;*/

	// Private Functions 
	void Uninitilize();
	void PrintNodes(fbxsdk::FbxNode* _node);
	void HandleNode(fbxsdk::FbxNode* _node);
	void PrintAttribute(fbxsdk::FbxNodeAttribute* _nodeAttribute);
	fbxsdk::FbxString GetAttributeTypeName(fbxsdk::FbxNodeAttribute* _nodeAttribute);
	void PrintTabs();
	void ParseOutFileName();

	void HandleMesh(fbxsdk::FbxNode* _node);

public:

	FBXLoaderManager();
	~FBXLoaderManager();

	static inline const wchar_t* GetFilePathAndTypes() { return L"..\\Assets\\FBXs\\*.fbx"; };
	void Initilize(wchar_t* _FbxFileName);
};

