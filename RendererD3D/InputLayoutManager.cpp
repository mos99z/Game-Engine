#include "stdafx.h"
#include "InputLayoutManager.h"


namespace RendererD3D
{
	InputLayoutManager* InputLayoutManager::instancePtr = nullptr;

	 InputLayoutManager& InputLayoutManager::GetRef()
	{
		if (!instancePtr)
		{
			instancePtr = new InputLayoutManager;
		}
		return *instancePtr;
	}

	 void InputLayoutManager::DeleteInstance()
	 {
		 delete instancePtr;
		 instancePtr = nullptr;
	 }

	InputLayoutManager::InputLayoutManager()
	{
		//eVertex_POSNORDIFF
		std::ifstream fin;
		fin.open("ShaderObj\\Vertex_POSNORDIFF.cso",  std::ios_base::binary);
		fin.seekg(0, std::ios_base::end);
		UINT VSbyteCodeSize = (UINT)fin.tellg();
		char* VSbyteCode = new char[VSbyteCodeSize];
		fin.seekg(0, std::ios_base::beg);
		fin.read(VSbyteCode, VSbyteCodeSize);
		const UINT Vertex_POSNORDIFF_numofElements = 3;
		D3D11_INPUT_ELEMENT_DESC vertexPNDSDesc[Vertex_POSNORDIFF_numofElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		Renderer::theDevicePtr->CreateInputLayout(vertexPNDSDesc, Vertex_POSNORDIFF_numofElements, VSbyteCode,
			VSbyteCodeSize, &inputLayouts[eVertex_POSNORDIFF]);
		fin.close();
		delete VSbyteCode;


		//eVertex_PosNorDiffSpecUVTan
		fin.open("ShaderObj\\Vertex_PosNorDiffUVTan.cso", std::ios_base::binary);
		fin.seekg(0, std::ios_base::end);
		VSbyteCodeSize = (UINT)fin.tellg();
		VSbyteCode = new char[VSbyteCodeSize];
		fin.seekg(0, std::ios_base::beg);
		fin.read(VSbyteCode, VSbyteCodeSize);
		const UINT Vertex_PosNorDiffUVTan_numofElements = 5;
		D3D11_INPUT_ELEMENT_DESC vertexPNDSUTDesc[Vertex_PosNorDiffUVTan_numofElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		Renderer::theDevicePtr->CreateInputLayout(vertexPNDSUTDesc, Vertex_PosNorDiffUVTan_numofElements, VSbyteCode,
			VSbyteCodeSize, &inputLayouts[eVertex_PosNorDiffUVTan]);
		fin.close();
		delete VSbyteCode;

		//eVertex_PosNorDiffSpecUVTanBoneWeight
		fin.open("ShaderObj\\Vertex_PosNorDiffUVTanBoneWeight.cso", std::ios_base::binary);
		fin.seekg(0, std::ios_base::end);
		VSbyteCodeSize = (UINT)fin.tellg();
		VSbyteCode = new char[VSbyteCodeSize];
		fin.seekg(0, std::ios_base::beg);
		fin.read(VSbyteCode, VSbyteCodeSize);
		const UINT Vertex_PosNorDiffUVTanBoneWeight_numofElements = 7;
		D3D11_INPUT_ELEMENT_DESC vertexPNDSUTDBWDesc[Vertex_PosNorDiffUVTanBoneWeight_numofElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_SINT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		Renderer::theDevicePtr->CreateInputLayout(vertexPNDSUTDBWDesc, Vertex_PosNorDiffUVTanBoneWeight_numofElements, VSbyteCode,
			VSbyteCodeSize, &inputLayouts[eVertex_PosNorDiffUVTanBoneWeight]);
		fin.close();
		delete VSbyteCode;
	}




}