#include "stdafx.h"
#include "Renderer.h"
#include "RenderMaterial.h"
#include "RenderSet.h"
namespace RendererD3D
{
	RenderMaterial::RenderMaterial()
	{
		RenderFunc = RenderMaterial::Draw;
	}


	RenderMaterial::~RenderMaterial()
	{
	}

	void RenderMaterial::Draw(RenderNode &node)
	{
		RenderMaterial& nodeMaterial = (RenderMaterial&)node;
		Renderer::Render(nodeMaterial.renderSet);
	}
}
