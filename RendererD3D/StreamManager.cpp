#include "stdafx.h"
#include "StreamManager.h"
#include "RenderShape.h"
#include "IndexBufferManager.h"
#include <AWBXLoader.h>
namespace RendererD3D
{
	StreamManager*  StreamManager::instancePtr = nullptr;
	ID3D11Buffer* StreamManager::GstreamBufferPtr = nullptr;
	StreamManager& StreamManager::GetRef()
	{
		if (!instancePtr)
		{
			instancePtr = new StreamManager;
		}
		return *instancePtr;
	}

	void StreamManager::DeleteInstance()
	{
		ReleaseCOM(GstreamBufferPtr);
		delete instancePtr;
		instancePtr = nullptr;
	}

	StreamManager::StreamManager()
	{
	}


	StreamManager::~StreamManager()
	{
		
		delete[] TstreamRawBufferPtr;
	}

	void StreamManager::AddGStream(std::string& _filename, RenderShape& renderShape)
	{
		AWBX::AWBXLoader loaderTest;
		unsigned int* indices = nullptr;

		Gstream* RawBufferPtr = nullptr;
		loaderTest.LoadAWBXCombinedMesh(("..\\Assets\\FBXs\\" + _filename).c_str(), renderShape.numofVertices, (void**)&RawBufferPtr, (void**)&TstreamRawBufferPtr, renderShape.numofIndices, &indices);
		numofGstream += renderShape.numofVertices;


		UINT ret = 0;
		D3D11_BUFFER_DESC vertexBuffer_DESC;
		ZeroMemory(&vertexBuffer_DESC, sizeof(D3D11_BUFFER_DESC));
		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		if (GstreamBufferPtr)
		{
			GstreamBufferPtr->GetDesc(&vertexBuffer_DESC);
			unsigned int oldBufferSize = vertexBuffer_DESC.ByteWidth;
			vertexBuffer_DESC.ByteWidth += renderShape.numofVertices* sizeof(Gstream);
			vertexBufferData.pSysMem = new Gstream[vertexBuffer_DESC.ByteWidth];
			memcpy((char *)(vertexBufferData.pSysMem) + oldBufferSize,
				RawBufferPtr, sizeof(Gstream) * renderShape.numofVertices);
			ID3D11Buffer* tempVertexBufferPtr = nullptr;
			Renderer::theDevicePtr->CreateBuffer(&vertexBuffer_DESC, &vertexBufferData, &tempVertexBufferPtr);
			Renderer::theContextPtr->CopySubresourceRegion(tempVertexBufferPtr, 0, 0, 0, 0, GstreamBufferPtr, 0, 0);
			ReleaseCOM(GstreamBufferPtr);
			GstreamBufferPtr = tempVertexBufferPtr;
			ret = oldBufferSize / sizeof(Gstream);
			delete[] vertexBufferData.pSysMem;
		}
		else
		{
			vertexBuffer_DESC.Usage = D3D11_USAGE_DEFAULT;
			vertexBuffer_DESC.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBuffer_DESC.ByteWidth = renderShape.numofVertices* sizeof(Gstream);
			vertexBufferData.pSysMem = RawBufferPtr;
			Renderer::theDevicePtr->CreateBuffer(&vertexBuffer_DESC, &vertexBufferData, &GstreamBufferPtr);
		}
		renderShape.startVertex = ret;
		delete[] RawBufferPtr;




		renderShape.startIndex = IndexBufferManager::GetRef().AddIndices(indices, renderShape.numofIndices);
		delete[] indices;

	}


}