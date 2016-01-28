#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <fbxsdk.h>
#include <comdef.h>

enum DataType : int8_t { VertexBuffer, IndexBuffer, Texture, MeshData, WeightData, AnimationData };

#pragma region Headers

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

struct WeightsHeader
{
	DataType m_DataType = WeightData;
	unsigned int m_DataByteSize;
};

struct AnimationHeader
{
	DataType m_DataType = AnimationData;
};

#pragma endregion

struct __declspec(align(16)) VBuffer
{
	float m_Position[3]{ 0,0,0 };
	float m_Normal[3]{ 0,1,0 };
	float m_Diffuse[4]{ 1,0,1,1 };
	float empty[2]{ 0,0 };
};

struct __declspec(align(16)) TBuffer
{
	float m_UV[2]{ 0.5f, 0.5f };
	float m_Tangent[4]{ 0,0,1,1 };
	float empty[2]{ 0,0 };
};

struct __declspec(align(16)) ABuffer
{
	unsigned int m_JointIndex[4]{ 0,0,0,0 };
	float m_JointWeight[4]{ 0,0,0,0 };
};

struct Bone
{
	float m_Scaling[3];
	float m_QuaternionRot[4];
	float m_Translation[3];
};

struct KeyFrameBuffer
{
	float m_TimeStamp{ 0 };
	Bone* mv_bones;
};

struct VertexWeight
{
	unsigned int m_JointIndex{ 0 };
	float m_JointWeight{ 0 };
};

struct KeyFrame
{
	float m_TimeStamp{ 0 };
	std::vector<Bone> mv_bones;
};

struct AnimationBuffer
{
	int m_NumKeyFrames{ 0 };
	int m_numBones;
	KeyFrameBuffer* m_KeyFrames;
};

struct Vertex
{
	VBuffer m_vertexData;
	TBuffer m_textureData;
	ABuffer m_animationData;

	bool operator==(const Vertex& _rhs);
};

struct Animation
{
	std::vector<KeyFrame> mv_KeyFrames;
};

struct Joint
{
	int m_ParentIdex;
	fbxsdk::FbxAMatrix m_GlobalBindPoseInverse;
	fbxsdk::FbxNode* m_Node;
	std::string ms_Name;

};

struct ControlPoint
{
	float m_Position[3];
	std::vector<VertexWeight> m_BoneWeights;
};

class FBXLoaderManager
{
private:

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

	CONSOLE_SCREEN_BUFFER_INFO m_ConsoleBufferInfo;
	HANDLE m_ConsoleHandle;
	COORD m_MainProgress;
	COORD m_SecondaryProgress;

	int m_DepthLevel;
	unsigned int m_VBufferByteSize;
	unsigned int m_TBufferByteSize;
	unsigned int m_ABufferByteSize;
	unsigned int m_BoneByteSize;
	int m_numNodes;
	int m_nodesProcessed;
	bool m_FoundMesh;
	bool m_BuiltBaseAnimation;

	std::vector<Joint> mv_Joints;
	std::vector<ControlPoint> m_ControlPoints;
	Animation m_Animation;

	// Private Functions 
	void Uninitilize();
	void HandleNode(fbxsdk::FbxNode* _node);
	void HandleMesh(fbxsdk::FbxNode* _node);
	void HandleControlPoints(fbxsdk::FbxNode* _node);
	void HandleSkeleton(fbxsdk::FbxNode* _node, int _currIndex, int _parentIndex);
	void HandleDeformers(fbxsdk::FbxNode* _node);
	unsigned int GetJointIndexByName(std::string _name);
	//void GatherAnimationData();
	void ParseOutFileName();

public:

	FBXLoaderManager();
	~FBXLoaderManager();

	void Initilize(const wchar_t* _FbxFileName);
};

