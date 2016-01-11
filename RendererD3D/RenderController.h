#pragma once

#include "RenderSet.h"
namespace  RendererD3D
{
	class RenderController
	{
		RenderSet renderSet;
	public:
		RenderController();
		~RenderController();
		inline RenderSet& GetSet() { return renderSet; }
	};
}

