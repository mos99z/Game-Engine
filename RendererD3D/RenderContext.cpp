#include "stdafx.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "RasterizerStateManager.h"
#include "InputLayoutManager.h"
#include "IndexBufferManager.h"
#include "BlendStateManager.h"
#include "StreamManager.h"
#include "DepthStencilStateManager.h"
namespace RendererD3D
{
	bool RenderContext::wireFrame = false;
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
		Renderer::theContextPtr->OMSetRenderTargets(1, &Renderer::theRenderTargetViewPtr, Renderer::theDepthStencilViewPtr);
		//Set VertexBuffer
		UINT stripe[3] = { sizeof(Gstream),sizeof(Tstream),sizeof(Astream) };
		UINT offset[3] = { 0,0,0 };
		ID3D11Buffer* buffers[3] = { StreamManager::GetRef().GstreamBufferPtr ,StreamManager::GetRef().TstreamBufferPtr,StreamManager::GetRef().AstreamBufferPtr };
		Renderer::theContextPtr->IASetVertexBuffers(0, 3, buffers, stripe, offset);
		//Set IndexBuffer 
		Renderer::theContextPtr->IASetIndexBuffer(IndexBufferManager::GetRef().indexBufferPtr, DXGI_FORMAT_R32_UINT, 0);
		//Set InputLayout
		Renderer::theContextPtr->IASetInputLayout(InputLayoutManager::GetRef().inputLayouts[InputLayoutManager::eVertex_PosNorDiffUVTan]);


		Renderer::theContextPtr->VSSetShader(ShaderManager::GetVertexShaders()[ShaderManager::DEFAULT_VS], 0, 0);
		Renderer::theContextPtr->PSSetShader(ShaderManager::GetPixelShaders()[ShaderManager::DEFAULT_PS], 0, 0);
		Renderer::theContextPtr->OMSetDepthStencilState(DepthStencilStateManager::GetRef().dsStates[DepthStencilStateManager::DSS_Default], 0);
		if (wireFrame)
		{
			Renderer::theContextPtr->RSSetState(RasterizerStateManager::GetRef().rasterStates[RasterizerStateManager::RS_LINE]);
		}
		else
		{
			Renderer::theContextPtr->RSSetState(RasterizerStateManager::GetRef().rasterStates[RasterizerStateManager::RS_Default]);
		}
		Renderer::theContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Renderer::Render(nodeContext.renderSet);
	}

	void RenderContext::GBufferPacking(RenderNode &node)
	{
		RenderContext& nodeContext = (RenderContext&)node;
		ID3D11RenderTargetView* RTVs[3] = { Renderer::diffuseRTVPtr ,Renderer::normalRTVPtr,Renderer::specRTVPtr };
		Renderer::theContextPtr->OMSetRenderTargets(3, RTVs, Renderer::theDepthStencilViewPtr);
		//Set VertexBuffer
		UINT stripe[3] = { sizeof(Gstream),sizeof(Tstream),sizeof(Astream) };
		UINT offset[3] = { 0,0,0 };
		ID3D11Buffer* buffers[3] = { StreamManager::GetRef().GstreamBufferPtr ,StreamManager::GetRef().TstreamBufferPtr,StreamManager::GetRef().AstreamBufferPtr };
		Renderer::theContextPtr->IASetVertexBuffers(0, 3, buffers, stripe, offset);
		//Set IndexBuffer 
		Renderer::theContextPtr->IASetIndexBuffer(IndexBufferManager::GetRef().indexBufferPtr, DXGI_FORMAT_R32_UINT, 0);
		//Set InputLayout
		Renderer::theContextPtr->IASetInputLayout(InputLayoutManager::GetRef().inputLayouts[InputLayoutManager::eVertex_PosNorDiffUVTanBoneWeight]);
		Renderer::theContextPtr->VSSetShader(ShaderManager::GetVertexShaders()[ShaderManager::GBUFFERPACKINGANIMATION_VS], 0, 0);
		Renderer::theContextPtr->PSSetShader(ShaderManager::GetPixelShaders()[ShaderManager::GBUFFERPACKING_PS], 0, 0);
		Renderer::theContextPtr->OMSetDepthStencilState(DepthStencilStateManager::GetRef().dsStates[DepthStencilStateManager::DSS_Default], 0);
		if (wireFrame)
		{
			Renderer::theContextPtr->RSSetState(RasterizerStateManager::GetRef().rasterStates[RasterizerStateManager::RS_LINE]);
		}
		else
		{
			Renderer::theContextPtr->RSSetState(RasterizerStateManager::GetRef().rasterStates[RasterizerStateManager::RS_Default]);
		}
		Renderer::theContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Renderer::Render(nodeContext.renderSet);
	}

	void RenderContext::GBufferUnpacking(RenderNode &node)
	{
		RenderContext& nodeContext = (RenderContext&)node;
		Renderer::theContextPtr->OMSetRenderTargets(1, &Renderer::theRenderTargetViewPtr, nullptr);
		Renderer::theContextPtr->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		UINT stripe[2] = { 0,0 };
		UINT offset[2] = { 0,0 };
		ID3D11Buffer* buffers[2] = { nullptr ,nullptr };
		Renderer::theContextPtr->IASetVertexBuffers(0, 2, buffers, stripe, offset);

		Renderer::theContextPtr->IASetInputLayout(nullptr);
		Renderer::theContextPtr->VSSetShader(ShaderManager::GetVertexShaders()[ShaderManager::GBUFFERUNPACKING_VS], 0, 0);
		Renderer::theContextPtr->PSSetShader(ShaderManager::GetPixelShaders()[ShaderManager::GBUFFERUNPACKING_PS], 0, 0);
		Renderer::theContextPtr->RSSetState(RasterizerStateManager::GetRef().rasterStates[RasterizerStateManager::RS_Default]);
		Renderer::theContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		Renderer::Render(nodeContext.renderSet);
	}


	void RenderContext::ToggleWireFrame()
	{
		wireFrame = !wireFrame;
	}
}