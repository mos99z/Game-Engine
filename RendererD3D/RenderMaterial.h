#pragma once
#include "RenderNode.h"
namespace RendererD3D
{
	class RenderSet;
	class RenderMaterial : public RenderNode
	{
	public:
		RenderSet* renderSetPtr = nullptr;
		RenderMaterial();
		~RenderMaterial();
		static void Draw(RenderNode &node);
	};
}
