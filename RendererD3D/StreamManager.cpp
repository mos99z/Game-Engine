#include "stdafx.h"
#include "StreamManager.h"
#include "RenderShape.h"
#include "RenderMesh.h"
#include "IndexBufferManager.h"
#include <AWBXLoader.h>
namespace RendererD3D
{
	StreamManager*  StreamManager::instancePtr = nullptr;
	ID3D11Buffer* StreamManager::GstreamBufferPtr = nullptr;
	ID3D11Buffer* StreamManager::TstreamBufferPtr = nullptr;
	ID3D11Buffer* StreamManager::AstreamBufferPtr = nullptr;
	std::unordered_map<std::string, Model> StreamManager::AllModels;


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
		ReleaseCOM(AstreamBufferPtr);
		delete instancePtr;
		instancePtr = nullptr;
	}

	StreamManager::StreamManager()
	{
	}


	StreamManager::~StreamManager()
	{


	}


	unsigned int StreamManager::AddGstream(const Gstream* _RawGBufferPtr, unsigned int& _numofVertices)
	{
		UINT startVertex = 0;
		D3D11_BUFFER_DESC vertexBuffer_DESC;
		ZeroMemory(&vertexBuffer_DESC, sizeof(D3D11_BUFFER_DESC));
		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		if (GstreamBufferPtr)
		{
			GstreamBufferPtr->GetDesc(&vertexBuffer_DESC);
			unsigned int oldBufferSize = vertexBuffer_DESC.ByteWidth;
			vertexBuffer_DESC.ByteWidth += _numofVertices * sizeof(Gstream);
			vertexBufferData.pSysMem = new Gstream[vertexBuffer_DESC.ByteWidth];
			memcpy((char *)(vertexBufferData.pSysMem) + oldBufferSize,
				_RawGBufferPtr, sizeof(Gstream) * _numofVertices);
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
			vertexBuffer_DESC.ByteWidth = _numofVertices* sizeof(Gstream);
			vertexBufferData.pSysMem = _RawGBufferPtr;
			Renderer::theDevicePtr->CreateBuffer(&vertexBuffer_DESC, &vertexBufferData, &GstreamBufferPtr);
		}
		return startVertex;
	}


	void StreamManager::AddTstream(const Tstream* _RawTBufferPtr, unsigned int& _numofVertices)
	{

	
		D3D11_BUFFER_DESC vertexBuffer_DESC;
		ZeroMemory(&vertexBuffer_DESC, sizeof(D3D11_BUFFER_DESC));
		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		if (TstreamBufferPtr)
		{
			TstreamBufferPtr->GetDesc(&vertexBuffer_DESC);
			unsigned int oldBufferSize = vertexBuffer_DESC.ByteWidth;
			vertexBuffer_DESC.ByteWidth += _numofVertices* sizeof(Tstream);
			vertexBufferData.pSysMem = new Tstream[vertexBuffer_DESC.ByteWidth];
			memcpy((char *)(vertexBufferData.pSysMem) + oldBufferSize,
				_RawTBufferPtr, sizeof(Tstream) * _numofVertices);
			ID3D11Buffer* tempVertexBufferPtr = nullptr;
			Renderer::theDevicePtr->CreateBuffer(&vertexBuffer_DESC, &vertexBufferData, &tempVertexBufferPtr);
			Renderer::theContextPtr->CopySubresourceRegion(tempVertexBufferPtr, 0, 0, 0, 0, TstreamBufferPtr, 0, 0);
			ReleaseCOM(TstreamBufferPtr);
			TstreamBufferPtr = tempVertexBufferPtr;
			delete[] vertexBufferData.pSysMem;
		}
		else
		{
			vertexBuffer_DESC.Usage = D3D11_USAGE_DEFAULT;
			vertexBuffer_DESC.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBuffer_DESC.ByteWidth = _numofVertices* sizeof(Tstream);
			vertexBufferData.pSysMem = _RawTBufferPtr;
			Renderer::theDevicePtr->CreateBuffer(&vertexBuffer_DESC, &vertexBufferData, &TstreamBufferPtr);
		}
		
	}

	void StreamManager::AddAstream(const Astream* _RawABufferPtr, unsigned int& _numofVertices)
	{
		
		D3D11_BUFFER_DESC vertexBuffer_DESC;
		ZeroMemory(&vertexBuffer_DESC, sizeof(D3D11_BUFFER_DESC));
		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		if (AstreamBufferPtr)
		{
			AstreamBufferPtr->GetDesc(&vertexBuffer_DESC);
			unsigned int oldBufferSize = vertexBuffer_DESC.ByteWidth;
			vertexBuffer_DESC.ByteWidth += _numofVertices* sizeof(Astream);
			vertexBufferData.pSysMem = new Astream[vertexBuffer_DESC.ByteWidth];
			memcpy((char *)(vertexBufferData.pSysMem) + oldBufferSize,
				_RawABufferPtr, sizeof(Astream) * _numofVertices);
			ID3D11Buffer* tempVertexBufferPtr = nullptr;
			Renderer::theDevicePtr->CreateBuffer(&vertexBuffer_DESC, &vertexBufferData, &tempVertexBufferPtr);
			Renderer::theContextPtr->CopySubresourceRegion(tempVertexBufferPtr, 0, 0, 0, 0, AstreamBufferPtr, 0, 0);
			ReleaseCOM(AstreamBufferPtr);
			AstreamBufferPtr = tempVertexBufferPtr;
			delete[] vertexBufferData.pSysMem;
		}
		else
		{
			vertexBuffer_DESC.Usage = D3D11_USAGE_DEFAULT;
			vertexBuffer_DESC.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBuffer_DESC.ByteWidth = _numofVertices* sizeof(Astream);
			vertexBufferData.pSysMem = _RawABufferPtr;
			Renderer::theDevicePtr->CreateBuffer(&vertexBuffer_DESC, &vertexBufferData, &AstreamBufferPtr);
		}
		
	}

	void StreamManager::AddStream(std::string& _filename, RenderMesh& nodeMesh)
	{
		if (AllModels.count(_filename))
		{
			nodeMesh.SetNumberOfIndices(AllModels[_filename].numOfIndices);
			nodeMesh.SetNumberOfVertices(AllModels[_filename].numOfVertices);
			nodeMesh.SetStartIndex(AllModels[_filename].stratIndex);
			nodeMesh.SetStartVertex(AllModels[_filename].stratVertex);
			return;
		}


		AWBX::AWBXLoader loaderTest;


		unsigned int* indices = nullptr;
		Gstream* RawGBufferPtr = nullptr;
		Tstream* RawTBufferPtr = nullptr;
		Astream* RawABufferPtr = nullptr;
		unsigned int numofVertices = 0;
		unsigned int numofIndices = 0;

		loaderTest.LoadAWBXCombinedMesh(("..\\..\\Assets\\FBXs\\" + _filename).c_str(), numofVertices, (void**)&RawGBufferPtr, (void**)&RawTBufferPtr, (void*&)RawABufferPtr, numofIndices, &indices, );

		UINT startVertex = AddGstream(RawGBufferPtr, numofVertices);
		AddTstream(RawTBufferPtr, numofVertices);

		nodeMesh.SetNumberOfVertices(numofVertices);
		nodeMesh.SetNumberOfIndices(numofIndices);
		nodeMesh.SetStartVertex(startVertex);
		nodeMesh.SetStartIndex(IndexBufferManager::GetRef().AddIndices(indices, nodeMesh.NumberOfIndices()));

		AllModels[_filename].numOfIndices = nodeMesh.NumberOfIndices();
		AllModels[_filename].numOfVertices = nodeMesh.NumberOfVertices();
		AllModels[_filename].stratIndex = nodeMesh.StartIndex();
		AllModels[_filename].stratVertex = nodeMesh.StartVertex();



		delete[] RawGBufferPtr;
		delete[] RawTBufferPtr;
		delete[] RawABufferPtr;
		delete[] indices;

	}


}