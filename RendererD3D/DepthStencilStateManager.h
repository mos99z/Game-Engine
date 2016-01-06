#pragma once

namespace  RendererD3D
{
	class DepthStencilStateManager
	{
	public:
		enum DSStates {
			DSS_Default = 0, DSS_LessEqual, DSS_NoDepth,DSS_COUNT
		};
		CComPtr<ID3D11DepthStencilState > dsStates[DSS_COUNT];
	private:
		
	public:
		DepthStencilStateManager();
		~DepthStencilStateManager();
	};
}
