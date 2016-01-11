#pragma once

#ifdef RENDERSHAPEDLL_EXPORTS
#define  RENDERSHAPEDLL __declspec(dllexport) 
#else
#define  RENDERSHAPEDLL __declspec(dllimport) 
#endif

#include "RenderNode.h"
namespace  RendererD3D
{
	class RenderMesh;

	class RenderShape :
		public RenderNode
	{
		
	public:
		float4x4 worldMatrix;
		RenderMesh* mesh;
		UINT numofVertices = 0;
		UINT startVertex = 0;
		UINT startIndex = 0;
		UINT numofIndices = 0;
		RENDERSHAPEDLL RenderShape();
		RENDERSHAPEDLL ~RenderShape();
	
		static void Draw(RenderNode &node);
	};

}