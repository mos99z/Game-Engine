#include "stdafx.h"
#include "Renderer.h"
#include "RenderMaterial.h"
#include "RenderSet.h"
namespace RendererD3D
{
	RenderMaterial::RenderMaterial()
	{
		renderSetPtr = new RenderSet;

	}


	RenderMaterial::~RenderMaterial()
	{
		delete renderSetPtr;
	}

	void RenderMaterial::Draw(RenderNode &node)
	{
		RenderMaterial& nodeMaterial = (RenderMaterial&)node;
		Renderer::Render(*nodeMaterial.renderSetPtr);
	}
}
