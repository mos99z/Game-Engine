#pragma once
#include "RenderNode.h"
namespace RendererD3D
{
	class RenderSet;
	class RenderContext :public RenderNode
	{
		CComPtr<ID3D11VertexShader> default_VS;
		CComPtr<ID3D11PixelShader> default_PS;

	public:
		RenderSet* renderSetPtr = nullptr;
		RenderContext();
		~RenderContext();

		static void Draw(RenderNode &node);
	};

}