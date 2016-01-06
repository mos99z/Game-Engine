#pragma once
#include "RenderNode.h"
namespace RendererD3D
{
	class RenderSet;
	class RenderContext :public RenderNode
	{
		ID3D11VertexShader* default_VS = nullptr;
		ID3D11PixelShader* default_PS = nullptr;

	public:
		 RenderSet* renderSetPtr = nullptr;
		RenderContext();
		~RenderContext();
		
		static void Draw(RenderNode &node);
	};

}