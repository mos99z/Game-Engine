#pragma once
#include <vector>
#include "Renderer.h"
namespace  RendererD3D
{
	class ShaderManager
	{
		
		static std::vector<ID3D11VertexShader*> VertexShaders;
		static std::vector<ID3D11PixelShader*> PixelShaders;
		ShaderManager() {}
		static ShaderManager* instancePtr;
	public:
		enum eVertexShaders
		{
			DEFAULT_VS,
		};
		enum ePixelShaders
		{
			DEFAULT_PS,
		};
		~ShaderManager() {}
		static ShaderManager& GetRef();
		static void DeleteInstance();
		static void CreateVertexShaders();
		static void CreatePixelShaders();

		static inline std::vector<ID3D11VertexShader*>& GetVertexShaders() { return VertexShaders; }
		static inline std::vector<ID3D11PixelShader*>& GetPixelShaders() { return PixelShaders; }

	};
}
