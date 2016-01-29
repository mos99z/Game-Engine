#include "stdafx.h"
#include "RenderShape.h"
#include "RenderMesh.h"
#include "Renderer.h"
#include "BlendStateManager.h"
#include "RasterizerStateManager.h"
#include "DepthStencilStateManager.h"
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

		/*Renderer::theContextPtr->RSSetState(RasterizerStateManager::GetRef().rasterStates[RasterizerStateManager::RS_CCW]);
		Renderer::theContextPtr->OMSetDepthStencilState(DepthStencilStateManager::GetRef().dsStates[DepthStencilStateManager::DSS_DEFERRED], 0);
		Renderer::theContextPtr->OMSetBlendState(BlendStateManager::GetRef().blendStates[BlendStateManager::BS_Additive], nullptr, 0xffffffff);*/

		cbDirLight DirLight;
		DirLight.DLightColor = float4{ 1.0f,0.0f,0.0f,1.0f };
		DirLight.lightPos = float4{ 0.0f, 100.0f,0.0f,1.0f };
		D3D11_MAPPED_SUBRESOURCE lightDataMap;
		Renderer::theContextPtr->Map(Renderer::thePerDirLightCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &lightDataMap);
		memcpy(lightDataMap.pData, &DirLight, sizeof(cbDirLight));
		Renderer::theContextPtr->Unmap(Renderer::thePerDirLightCBuffer, 0);

		Renderer::theContextPtr->VSSetConstantBuffers(cbDirLight::REGISTER_SLOT, 1, &Renderer::thePerDirLightCBuffer);
		Renderer::theContextPtr->PSSetConstantBuffers(cbDirLight::REGISTER_SLOT, 1, &Renderer::thePerDirLightCBuffer);


		Renderer::theContextPtr->Draw(4, 0);


		


		/*DirLight.DLightColor = float4{ 0.0f,1.0f,0.0f,1.0f };
		DirLight.lightPos = float4{ 0.0f, 100.0f,0.0f,1.0f };
		Renderer::theContextPtr->Map(Renderer::thePerDirLightCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &lightDataMap);
		memcpy(lightDataMap.pData, &DirLight, sizeof(cbDirLight));
		Renderer::theContextPtr->Unmap(Renderer::thePerDirLightCBuffer, 0);

		Renderer::theContextPtr->VSSetConstantBuffers(cbDirLight::REGISTER_SLOT, 1, &Renderer::thePerDirLightCBuffer);
		Renderer::theContextPtr->PSSetConstantBuffers(cbDirLight::REGISTER_SLOT, 1, &Renderer::thePerDirLightCBuffer);
		Renderer::theContextPtr->Draw(4, 0);*/
	}
}