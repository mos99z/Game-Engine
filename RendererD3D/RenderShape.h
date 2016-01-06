#pragma once
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
		RenderShape();
		~RenderShape();
		static void Draw(RenderNode &node);
	};

}