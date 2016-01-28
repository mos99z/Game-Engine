#pragma once

namespace  RendererD3D
{
	class DepthStencilStateManager
	{
	public:
		enum DSStates {
			DSS_Default = 0, DSS_LessEqual, DSS_NoDepth,DSS_DEFERRED, DSS_COUNT
		};
		CComPtr<ID3D11DepthStencilState > dsStates[DSS_COUNT];
	private:
		static DepthStencilStateManager* instancePtr;
		DepthStencilStateManager();
	public:
		static DepthStencilStateManager& GetRef();
		static void DeleteInstance();
	};
}
