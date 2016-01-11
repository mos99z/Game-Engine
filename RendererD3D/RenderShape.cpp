#include "stdafx.h"
#include "RenderShape.h"
#include "RenderMesh.h"
#include "Renderer.h"
namespace RendererD3D
{
	RenderShape::RenderShape()
	{
		RenderFunc = RenderShape::Draw;
	}


	RenderShape::~RenderShape()
	{
	}

	void  RenderShape::Draw(RenderNode &node)
	{
		RenderShape& nodeShape = (RenderShape&)node;
		Renderer::SetPerObjectData(nodeShape.worldMatrix);
		Renderer::theContextPtr->DrawIndexed(nodeShape.numofIndices,nodeShape.startIndex, nodeShape.startVertex);
	}
}