#include "stdafx.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "RenderSet.h"
#include "ShaderManager.h"
#include "RasterizerStateManager.h"
#include "DepthStencilStateManager.h"
namespace RendererD3D
{

	RenderContext::RenderContext()
	{
		renderSetPtr = new RenderSet;
	}


	RenderContext::~RenderContext()
	{
		delete renderSetPtr;
	}

	void RenderContext::Draw(RenderNode &node)
	{
		RenderContext& nodeContext = (RenderContext&)node;
		Renderer::theContextPtr->VSSetShader(ShaderManager::GetVertexShaders()[ShaderManager::DEFAULT_VS], 0, 0);
		Renderer::theContextPtr->PSSetShader(ShaderManager::GetPixelShaders()[ShaderManager::DEFAULT_PS], 0, 0);
		Renderer::theContextPtr->OMSetDepthStencilState(DepthStencilStateManager::GetRef().dsStates[DepthStencilStateManager::DSS_NoDepth],0);
		//Renderer::theContextPtr->RSSetState(RasterizerStateManager::GetRef().rasterStates[0]);
		Renderer::theContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Renderer::Render(*nodeContext.renderSetPtr);
	}
}