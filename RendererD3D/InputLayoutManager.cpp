#include "stdafx.h"
#include "InputLayoutManager.h"


namespace RendererD3D
{
	InputLayoutManager::InputLayoutManager()
	{
		//eVertex_POSNORDIFFSPEC
		std::ifstream fin;
		fin.open("ShaderObj\\Vertex_POSNORDIFFSPEC.cso",  std::ios_base::binary);
		fin.seekg(0, std::ios_base::end);
		UINT VSbyteCodeSize = (UINT)fin.tellg();
		char* VSbyteCode = new char[VSbyteCodeSize];
		fin.seekg(0, std::ios_base::beg);
		fin.read(VSbyteCode, VSbyteCodeSize);
		const UINT Vertex_POSNORDIFFSPEC_numofElements = 4;
		D3D11_INPUT_ELEMENT_DESC vertexPNDSDesc[Vertex_POSNORDIFFSPEC_numofElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		Renderer::theDevicePtr->CreateInputLayout(vertexPNDSDesc, Vertex_POSNORDIFFSPEC_numofElements, VSbyteCode,
			VSbyteCodeSize, &inputLayouts[eVertex_POSNORDIFFSPEC]);
		fin.close();
		delete VSbyteCode;


		//eVertex_PosNorDiffSpecUVTan
		fin.open("ShaderObj\\Vertex_PosNorDiffSpecUVTan.cso", std::ios_base::binary);
		fin.seekg(0, std::ios_base::end);
		VSbyteCodeSize = (UINT)fin.tellg();
		VSbyteCode = new char[VSbyteCodeSize];
		fin.seekg(0, std::ios_base::beg);
		fin.read(VSbyteCode, VSbyteCodeSize);
		const UINT Vertex_PosNorDiffSpecUVTan_numofElements = 6;
		D3D11_INPUT_ELEMENT_DESC vertexPNDSUTDesc[Vertex_PosNorDiffSpecUVTan_numofElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		Renderer::theDevicePtr->CreateInputLayout(vertexPNDSUTDesc, Vertex_PosNorDiffSpecUVTan_numofElements, VSbyteCode,
			VSbyteCodeSize, &inputLayouts[eVertex_PosNorDiffSpecUVTan]);
		fin.close();
		delete VSbyteCode;

		//eVertex_PosNorDiffSpecUVTanBoneWeight
		fin.open("ShaderObj\\Vertex_PosNorDiffSpecUVTanBoneWeight.cso", std::ios_base::binary);
		fin.seekg(0, std::ios_base::end);
		VSbyteCodeSize = (UINT)fin.tellg();
		VSbyteCode = new char[VSbyteCodeSize];
		fin.seekg(0, std::ios_base::beg);
		fin.read(VSbyteCode, VSbyteCodeSize);
		const UINT Vertex_PosNorDiffSpecUVTanBoneWeight_numofElements = 8;
		D3D11_INPUT_ELEMENT_DESC vertexPNDSUTDBWDesc[Vertex_PosNorDiffSpecUVTanBoneWeight_numofElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_SINT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		Renderer::theDevicePtr->CreateInputLayout(vertexPNDSUTDBWDesc, Vertex_PosNorDiffSpecUVTanBoneWeight_numofElements, VSbyteCode,
			VSbyteCodeSize, &inputLayouts[eVertex_PosNorDiffSpecUVTanBoneWeight]);
		fin.close();
		delete VSbyteCode;
	}


	InputLayoutManager::~InputLayoutManager()
	{
		for (auto& inputLayout : inputLayouts)
		{
			ReleaseCOM(inputLayout);
		}
	}

}