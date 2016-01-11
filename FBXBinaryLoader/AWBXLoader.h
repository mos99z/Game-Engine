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

		std::vector<int32_t> m_VertexSizes;
		std::vector<VBuffer*> m_VertexData;

		std::vector<int32_t> m_IndexSizes;
		std::vector<unsigned int*> m_IndexData;

		void HandleMeshHeader();
		void HandleVertexBuffer();
		void HandleIndexBuffer();

	public:
		AWBXLOADERDLL_API AWBXLoader();
		AWBXLOADERDLL_API ~AWBXLoader();

		/// Out_VertexData Layout:
		///		float Position[3]
		///		float Normal[3]
		///		float Diffuse[4]
		AWBXLOADERDLL_API bool LoadAWBXMeshes(char* IN_AWBXFilePath, int& OUT_numMeshes, unsigned int** OUT_MeshVertexSizes, void*** OUT_VertexData, unsigned int** OUT_MeshIndexSizes, unsigned int*** OUT_IndexData);
	};
}
