#pragma once
namespace  RendererD3D
{
	class RasterizerStateManager
	{
	public:
		enum RasterStates { RS_Default = 0, RS_LINE, RS_CCW, RS_NOCULL, RS_COUNT };
		CComPtr<ID3D11RasterizerState> rasterStates[RS_COUNT];
	private:
		RasterStates currentState;
	public:
		RasterizerStateManager();
		~RasterizerStateManager();

		static RasterizerStateManager& GetRef();
		
	};
}
