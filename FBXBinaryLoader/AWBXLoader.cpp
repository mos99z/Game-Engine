#define _CRT_SECURE_NO_WARNINGS
#define AWBXLOADERDLL_EXPORTS
#define MATRIXSIZE 16

#include "AWBXLoader.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace AWBX
{
	std::ifstream ReadStream;

	AWBXLoader::AWBXLoader()
	{
		m_UIntSize = sizeof(unsigned int);
		m_DoubleSize = sizeof(double);
		m_VBufferSize = sizeof(VBuffer);
		m_TBufferSize = sizeof(TBuffer);
		m_ABufferSize = sizeof(ABuffer);
		m_FloatSize = sizeof(float);
		m_BoneSize = sizeof(Bone);
		m_MatrixByteSize = MATRIXSIZE * m_DoubleSize;
	}

	AWBXLoader::~AWBXLoader()
	{
	}

	void AWBXLoader::DeleteAllocatedMemory()
	{
		size_t numAllocatedVertBuffers = m_VertexData.size();
		for (size_t vertDataBuffer = 0; vertDataBuffer < numAllocatedVertBuffers; vertDataBuffer++)
		{
			delete[] m_VertexData[vertDataBuffer];
		}

		size_t numAllocatedIndexBuffers = m_IndexData.size();
		for (size_t IndexDataBuffer = 0; IndexDataBuffer < numAllocatedIndexBuffers; IndexDataBuffer++)
		{
			delete[] m_IndexData[IndexDataBuffer];
		}

		size_t numAllocatedMatrixBuffers = m_Matricies.size();
		for (size_t MatrixDataBuffer = 0; MatrixDataBuffer < numAllocatedMatrixBuffers; MatrixDataBuffer++)
		{
			delete[] m_Matricies[MatrixDataBuffer];
		}

		size_t numAllocatedTextureBuffers = m_TextureData.size();
		for (size_t TextureDataBuffer = 0; TextureDataBuffer < numAllocatedTextureBuffers; TextureDataBuffer++)
		{
			delete[] m_TextureData[TextureDataBuffer];
		}
	}

	bool AWBXLoader::LoadAWBXMeshes(const char* IN_AWBXFilePath, int& OUT_numMeshes, unsigned int** OUT_NumVerts, void*** OUT_VertexData, void*** OUT_TextureData, unsigned int** OUT_NumIndexes, unsigned int*** OUT_IndexData, double*** OUT_matrixArray)
	{
		m_SingleMesh = false;

		if (OpenFile(IN_AWBXFilePath))
		{
			if (!ParseFile())
			{
				DeleteAllocatedMemory();
				return false;
			}

			OUT_numMeshes = m_currNumMeshes;
			*OUT_NumVerts = new unsigned int[OUT_numMeshes];
			*OUT_VertexData = new void*[OUT_numMeshes];
			*OUT_TextureData = new void*[OUT_numMeshes];
			*OUT_NumIndexes = new unsigned int[OUT_numMeshes];
			*OUT_IndexData = new unsigned int*[OUT_numMeshes];

			if (OUT_matrixArray)
				*OUT_matrixArray = new double*[OUT_numMeshes];

			unsigned int vertexByteSize;
			unsigned int textureByteSize;
			unsigned int indexByteSize;
			for (int mesh = 0; mesh < OUT_numMeshes; mesh++)
			{
				(*OUT_NumVerts)[mesh] = m_VertexSizes[mesh];
				(*OUT_VertexData)[mesh] = new VBuffer[m_VertexSizes[mesh]];
				vertexByteSize = m_VertexSizes[mesh] * m_VBufferSize;
				memcpy_s((*OUT_VertexData)[mesh], vertexByteSize, m_VertexData[mesh], vertexByteSize);

				(*OUT_TextureData)[mesh] = new TBuffer[m_VertexSizes[mesh]];
				textureByteSize = m_VertexSizes[mesh] * m_TBufferSize;
				memcpy_s((*OUT_TextureData)[mesh], textureByteSize, m_TextureData[mesh], textureByteSize);

				(*OUT_NumIndexes)[mesh] = m_IndexSizes[mesh];
				(*OUT_IndexData)[mesh] = new unsigned int[m_IndexSizes[mesh]];
				indexByteSize = m_IndexSizes[mesh] * m_UIntSize;
				memcpy_s((*OUT_IndexData)[mesh], indexByteSize, m_IndexData[mesh], indexByteSize);

				if (OUT_matrixArray)
				{
					(*OUT_matrixArray)[mesh] = new double[MATRIXSIZE];
					memcpy_s((*OUT_matrixArray)[mesh], m_MatrixByteSize, m_Matricies[mesh], m_MatrixByteSize);
				}
				else
				{
					delete[] m_Matricies[mesh];
				}
			}
		}
		return true;
	}

	bool AWBXLoader::LoadAWBXCombinedMesh(const char* IN_AWBXFilePath,
		unsigned int& OUT_NumVerts, void** OUT_VertexData, void** OUT_TextureData, void*& OUT_WeightData,
		unsigned int& OUT_NumIndexes, unsigned int** OUT_IndexData,
		void*& OUT_AnimationData)
	{
		m_totalVertexes = 0;
		m_totalIndexes = 0;
		m_SingleMesh = true;
		m_LoadedAnimationData = false;

		if (OpenFile(IN_AWBXFilePath))
		{
			if (!ParseFile())
			{
				DeleteAllocatedMemory();
				return false;
			}

			OUT_WeightData = m_WeightData;
			OUT_AnimationData = m_Animation;

			OUT_NumVerts = m_totalVertexes;
			*OUT_VertexData = new VBuffer[OUT_NumVerts];
			*OUT_TextureData = new TBuffer[OUT_NumVerts];
			OUT_NumIndexes = m_totalIndexes;
			*OUT_IndexData = new unsigned int[OUT_NumIndexes];

			unsigned int vertOffset = 0;
			unsigned int indexOffset = 0;
			unsigned int textureOffset = 0;

			unsigned int vertexByteSize;
			unsigned int textureByteSize;
			unsigned int indexByteSize;
			for (int mesh = 0; mesh < m_currNumMeshes; mesh++)
			{

				vertexByteSize = m_VertexSizes[mesh] * m_VBufferSize;
				memcpy_s((char*)(*OUT_VertexData) + vertOffset, vertexByteSize, (char*)m_VertexData[mesh], vertexByteSize);
				vertOffset += m_VertexSizes[mesh] * m_VBufferSize;

				textureByteSize = m_VertexSizes[mesh] * m_TBufferSize;
				memcpy_s((char*)(*OUT_TextureData) + textureOffset, textureByteSize, (char*)m_TextureData[mesh], textureByteSize);
				textureOffset += m_VertexSizes[mesh] * m_TBufferSize;


				indexByteSize = m_IndexSizes[mesh] * m_UIntSize;
				memcpy_s((*OUT_IndexData) + indexOffset, indexByteSize, m_IndexData[mesh], indexByteSize);
				indexOffset += m_IndexSizes[mesh];
			}
		}
		else
			return false;

		return true;
	}

	bool AWBXLoader::OpenFile(const char* _filePath)
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

			//if (!m_SingleMesh)
			//{
			//	double* worldMatrix = new double[MATRIXSIZE];
			//	ReadStream.read((char*)worldMatrix, m_MatrixByteSize);
			//	m_Matricies.push_back(worldMatrix);
			//}
			//else
			//{
			//	double buffer[MATRIXSIZE];
			//	ReadStream.read((char*)buffer, m_MatrixByteSize);
			//}

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
				HandleTextureBuffer();
				break;
			}
			case AWBXLoader::MeshData:
			{
				m_currNumMeshes++;
				HandleMeshHeader();
				break;
			}
			case AWBXLoader::WeightBufer:
			{
				HandleWeightBuffer();
				break;
			}
			case AWBXLoader::AnimationData:
			{
				HandleAnimationData();
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
		/*
		int16_t meshNameSize;
		int8_t* meshName;
		ReadStream.read((char*)&meshNameSize, sizeof(int16_t));
		meshName = new int8_t[meshNameSize];
		ReadStream.read((char*)meshName, meshNameSize);

		//temp
		delete[] meshName;
		*/

		if (!m_SingleMesh)
		{
			double* localMatrix = new double[MATRIXSIZE];
			ReadStream.read((char*)localMatrix, m_MatrixByteSize);
			m_Matricies.push_back(localMatrix);
		}
		else
		{
			double buffer[MATRIXSIZE];
			ReadStream.read((char*)buffer, m_MatrixByteSize);
		}
	}

	void AWBXLoader::HandleVertexBuffer()
	{
		unsigned int vertDataSize;
		ReadStream.read((char*)&vertDataSize, m_UIntSize);
		unsigned int numVerts = vertDataSize / m_VBufferSize;
		m_totalVertexes += numVerts;
		VBuffer* vertexData = new VBuffer[numVerts];
		ReadStream.read((char*)vertexData, vertDataSize);

		m_VertexSizes.push_back(numVerts);
		m_VertexData.push_back(vertexData);
	}

	void AWBXLoader::HandleIndexBuffer()
	{
		unsigned int IndexDataSize;
		ReadStream.read((char*)&IndexDataSize, m_UIntSize);
		unsigned int numIndexes = IndexDataSize / m_UIntSize;
		m_totalIndexes += numIndexes;
		unsigned int* IndexData = new unsigned int[numIndexes];
		ReadStream.read((char*)IndexData, IndexDataSize);

		m_IndexSizes.push_back(numIndexes);
		m_IndexData.push_back(IndexData);
	}

	void AWBXLoader::HandleTextureBuffer()
	{
		unsigned int textureDataByteSize;
		ReadStream.read((char*)&textureDataByteSize, m_UIntSize);
		unsigned int TBufferArraySize = textureDataByteSize / m_TBufferSize;
		TBuffer* textureData = new TBuffer[TBufferArraySize];
		ReadStream.read((char*)textureData, textureDataByteSize);

		m_TextureData.push_back(textureData);
	}

	void AWBXLoader::HandleWeightBuffer()
	{
		unsigned int animationDataByteSize;
		ReadStream.read((char*)&animationDataByteSize, m_UIntSize);
		unsigned int animationArraySize = animationDataByteSize / m_ABufferSize;
		m_WeightData = new ABuffer[animationArraySize];
		ReadStream.read((char*)m_WeightData, animationDataByteSize);
	}

	void AWBXLoader::HandleAnimationData()
	{
		if (m_LoadedAnimationData)
		{
#if _DEBUG
			_ASSERT_EXPR(0, L"More than 1 set of AnimationData Found!");
#endif
			return;
		}

		m_Animation = new AnimationBuffer;
		ReadStream.read((char*)&m_Animation->m_NumKeyFrames, m_UIntSize);
		ReadStream.read((char*)&m_numBones, m_UIntSize);
		m_DM_TimeStamps = new float[m_Animation->m_NumKeyFrames];
		ReadStream.read((char*)m_DM_TimeStamps, m_Animation->m_NumKeyFrames * m_FloatSize);
		m_KeyFramesBoneLists = new Bone[m_Animation->m_NumKeyFrames * m_numBones];
		ReadStream.read((char*)&m_KeyFramesBoneLists, m_BoneSize * m_Animation->m_NumKeyFrames * m_numBones);

		m_Animation->m_KeyFrames = new KeyFrameBuffer[m_Animation->m_NumKeyFrames];

		for (int currKeyFrame = 0; currKeyFrame < m_Animation->m_NumKeyFrames; currKeyFrame++)
		{
			m_Animation->m_KeyFrames[currKeyFrame].m_bones = new Bone[m_numBones];
			m_Animation->m_KeyFrames[currKeyFrame].m_numBones = m_numBones;
			m_Animation->m_KeyFrames[currKeyFrame].m_TimeStamp = m_DM_TimeStamps[currKeyFrame];
			m_Animation->m_KeyFrames[currKeyFrame].m_bones = m_KeyFramesBoneLists + (m_numBones * currKeyFrame);
		}

		delete[] m_DM_TimeStamps;
		m_DM_TimeStamps = nullptr;
		m_LoadedAnimationData = true;
	}

}
