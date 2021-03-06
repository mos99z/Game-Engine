#pragma once
#include "RenderNode.h"
#include "RenderSet.h"

#ifdef RENDERCONTEXTDLL_EXPORTS
#define  RENDERCONTEXTDLL __declspec(dllexport)
#else
#define  RENDERCONTEXTDLL __declspec(dllimport)
#endif



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
		static void GBufferPacking(RenderNode &node);
		static void GBufferUnpacking(RenderNode &node);
		RENDERCONTEXTDLL static void ToggleWireFrame();
	};

}