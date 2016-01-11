#include "stdafx.h"
#include "IndexBufferManager.h"
#include "Renderer.h"
namespace RendererD3D
{
	IndexBufferManager*  IndexBufferManager::instancePtr = nullptr;

	IndexBufferManager::IndexBufferManager()
	{
		unsigned int cubeIndices[] =
		{
			0, 2, 1, // -x
			1, 2, 3,

			4, 5, 6, // +x
			5, 7, 6,

			0, 1, 5, // -y
			0, 5, 4,

			2, 6, 7, // +y
			2, 7, 3,

			0, 4, 6, // -z
			0, 6, 2,

			1, 3, 7, // +z
			1, 7, 5,
		};
		const UINT indexBufferSize = sizeof(cubeIndices);
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = indexBufferSize;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = cubeIndices;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		Renderer::theDevicePtr->CreateBuffer(&bufferDesc, &InitData, &indexBufferPtr.p);
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
		delete instancePtr;
		instancePtr = nullptr;
	}

}
