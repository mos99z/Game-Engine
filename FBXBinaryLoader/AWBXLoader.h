#ifdef AWBXLOADERDLL_EXPORTS
#define AWBXLOADERDLL_API __declspec(dllexport) 
#else
#define AWBXLOADERDLL_API __declspec(dllimport) 
#endif

#pragma once
#include <iostream>
#include <vector>
#define AWBXLOADERDLL_EXPORTS

namespace AWBX
{
	class AWBXLoader
	{
	private:

		enum DataType : int8_t { VertexBuffer, IndexBuffer, Texture, MeshData };

		struct MeshHeader
		{
			DataType m_DataType = MeshData;
			int16_t m_MeshNameSize;
			int8_t* m_MeshName;
		};

		struct VertexHeader
		{
			DataType m_DataType = VertexBuffer;
			int32_t m_DataByteSize;
		};

		struct IndexHeader
		{
			DataType m_DataType = IndexBuffer;
			int32_t m_DataByteSize;
		};

		struct __declspec(align(16)) VBuffer
		{
			float m_Position[3]{ 0,0,0 };
			float m_Normal[3]{ 0,0,1 };
			float m_Diffuse[4]{ 1,0,1,1 };
		};

		struct __declspec(align(16)) TBuffer
		{
			float m_UV[2]{ 0.5f, 0.5f };
			float m_Tangent[4]{ 0,0,1,1 };
		};

		std::vector<unsigned int> m_VertexSizes;
		std::vector<VBuffer*> m_VertexData;
		std::vector<TBuffer*> m_TextureData;

		std::vector<unsigned int> m_IndexSizes;
		std::vector<unsigned int*> m_IndexData;
		std::vector<double*> m_Matricies;

		bool m_SingleMesh;
		int m_currNumMeshes;

		unsigned int m_totalVertexes;
		unsigned int m_totalIndexes;
		unsigned int m_UIntSize;
		unsigned int m_DoubleSize;
		unsigned int m_MatrixByteSize;
		unsigned int m_VBufferSize;
		unsigned int m_TBufferSize;

		void HandleMeshHeader();
		void HandleVertexBuffer();
		void HandleIndexBuffer();
		void HandleTextureBuffer();

		void DeleteAllocatedMemory();

		bool AWBXLoader::OpenFile(const char* _filePath);
		bool AWBXLoader::ParseFile();

	public:
		AWBXLOADERDLL_API AWBXLoader();
		AWBXLOADERDLL_API ~AWBXLoader();

		/// Out_VertexData Layout:
		///		float Position[3]
		///		float Normal[3]
		///		float Diffuse[4]
		AWBXLOADERDLL_API bool LoadAWBXMeshes(const char* IN_AWBXFilePath, int& OUT_numMeshes, unsigned int** OUT_NumVerts, void*** OUT_VertexData, void*** OUT_TextureData, unsigned int** OUT_NumIndexes, unsigned int*** OUT_IndexData, double*** OUT_matrixArray = nullptr);
		AWBXLOADERDLL_API bool LoadAWBXCombinedMesh(const char* IN_AWBXFilePath, unsigned int& OUT_NumVerts, void** OUT_VertexData, void** OUT_TextureData, unsigned int& OUT_NumIndexes, unsigned int** OUT_IndexData);
	};
}
