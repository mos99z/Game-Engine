#pragma once

#ifdef RENDERMATERIALDLL_EXPORTS
#define  RENDERMATERIALDLL __declspec(dllexport) 
#else
#define  RENDERMATERIALDLL __declspec(dllimport) 
#endif

#include "RenderNode.h"
#include "RenderSet.h"
namespace RendererD3D
{
	class RenderMaterial : public RenderNode
	{
	public:
		RenderSet renderSet;
		RENDERMATERIALDLL RenderMaterial();
		RENDERMATERIALDLL ~RenderMaterial();
		RENDERMATERIALDLL inline RenderSet& GetSet() { return renderSet; }
		static void Draw(RenderNode &node);
	};
}
