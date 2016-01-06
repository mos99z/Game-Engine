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
};

enum DataType { VertexBuffer, IndexBuffer, Texture };

struct BinaryFileHeaderData
{
	char m_FileVarification[4]{ 'B', 'F', 'b', 'x' };
	int m_TotalDataSize = 0;
};

struct ObjectHeader
{
	int m_DataSize = 0;
	DataType m_DataType;
	float worldMatrix[3];
};

struct __declspec(align(16)) Gstream
{
	float position[3];
	float normal[3];
	float diffuse[4]{1,0,1,1};
	float specular[2];
	float UV[2];
	float tangent[2];
	int bone[4]{0,0,0,0};
	float weights[4]{0,0,0,0};
};

class FBXLoaderManager
{
private:
	std::string ms_FilePath = "..\\..\\Assets\\FBXs\\";

	fbxsdk::FbxManager* mp_FbxManager;
	fbxsdk::FbxIOSettings* mp_FbxIOSettings;
	fbxsdk::FbxImporter* mp_FbxImporter;
	fbxsdk::FbxScene* mp_FbxScene;
	fbxsdk::FbxNode* mp_FbxRootNode;
	std::string ms_FbxFileName;
	std::string ms_FileOutName;
	int m_NumTabs;

	/// Private Functions 
	void PrintNodes(fbxsdk::FbxNode* _node);
	void PrintAttribute(fbxsdk::FbxNodeAttribute* _nodeAttribute);
	fbxsdk::FbxString GetAttributeTypeName(fbxsdk::FbxNodeAttribute* _nodeAttribute);
	void PrintTabs();
	void ParseOutFileName();

public:

	FBXLoaderManager();
	~FBXLoaderManager();

	static inline const wchar_t* GetFilePathAndTypes() { return L"..\\..\\Assets\\FBXs\\*.fbx"; };
	void Initilize(wchar_t* _FbxFileName);
};

