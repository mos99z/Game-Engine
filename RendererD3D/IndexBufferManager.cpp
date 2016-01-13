#include "stdafx.h"
#include "IndexBufferManager.h"
#include "Renderer.h"
namespace RendererD3D
{
	IndexBufferManager*  IndexBufferManager::instancePtr = nullptr;
	ID3D11Buffer* IndexBufferManager::indexBufferPtr = nullptr;
	IndexBufferManager::IndexBufferManager()
	{
		//unsigned int cubeIndices[] =
		//{
		//	0, 2, 1, // -x
		//	1, 2, 3,

		//	4, 5, 6, // +x
		//	5, 7, 6,

		//	0, 1, 5, // -y
		//	0, 5, 4,

		//	2, 6, 7, // +y
		//	2, 7, 3,

		//	0, 4, 6, // -z
		//	0, 6, 2,

		//	1, 3, 7, // +z
		//	1, 7, 5,
		//};
		//const UINT indexBufferSize = sizeof(cubeIndices);
		//D3D11_BUFFER_DESC bufferDesc;
		//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		//bufferDesc.ByteWidth = indexBufferSize;
		//bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//bufferDesc.CPUAccessFlags = 0;
		//bufferDesc.MiscFlags = 0;
		//D3D11_SUBRESOURCE_DATA InitData;
		//InitData.pSysMem = cubeIndices;
		//InitData.SysMemPitch = 0;
		//InitData.SysMemSlicePitch = 0;
		//Renderer::theDevicePtr->CreateBuffer(&bufferDesc, &InitData, &indexBufferPtr);
	}


	IndexBufferManager& IndexBufferManager::GetRef()
	{
		if (!instancePtr)
		{
			instancePtr = new IndexBufferManager;
		}
		return *instancePtr;
	}
	void IndexBufferManager::DeleteInstance()
	{
		ReleaseCOM(indexBufferPtr);
		delete instancePtr;
		instancePtr = nullptr;
	}

	UINT IndexBufferManager::AddIndices(const UINT *_indices, UINT _numIndices)
	{
		UINT ret = 0;
		D3D11_BUFFER_DESC inputBuffer_DESC;
		ZeroMemory(&inputBuffer_DESC, sizeof(D3D11_BUFFER_DESC));
		D3D11_SUBRESOURCE_DATA indexBufferData;
		ZeroMemory(&indexBufferData, sizeof(indexBufferData));
		if (indexBufferPtr)
		{
			indexBufferPtr->GetDesc(&inputBuffer_DESC);
			unsigned int oldBufferSize = inputBuffer_DESC.ByteWidth;
			inputBuffer_DESC.ByteWidth += _numIndices* sizeof(unsigned int);
			indexBufferData.pSysMem = new unsigned int[inputBuffer_DESC.ByteWidth];
			memcpy((char *)(indexBufferData.pSysMem) + oldBufferSize,
				_indices, sizeof(unsigned int) * _numIndices);
			ID3D11Buffer* tempIndexBufferPtr = nullptr;
			Renderer::theDevicePtr->CreateBuffer(&inputBuffer_DESC, &indexBufferData, &tempIndexBufferPtr);
			Renderer::theContextPtr->CopySubresourceRegion(tempIndexBufferPtr, 0, 0, 0, 0, indexBufferPtr, 0, 0);
			ReleaseCOM(indexBufferPtr);
			indexBufferPtr = tempIndexBufferPtr;
			ret = oldBufferSize / sizeof(unsigned int);
			delete[] indexBufferData.pSysMem;
		}
		else
		{
			inputBuffer_DESC.Usage = D3D11_USAGE_DEFAULT;
			inputBuffer_DESC.BindFlags = D3D11_BIND_INDEX_BUFFER;
			inputBuffer_DESC.ByteWidth = _numIndices* sizeof(unsigned int);
			indexBufferData.pSysMem = _indices;
			Renderer::theDevicePtr->CreateBuffer(&inputBuffer_DESC, &indexBufferData, &indexBufferPtr);
		}
		return ret;
	}

}
