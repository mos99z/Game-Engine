#include "stdafx.h"
#include "RenderShape.h"
#include "RenderMesh.h"
#include "Renderer.h"
namespace RendererD3D
{
	RenderShape::RenderShape()
	{
		RenderFunc = &Draw;
	}


	RenderShape::~RenderShape()
	{
	}

	void  RenderShape::Draw(RenderNode &node)
	{
		RenderShape& nodeShape = (RenderShape&)node;
		Renderer::SetPerObjectData(nodeShape.worldMatrix);
	}
}