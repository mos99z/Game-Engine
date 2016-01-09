#include "stdafx.h"
#include "DepthStencilStateManager.h"
#include "Renderer.h"
namespace  RendererD3D
{
	DepthStencilStateManager* DepthStencilStateManager::instancePtr = nullptr;
	DepthStencilStateManager::DepthStencilStateManager()
	{
		D3D11_DEPTH_STENCIL_DESC dssDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
		dssDesc.DepthEnable = FALSE;

		Renderer::theDevicePtr->CreateDepthStencilState(&dssDesc, &dsStates[DSS_NoDepth].p);

		dssDesc.DepthEnable = TRUE;
		Renderer::theDevicePtr->CreateDepthStencilState(&dssDesc, &dsStates[DSS_Default].p);

		dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		Renderer::theDevicePtr->CreateDepthStencilState(&dssDesc, &dsStates[DSS_LessEqual].p);
		
	}
	

	DepthStencilStateManager& DepthStencilStateManager::GetRef()
	{
		if (!instancePtr)
		{
			instancePtr = new DepthStencilStateManager;
		}
		return *instancePtr;
	}

	void DepthStencilStateManager::DeleteInstance()
	{
		delete instancePtr;
		instancePtr = nullptr;
	}

}