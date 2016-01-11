#include "stdafx.h"
#include <type_traits>
#include "ShaderManager.h"

namespace  RendererD3D
{


	bool LoadVertexShaderFromFile(ID3D11VertexShader** shaderPtr, std::string filepath)
	{
		std::ifstream fin;
		fin.open(filepath, std::ios_base::binary);
		if (fin.is_open())
		{
			
			fin.seekg(0, std::ios_base::end);
			UINT byteCodeSize = (UINT)fin.tellg();
			char* byteCode = new char[byteCodeSize];
			fin.seekg(0, std::ios_base::beg);
			fin.read(byteCode, byteCodeSize);
			Renderer::theDevicePtr->CreateVertexShader(byteCode, byteCodeSize, nullptr, shaderPtr);
			fin.close();
			delete[] byteCode;
			return true;
		}
		return false;
	}

	bool LoadPixelShaderFromFile(ID3D11PixelShader** shaderPtr, std::string filepath)
	{
		std::ifstream fin;
		fin.open(filepath, std::ios_base::binary);
		if (fin.is_open())
		{
			fin.seekg(0, std::ios_base::end);
			UINT byteCodeSize = (UINT)fin.tellg();
			char* byteCode = new char[byteCodeSize];
			fin.seekg(0, std::ios_base::beg);
			fin.read(byteCode, byteCodeSize);
			Renderer::theDevicePtr->CreatePixelShader(byteCode, byteCodeSize, nullptr, shaderPtr);
			fin.close();
			delete[] byteCode;
			return true;
		}
		return false;
	}

	ShaderManager* ShaderManager::instancePtr = nullptr;
	std::vector<ID3D11VertexShader*> ShaderManager::VertexShaders;
	std::vector<ID3D11PixelShader*> ShaderManager::PixelShaders;

	ShaderManager& 	ShaderManager::GetRef()
	{
		if (!instancePtr)
		{
			instancePtr = new ShaderManager;
			CreateVertexShaders();
			CreatePixelShaders();
		}
		return *instancePtr;
	}
	void 	ShaderManager::DeleteInstance()
	{
		for (auto& vShader : VertexShaders)
		{
			ReleaseCOM(vShader);
		}
		for (auto& pShader : PixelShaders)
		{
			ReleaseCOM(pShader);
		}
		delete instancePtr;
		instancePtr = nullptr;
	}
	void 	ShaderManager::CreateVertexShaders()
	{
		ID3D11VertexShader* VertexShaderPtr = nullptr;
		LoadVertexShaderFromFile(&VertexShaderPtr, "ShaderObj\\Default_VS.cso");
		VertexShaders.push_back(VertexShaderPtr);
	}
	void 	ShaderManager::CreatePixelShaders()
	{
		ID3D11PixelShader* PixelShaderPtr = nullptr;
		LoadPixelShaderFromFile(&PixelShaderPtr, "ShaderObj\\Default_PS.cso");
		PixelShaders.push_back(PixelShaderPtr);
	}
}
