#include "stdafx.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "RasterizerStateManager.h"
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
		Renderer::theDevicePtr->CreateVertexShader(VSbyteCode, VSbyteCodeSize, nullptr, &default_VS);


		fin.open("ShaderObj\\Default_PS.cso", std::ios_base::binary);
		fin.seekg(0, std::ios_base::end);
		UINT PSbyteCodeSize = (UINT)fin.tellg();
		char* PSbyteCode = new char[PSbyteCodeSize];
		fin.seekg(0, std::ios_base::beg);
		fin.read(PSbyteCode, PSbyteCodeSize);
		Renderer::theDevicePtr->CreatePixelShader(PSbyteCode, PSbyteCodeSize, nullptr, &default_PS);
	}


	RenderContext::~RenderContext()
	{
		ReleaseCOM(default_VS);
		ReleaseCOM(default_PS);
	}

	void RenderContext::Draw(RenderNode &node)
	{
		RenderContext& nodeContext = (RenderContext&)node;
		Renderer::theContextPtr->VSSetShader(nodeContext.default_VS, nullptr, 0);
		Renderer::theContextPtr->PSSetShader(nodeContext.default_PS, nullptr, 0);
		Renderer::theContextPtr->RSSetState(RasterizerStateManager::GetRef().rasterStates[0]);
		
	}
}