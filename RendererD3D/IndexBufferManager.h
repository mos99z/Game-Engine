#pragma once
namespace RendererD3D
{
	class IndexBufferManager
	{
		
	private:
		IndexBufferManager();
		static IndexBufferManager*  instancePtr;
	public:
		static ID3D11Buffer* indexBufferPtr;
		UINT numofIndices = 0;
		static IndexBufferManager& GetRef();
		static void DeleteInstance();
		UINT AddIndices(const UINT *_indices, UINT numIndices);

		inline ID3D11Buffer * const GetIndices() { return indexBufferPtr; }
	};

}