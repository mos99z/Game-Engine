#pragma once
#include "RenderNode.h"
namespace RendererD3D
{
	class RenderSet;
	class RenderContext :public RenderNode
	{
	public:
		RenderSet* renderSetPtr = nullptr;
		RenderContext();
		~RenderContext();

		static void Draw(RenderNode &node);
	};

}