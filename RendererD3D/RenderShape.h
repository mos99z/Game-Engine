#pragma once
#include "RenderNode.h"
namespace  RendererD3D
{
	class RenderMesh;

	class RenderShape :
		public RenderNode
	{
		float4x4 worldMatrix;
		RenderMesh* mesh;
	public:
		RenderShape();
		~RenderShape();
		static void Draw(RenderNode &node);
	};

}