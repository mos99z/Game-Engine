#define _CRT_SECURE_NO_WARNINGS
#define AWBXLOADERDLL_EXPORTS

#include "AWBXLoader.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace AWBX
{
	std::ifstream ReadStream;

	AWBXLoader::AWBXLoader()
	{

	}

	AWBXLoader::~AWBXLoader()
	{

	}

	bool AWBXLoader::LoadAWBXMeshes(char* IN_AWBXFilePath, int& OUT_numMeshes, unsigned int** OUT_MeshVertexSizes, void*** OUT_VertexData, unsigned int** OUT_MeshIndexSizes, unsigned int*** OUT_IndexData)
	{
		ReadStream.open(IN_AWBXFilePath, std::ios_base::binary);

		OUT_numMeshes = 0;

		if (ReadStream.is_open())
		{
			char checkHeader[5];
			ReadStream.read(checkHeader, 4);
			checkHeader[4] = '\0';
			int fileCheckValue = strcmp(checkHeader, "AWBX");
			if (fileCheckValue)
			{
#if _DEBUG
				std::wstring message = L"AWBXLoader Following file is NOT an AWBX: ";
				size_t errorMessageLength = strlen(IN_AWBXFilePath) + 1;
				wchar_t* errorMessage = new wchar_t[errorMessageLength];
				mbstowcs(errorMessage, IN_AWBXFilePath, errorMessageLength);
				message.append(errorMessage);
				_ASSERT_EXPR(0, message.c_str());
				delete[] errorMessage;
#endif
				return false;
			}

			DataType currDataType;

			for (;;)
			{
				ReadStream.read((char*)&currDataType, 1);
				if (ReadStream.eof())
					break;

				switch (currDataType)
				{
				case AWBXLoader::VertexBuffer:
				{
					HandleVertexBuffer();
					break;
				}
				case AWBXLoader::IndexBuffer:
				{
					HandleIndexBuffer();
					break;
				}
				case AWBXLoader::Texture:
				{

					break;
				}
				case AWBXLoader::MeshData:
				{
					OUT_numMeshes++;
					HandleMeshHeader();
					break;
				}
				default:
					break;
				}
			}

			ReadStream.close();

#pragma region Error Checking

			if (OUT_numMeshes == 0)
			{
#if _DEBUG
				_ASSERT_EXPR(0, L"Number of meshes == 0");
#endif
				return false;
			}

			if (m_IndexData.size() != OUT_numMeshes)
			{
#if _DEBUG
				_ASSERT_EXPR(0, L"Number of Index Buffers does NOT match number of meshes");
#endif
				return false;
			}

			if (m_IndexSizes.size() != OUT_numMeshes)
			{
#if _DEBUG
				_ASSERT_EXPR(0, L"Number of Index Sizes does NOT match number of meshes");
#endif
				return false;
			}

			if (m_VertexData.size() != OUT_numMeshes)
			{
#if _DEBUG
				_ASSERT_EXPR(0, L"Number of Vertex Buffers does NOT match number of meshes");
#endif
				return false;
			}

			if (m_VertexSizes.size() != OUT_numMeshes)
			{
#if _DEBUG
				_ASSERT_EXPR(0, L"Number of Vertex Sizes does NOT match number of meshes");
#endif
				return false;
			}

#pragma endregion

			*OUT_MeshVertexSizes = new unsigned int[OUT_numMeshes];
			*OUT_VertexData = new void*[OUT_numMeshes];

			*OUT_MeshIndexSizes = new unsigned int[OUT_numMeshes];
			*OUT_IndexData = new unsigned int*[OUT_numMeshes];

			for (int mesh = 0; mesh < OUT_numMeshes; mesh++)
			{
				(*OUT_MeshVertexSizes)[mesh] = (int)m_VertexSizes[mesh];
				(*OUT_VertexData)[mesh] = new VBuffer[m_VertexSizes[mesh]];
				memcpy_s((*OUT_VertexData)[mesh], m_VertexSizes[mesh] * sizeof(VBuffer), m_VertexData[mesh], m_VertexSizes[mesh] * sizeof(VBuffer));

				(*OUT_MeshIndexSizes)[mesh] = (int)m_IndexSizes[mesh];
				(*OUT_IndexData)[mesh] = new unsigned int[(int)m_IndexSizes[mesh]];
				memcpy_s((*OUT_IndexData)[mesh], (int)m_IndexSizes[mesh] * sizeof(unsigned int), m_IndexData[mesh], (int)m_IndexSizes[mesh] * sizeof(unsigned int));
			}

		}
		else
		{
#if _DEBUG
			std::wstring message = L"AWBXLoader Failed to open file: ";
			size_t errorMessageLength = strlen(IN_AWBXFilePath) + 1;
			wchar_t* errorMessage = new wchar_t[errorMessageLength];
			mbstowcs(errorMessage, IN_AWBXFilePath, errorMessageLength);
			message.append(errorMessage);
			_ASSERT_EXPR(0, message.c_str());
			delete[] errorMessage;
#endif
			return false;
		}

		return true;
	}

	void AWBXLoader::HandleMeshHeader()
	{
		int16_t meshNameSize;
		int8_t* meshName;

		ReadStream.read((char*)&meshNameSize, sizeof(int16_t));
		meshName = new int8_t[meshNameSize];
		ReadStream.read((char*)meshName, meshNameSize);
	}

	void AWBXLoader::HandleVertexBuffer()
	{
		unsigned int vertDataSize;
		ReadStream.read((char*)&vertDataSize, sizeof(unsigned int));
		unsigned int numVerts = vertDataSize / sizeof(VBuffer);
		VBuffer* vertexData = new VBuffer[numVerts];
		ReadStream.read((char*)vertexData, vertDataSize);

		m_VertexSizes.push_back(numVerts);
		m_VertexData.push_back(vertexData);
	}

	void AWBXLoader::HandleIndexBuffer()
	{
		unsigned int IndexDataSize;
		ReadStream.read((char*)&IndexDataSize, sizeof(unsigned int));
		unsigned int numIndexes = IndexDataSize / sizeof(unsigned int);
		unsigned int* IndexData = new unsigned int[numIndexes];
		ReadStream.read((char*)IndexData, IndexDataSize);

		m_IndexSizes.push_back(numIndexes);
		m_IndexData.push_back(IndexData);
	}
}