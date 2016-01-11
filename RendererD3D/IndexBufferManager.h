#pragma once
namespace RendererD3D
{
	class IndexBufferManager
	{
		
	private:
		IndexBufferManager();
		static IndexBufferManager*  instancePtr;
	public:
		CComPtr<ID3D11Buffer> indexBufferPtr;
		static IndexBufferManager& GetRef();
		static void DeleteInstance();
	};

}