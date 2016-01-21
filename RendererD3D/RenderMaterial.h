#pragma once

#ifdef RENDERMATERIALDLL_EXPORTS
#define  RENDERMATERIALDLL __declspec(dllexport) 
#else
#define  RENDERMATERIALDLL __declspec(dllimport) 
#endif

#include "RenderNode.h"
#include "RenderSet.h"

namespace RendererD3D
{
	class RenderMaterial : public RenderNode
	{
	public:
		enum eTextureTypes
		{
			DIFFUSE = 0,SPECULAR,NORMAL, TEXTURETYPE_COUNT,
		};
	private:
		struct Texture
		{
			eTextureTypes type;
			ID3D11ShaderResourceView* SRV = nullptr;
		};
		static std::unordered_map<std::wstring, Texture> AllMaterials;
		std::vector<std::reference_wrapper<Texture>> materials;
		RENDERMATERIALDLL static void LoadATexture(std::wstring& _texturePath, unsigned int _textureType = 0);
	public:
		RenderSet renderSet;
		RENDERMATERIALDLL RenderMaterial();
		RENDERMATERIALDLL ~RenderMaterial();
		RENDERMATERIALDLL inline RenderSet& GetSet() { return renderSet; }
		void AddMaterial(std::wstring _texturePath, unsigned int _textureType = 0);
		RENDERMATERIALDLL static void Draw(RenderNode &node);
		RENDERMATERIALDLL static void GBufferUnpacking(RenderNode &node);
	};
}
