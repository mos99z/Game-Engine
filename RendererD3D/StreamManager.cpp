#include "stdafx.h"
#include "StreamManager.h"
#include "RenderShape.h"
#include "IndexBufferManager.h"
#include <AWBXLoader.h>
namespace RendererD3D
{
	StreamManager*  StreamManager::instancePtr = nullptr;
	ID3D11Buffer* StreamManager::GstreamBufferPtr = nullptr;
	ID3D11Buffer* StreamManager::TstreamBufferPtr = nullptr;
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
		ReleaseCOM(TstreamBufferPtr);
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

		Gstream* RawGBufferPtr = nullptr;
		Tstream* RawTBufferPtr = nullptr;
		loaderTest.LoadAWBXCombinedMesh(("..\\..\\Assets\\FBXs\\" + _filename).c_str(), renderShape.numofVertices, (void**)&RawGBufferPtr, (void**)&RawTBufferPtr, renderShape.numofIndices, &indices);
		numofGstream += renderShape.numofVertices;
		numofTstream += renderShape.numofVertices;


		UINT startVertex = 0;
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
				RawGBufferPtr, sizeof(Gstream) * renderShape.numofVertices);
			ID3D11Buffer* tempVertexBufferPtr = nullptr;
			Renderer::theDevicePtr->CreateBuffer(&vertexBuffer_DESC, &vertexBufferData, &tempVertexBufferPtr);
			Renderer::theContextPtr->CopySubresourceRegion(tempVertexBufferPtr, 0, 0, 0, 0, GstreamBufferPtr, 0, 0);
			ReleaseCOM(GstreamBufferPtr);
			GstreamBufferPtr = tempVertexBufferPtr;
			startVertex = oldBufferSize / sizeof(Gstream);
			delete[] vertexBufferData.pSysMem;
		}
		else
		{
			vertexBuffer_DESC.Usage = D3D11_USAGE_DEFAULT;
			vertexBuffer_DESC.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBuffer_DESC.ByteWidth = renderShape.numofVertices* sizeof(Gstream);
			vertexBufferData.pSysMem = RawGBufferPtr;
			Renderer::theDevicePtr->CreateBuffer(&vertexBuffer_DESC, &vertexBufferData, &GstreamBufferPtr);
		}

		ZeroMemory(&vertexBuffer_DESC, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		if (TstreamBufferPtr)
		{
			TstreamBufferPtr->GetDesc(&vertexBuffer_DESC);
			unsigned int oldBufferSize = vertexBuffer_DESC.ByteWidth;
			vertexBuffer_DESC.ByteWidth += renderShape.numofVertices* sizeof(Tstream);
			vertexBufferData.pSysMem = new Tstream[vertexBuffer_DESC.ByteWidth];
			memcpy((char *)(vertexBufferData.pSysMem) + oldBufferSize,
				RawTBufferPtr, sizeof(Tstream) * renderShape.numofVertices);
			ID3D11Buffer* tempVertexBufferPtr = nullptr;
			Renderer::theDevicePtr->CreateBuffer(&vertexBuffer_DESC, &vertexBufferData, &tempVertexBufferPtr);
			Renderer::theContextPtr->CopySubresourceRegion(tempVertexBufferPtr, 0, 0, 0, 0, TstreamBufferPtr, 0, 0);
			ReleaseCOM(TstreamBufferPtr);
			TstreamBufferPtr = tempVertexBufferPtr;
			//startVertex = oldBufferSize / sizeof(Tstream);
			delete[] vertexBufferData.pSysMem;
		}
		else
		{
			vertexBuffer_DESC.Usage = D3D11_USAGE_DEFAULT;
			vertexBuffer_DESC.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBuffer_DESC.ByteWidth = renderShape.numofVertices* sizeof(Tstream);
			vertexBufferData.pSysMem = RawTBufferPtr;
			Renderer::theDevicePtr->CreateBuffer(&vertexBuffer_DESC, &vertexBufferData, &TstreamBufferPtr);
		}


		renderShape.startVertex = startVertex;
		delete[] RawGBufferPtr;
		delete[] RawTBufferPtr;



		renderShape.startIndex = IndexBufferManager::GetRef().AddIndices(indices, renderShape.numofIndices);
		delete[] indices;

	}


}