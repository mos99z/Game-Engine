#pragma once
#include "RenderNode.h"
namespace  RendererD3D
{
	class RenderMesh;

	class RenderShape :
		public RenderNode
	{
		xmfloat4x4 worldMatrix;
		RenderMesh* mesh;
	public:
		RenderShape();
		~RenderShape();
	};

}