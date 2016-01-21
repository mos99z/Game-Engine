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
		ID3D11VertexShader* Default_VS = nullptr;
		LoadVertexShaderFromFile(&Default_VS, "ShaderObj\\Default_VS.cso");
		VertexShaders.push_back(Default_VS);

		ID3D11VertexShader* GBufferPacking_VS = nullptr;
		LoadVertexShaderFromFile(&GBufferPacking_VS, "ShaderObj\\GBufferPacking_VS.cso");
		VertexShaders.push_back(std::move(GBufferPacking_VS));

		ID3D11VertexShader* GBufferUnpacking_VS = nullptr;
		LoadVertexShaderFromFile(&GBufferUnpacking_VS, "ShaderObj\\GBufferUnpacking_VS.cso");
		VertexShaders.push_back(std::move(GBufferUnpacking_VS));


	}
	void 	ShaderManager::CreatePixelShaders()
	{
		ID3D11PixelShader* Default_PS = nullptr;
		LoadPixelShaderFromFile(&Default_PS, "ShaderObj\\Default_PS.cso");
		PixelShaders.push_back(std::move(Default_PS));

		ID3D11PixelShader* GBufferPacking_PS = nullptr;
		LoadPixelShaderFromFile(&GBufferPacking_PS, "ShaderObj\\GBufferPacking_PS.cso");
		PixelShaders.push_back(std::move(GBufferPacking_PS));

		ID3D11PixelShader* GBufferUnpacking_PS = nullptr;
		LoadPixelShaderFromFile(&GBufferUnpacking_PS, "ShaderObj\\GBufferUnpacking_PS.cso");
		PixelShaders.push_back(std::move(GBufferUnpacking_PS));
	}
}
