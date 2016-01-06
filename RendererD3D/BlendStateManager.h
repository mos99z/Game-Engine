#pragma once
namespace  RendererD3D
{
	class BlendStateManager
	{
	public:
		enum BStates { BS_Default = 0, BS_Alpha, BS_Additive, BS_COUNT };
		CComPtr<ID3D11BlendState > blendStates[BS_COUNT];
	private:
	public:
		BlendStateManager();
		~BlendStateManager();
	};
}
