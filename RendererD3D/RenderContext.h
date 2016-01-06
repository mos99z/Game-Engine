#pragma once
#include "RenderNode.h"
namespace RendererD3D
{
	class RenderContext :public RenderNode
	{
		ID3D11VertexShader* default_VS = nullptr;
		ID3D11PixelShader* default_PS = nullptr;

	public:
		RenderContext();
		~RenderContext();
		static void Draw(RenderNode &node);
	};

}