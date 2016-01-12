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

	bool AWBXLoader::LoadAWBXMeshes(char* IN_AWBXFilePath, int& OUT_numMeshes, unsigned int** OUT_NumVerts, void*** OUT_VertexData, unsigned int** OUT_NumIndexes, unsigned int*** OUT_IndexData)
	{
		if (OpenFile(IN_AWBXFilePath))
		{
			if (!ParseFile())
				return false;

			OUT_numMeshes = m_currNumMeshes;
			*OUT_NumVerts = new unsigned int[OUT_numMeshes];
			*OUT_VertexData = new void*[OUT_numMeshes];

			*OUT_NumIndexes = new unsigned int[OUT_numMeshes];
			*OUT_IndexData = new unsigned int*[OUT_numMeshes];

			for (int mesh = 0; mesh < OUT_numMeshes; mesh++)
			{
				(*OUT_NumVerts)[mesh] = m_VertexSizes[mesh];
				(*OUT_VertexData)[mesh] = new VBuffer[m_VertexSizes[mesh]];
				memcpy_s((*OUT_VertexData)[mesh], m_VertexSizes[mesh] * sizeof(VBuffer), m_VertexData[mesh], m_VertexSizes[mesh] * sizeof(VBuffer));

				(*OUT_NumIndexes)[mesh] = m_IndexSizes[mesh];
				(*OUT_IndexData)[mesh] = new unsigned int[m_IndexSizes[mesh]];
				memcpy_s((*OUT_IndexData)[mesh], m_IndexSizes[mesh] * sizeof(unsigned int), m_IndexData[mesh], m_IndexSizes[mesh] * sizeof(unsigned int));
			}

		}

		return true;
	}

	bool AWBXLoader::LoadAWBXMesh(char* IN_AWBXFilePath, unsigned int& OUT_NumVerts, void** OUT_VertexData, unsigned int& OUT_NumIndexes, unsigned int** OUT_IndexData)
	{
		m_totalVertexes = 0;
		m_totalIndexes = 0;

		if (OpenFile(IN_AWBXFilePath))
		{
			if (!ParseFile())
				return false;

			OUT_NumVerts = m_totalVertexes;
			*OUT_VertexData = new VBuffer[OUT_NumVerts];
			OUT_NumIndexes = m_totalIndexes;
			*OUT_IndexData = new unsigned int[OUT_NumIndexes];

			unsigned int vertOffset = 0;
			unsigned int indexOffset = 0;
			for (int mesh = 0; mesh < m_currNumMeshes; mesh++)
			{
	
				memcpy_s((char*)(*OUT_VertexData) + vertOffset, m_VertexSizes[mesh] * sizeof(VBuffer), (char*)m_VertexData[mesh], m_VertexSizes[mesh] * sizeof(VBuffer));
				vertOffset += m_VertexSizes[mesh] * sizeof(VBuffer);
				memcpy_s((*OUT_IndexData) + indexOffset, m_IndexSizes[mesh] * sizeof(unsigned int), m_IndexData[mesh], (int)m_IndexSizes[mesh] * sizeof(unsigned int));
				indexOffset += m_IndexSizes[mesh];
			}
		}
		else
			return false;

		return true;
	}

	bool AWBXLoader::OpenFile(char* _filePath)
	{
		ReadStream.open(_filePath, std::ios_base::binary);
		m_currNumMeshes = 0;

		if (ReadStream.is_open())
		{
			char checkHeader[5];
			ReadStream.read(checkHeader, 4);
			checkHeader[4] = '\0';
			if (strcmp(checkHeader, "AWBX"))
			{
#if _DEBUG
				std::wstring message = L"AWBXLoader Following file is NOT an AWBX: ";
				size_t errorMessageLength = strlen(_filePath) + 1;
				wchar_t* errorMessage = new wchar_t[errorMessageLength];
				mbstowcs(errorMessage, _filePath, errorMessageLength);
				message.append(errorMessage);
				_ASSERT_EXPR(0, message.c_str());
				delete[] errorMessage;
#endif
				return false;
			}
		}
		else
		{
#if _DEBUG
			std::wstring message = L"AWBXLoader Failed to open file: ";
			size_t errorMessageLength = strlen(_filePath) + 1;
			wchar_t* errorMessage = new wchar_t[errorMessageLength];
			mbstowcs(errorMessage, _filePath, errorMessageLength);
			message.append(errorMessage);
			_ASSERT_EXPR(0, message.c_str());
			delete[] errorMessage;
#endif
			return false;
		}

		return true;
	}

	bool AWBXLoader::ParseFile()
	{
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
				m_currNumMeshes++;
				HandleMeshHeader();
				break;
			}
			default:
				break;
			}
		}

		ReadStream.close();

#pragma region Error Checking

		if (m_currNumMeshes == 0)
		{
#if _DEBUG
			_ASSERT_EXPR(0, L"Number of meshes == 0");
#endif
			return false;
		}

		if (m_IndexData.size() != m_currNumMeshes)
		{
#if _DEBUG
			_ASSERT_EXPR(0, L"Number of Index Buffers does NOT match number of meshes");
#endif
			return false;
		}

		if (m_IndexSizes.size() != m_currNumMeshes)
		{
#if _DEBUG
			_ASSERT_EXPR(0, L"Number of Index Sizes does NOT match number of meshes");
#endif
			return false;
		}

		if (m_VertexData.size() != m_currNumMeshes)
		{
#if _DEBUG
			_ASSERT_EXPR(0, L"Number of Vertex Buffers does NOT match number of meshes");
#endif
			return false;
		}

		if (m_VertexSizes.size() != m_currNumMeshes)
		{
#if _DEBUG
			_ASSERT_EXPR(0, L"Number of Vertex Sizes does NOT match number of meshes");
#endif
			return false;
		}

#pragma endregion

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
		m_totalVertexes += numVerts;
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
		m_totalIndexes += numIndexes;
		unsigned int* IndexData = new unsigned int[numIndexes];
		ReadStream.read((char*)IndexData, IndexDataSize);

		m_IndexSizes.push_back(numIndexes);
		m_IndexData.push_back(IndexData);
	}
}