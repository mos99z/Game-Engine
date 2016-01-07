#include "stdafx.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "RenderSet.h"
#include "RasterizerStateManager.h"
#include "DepthStencilStateManager.h"
namespace RendererD3D
{
	RenderContext::RenderContext()
	{
		std::ifstream fin;
		fin.open("ShaderObj\\Default_VS.cso", std::ios_base::binary);
		fin.seekg(0, std::ios_base::end);
		UINT VSbyteCodeSize = (UINT)fin.tellg();
		char* VSbyteCode = new char[VSbyteCodeSize];
		fin.seekg(0, std::ios_base::beg);
		fin.read(VSbyteCode, VSbyteCodeSize);
		Renderer::theDevicePtr->CreateVertexShader(VSbyteCode, VSbyteCodeSize, nullptr, &default_VS.p);
		fin.close();

		fin.open("ShaderObj\\Default_PS.cso", std::ios_base::binary);
		fin.seekg(0, std::ios_base::end);
		UINT PSbyteCodeSize = (UINT)fin.tellg();
		char* PSbyteCode = new char[PSbyteCodeSize];
		fin.seekg(0, std::ios_base::beg);
		fin.read(PSbyteCode, PSbyteCodeSize);
		Renderer::theDevicePtr->CreatePixelShader(PSbyteCode, PSbyteCodeSize, nullptr, &default_PS.p);
		fin.close();
		renderSetPtr = new RenderSet;
	}


	RenderContext::~RenderContext()
	{
		delete renderSetPtr;
	}

	void RenderContext::Draw(RenderNode &node)
	{
		RenderContext& nodeContext = (RenderContext&)node;
		Renderer::theContextPtr->VSSetShader(nodeContext.default_VS, 0, 0);
		Renderer::theContextPtr->PSSetShader(nodeContext.default_PS, 0, 0);
		Renderer::theContextPtr->OMSetDepthStencilState(DepthStencilStateManager::GetRef().dsStates[2],0);
		//Renderer::theContextPtr->RSSetState(RasterizerStateManager::GetRef().rasterStates[0]);
		Renderer::theContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Renderer::Render(*nodeContext.renderSetPtr);
	}
}