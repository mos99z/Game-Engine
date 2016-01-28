#include "stdafx.h"
#include "Renderer.h"
#include "RenderMaterial.h"
#include "RenderSet.h"
#include <DDSTextureLoader.h>
namespace RendererD3D
{
	std::unordered_map<std::wstring, RenderMaterial::Texture> RenderMaterial::AllMaterials;

	void RenderMaterial::LoadATexture(std::wstring& _texturePath, unsigned int _textureType)
	{
		if (!AllMaterials.count(_texturePath))
		{
			Texture newTexture;
			newTexture.type = (eTextureTypes)_textureType;
			DirectX::CreateDDSTextureFromFile(Renderer::theDevicePtr, _texturePath.c_str(), nullptr, &newTexture.SRV);
			AllMaterials[_texturePath] = newTexture;
		}
	}


	void RenderMaterial::AddMaterial(std::wstring _texturePath, unsigned int _textureType)
	{
		LoadATexture(_texturePath, _textureType);
		materials.push_back(AllMaterials[_texturePath]);
	}

	RenderMaterial::RenderMaterial()
	{
		RenderFunc = RenderMaterial::Draw;
	}


	RenderMaterial::~RenderMaterial()
	{
		for (auto& material : AllMaterials)
		{
			ReleaseCOM(material.second.SRV);
		}
	}

	void RenderMaterial::Draw(RenderNode &node)
	{
		RenderMaterial& nodeMaterial = (RenderMaterial&)node;
		std::vector<ID3D11ShaderResourceView*> SRVs;
		for (auto& i : nodeMaterial.materials)
		{
			SRVs.push_back(i.get().SRV);
		}
		Renderer::theContextPtr->PSSetShaderResources(0, (UINT)SRVs.size(), SRVs.data());

		Renderer::Render(nodeMaterial.renderSet);
		//Unbind SRVs
		ID3D11ShaderResourceView* nullSRVs[D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT]{ nullptr };
		Renderer::theContextPtr->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT, nullSRVs);
	}

	void RenderMaterial::GBufferUnpacking(RenderNode &node)
	{
		RenderMaterial& nodeMaterial = (RenderMaterial&)node;
		ID3D11ShaderResourceView* SRVs[4]
		{
			Renderer::depthSRVPtr,
			Renderer::diffuseSRVPtr,
			Renderer::normalSRVPtr,
			Renderer::specSRVPtr
		};
		Renderer::theContextPtr->PSSetShaderResources(1, 4, SRVs);
		Renderer::Render(nodeMaterial.renderSet);

		//Unbind SRVs
		ID3D11ShaderResourceView* nullSRVs[D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT]{ nullptr };
		Renderer::theContextPtr->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT, nullSRVs);
	}
}
