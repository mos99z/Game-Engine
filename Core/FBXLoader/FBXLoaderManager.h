#pragma once

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


class FBXLoaderManager
{
private:

	fbxsdk::FbxManager* mp_FbxManager;
	fbxsdk::FbxIOSettings* mp_FbxIOSettings;
	fbxsdk::FbxImporter* mp_FbxImporter;
	fbxsdk::FbxScene* mp_FbxScene;
	fbxsdk::FbxNode* mp_FbxRootNode;
	char* mp_FbxFileName;
	char* mp_FileOutName;
	int m_NumTabs;

	///\ Private Functions 
	void PrintNodes(fbxsdk::FbxNode* _node);
	void PrintAttribute(fbxsdk::FbxNodeAttribute* _nodeAttribute);
	fbxsdk::FbxString GetAttributeTypeName(fbxsdk::FbxNodeAttribute* _nodeAttribute);
	void PrintTabs();
	void ParseOutFileName();

public:

	FBXLoaderManager();
	~FBXLoaderManager();

	void Initilize(char* _FbxFileName);
};

