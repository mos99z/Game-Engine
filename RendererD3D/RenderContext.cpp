#include "stdafx.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "RasterizerStateManager.h"
#include "DepthStencilStateManager.h"
namespace RendererD3D
{

	RenderContext::RenderContext()
	{
		RenderFunc = RenderContext::Draw;
	}


	RenderContext::~RenderContext()
	{

	}

	void RenderContext::Draw(RenderNode &node)
	{
		RenderContext& nodeContext = (RenderContext&)node;
		Renderer::theContextPtr->VSSetShader(ShaderManager::GetVertexShaders()[ShaderManager::DEFAULT_VS], 0, 0);
		Renderer::theContextPtr->PSSetShader(ShaderManager::GetPixelShaders()[ShaderManager::DEFAULT_PS], 0, 0);
		Renderer::theContextPtr->OMSetDepthStencilState(DepthStencilStateManager::GetRef().dsStates[DepthStencilStateManager::DSS_Default], 0);
		Renderer::theContextPtr->RSSetState(RasterizerStateManager::GetRef().rasterStates[RasterizerStateManager::RS_CCW]);
		Renderer::theContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Renderer::Render(nodeContext.renderSet);
	}
}