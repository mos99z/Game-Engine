#include "stdafx.h"
#include "RenderShape.h"
#include "RenderMesh.h"
#include "Renderer.h"
namespace RendererD3D
{
	RenderShape::RenderShape()
	{
		RenderFunc = RenderShape::DrawIndexed;
	}




	RenderShape::~RenderShape()
	{
	}

	void  RenderShape::DrawIndexed(RenderNode &node)
	{
		RenderShape& nodeShape = (RenderShape&)node;
		RenderMesh& nodeMesh = nodeShape.renderMesh;
		Renderer::SetPerObjectData(nodeShape.worldMatrix);
		Renderer::theContextPtr->DrawIndexed(nodeMesh.NumberOfIndices(), nodeMesh.StartIndex(), nodeMesh.StartVertex());
	}
	 void  RenderShape::GBufferUnpacking(RenderNode &node)
	{
		RenderShape& nodeShape = (RenderShape&)node;
		Renderer::theContextPtr->Draw(4, 0);
	}
}