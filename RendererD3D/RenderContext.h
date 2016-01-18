#pragma once
#ifdef RENDERCONTEXTDLL_EXPORTS
#define  RENDERCONTEXTDLL __declspec(dllexport) 
#else
#define  RENDERCONTEXTDLL __declspec(dllimport) 
#endif
#include "RenderNode.h"
#include "RenderSet.h"
namespace RendererD3D
{
	class RenderContext :public RenderNode
	{
		static bool wireFrame;
	public:
		RenderSet renderSet;
		RENDERCONTEXTDLL RenderContext();
		RENDERCONTEXTDLL ~RenderContext();
		RENDERCONTEXTDLL inline RenderSet& GetSet() { return renderSet; }

		static void Draw(RenderNode &node);
		RENDERCONTEXTDLL static void ToggleWireFrame();
	};

}