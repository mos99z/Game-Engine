#include "stdafx.h"
#include "RasterizerStateManager.h"
#include "Renderer.h"
namespace  RendererD3D
{
	RasterizerStateManager* RasterizerStateManager::instancePtr = nullptr;
	RasterizerStateManager& RasterizerStateManager::GetRef()
	{
		if (!instancePtr)
		{
			instancePtr = new RasterizerStateManager;
		}
		return *instancePtr;
	}

	void RasterizerStateManager::DeleteInstance()
	{
		delete instancePtr;
		instancePtr = nullptr;
	}


	RasterizerStateManager::RasterizerStateManager()
	{
		D3D11_RASTERIZER_DESC desc;

		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.FrontCounterClockwise = FALSE;
		desc.DepthBias = 0;
		desc.SlopeScaledDepthBias = 0.0f;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = TRUE;
		desc.ScissorEnable = FALSE;

	
		desc.MultisampleEnable = FALSE;
		desc.AntialiasedLineEnable = FALSE;
		Renderer::theDevicePtr->CreateRasterizerState(&desc, &rasterStates[RS_Default].p);

		desc.FillMode = D3D11_FILL_WIREFRAME;
		Renderer::theDevicePtr->CreateRasterizerState(&desc, &rasterStates[RS_LINE].p);
		desc.FillMode = D3D11_FILL_SOLID;

		desc.FrontCounterClockwise = TRUE;
		Renderer::theDevicePtr->CreateRasterizerState(&desc, &rasterStates[RS_CCW].p);

		desc.CullMode = D3D11_CULL_NONE;
		Renderer::theDevicePtr->CreateRasterizerState(&desc, &rasterStates[RS_NOCULL].p);
		desc.CullMode = D3D11_CULL_BACK;
		Renderer::theDevicePtr->CreateRasterizerState(&desc, &rasterStates[RS_BACKCULL].p);
	}



}
