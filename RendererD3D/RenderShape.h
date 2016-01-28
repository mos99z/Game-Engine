#pragma once

#ifdef RENDERSHAPEDLL_EXPORTS
#define  RENDERSHAPEDLL __declspec(dllexport) 
#else
#define  RENDERSHAPEDLL __declspec(dllimport) 
#endif

#include "RenderNode.h"
#include "RenderMesh.h"
namespace  RendererD3D
{

	class RenderShape :
		public RenderNode
	{
		
	public:
		float4x4 worldMatrix;
		RenderMesh renderMesh;
		RENDERSHAPEDLL RenderShape();
		RENDERSHAPEDLL ~RenderShape();
	
		static void DrawIndexed(RenderNode &node);
		static void GBufferUnpacking(RenderNode &node);
	};

}