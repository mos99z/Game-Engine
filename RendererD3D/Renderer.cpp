#include "stdafx.h"
#include "Renderer.h"
#include "RenderContext.h"
#include "RenderMaterial.h"
#include "RenderShape.h"
#include "RenderSet.h"
#include "ShaderManager.h"
#include "InputLayoutManager.h"
#include "RasterizerStateManager.h"
#include "DepthStencilStateManager.h"
#include "BlendStateManager.h"
#include "IndexBufferManager.h"
#include <DDSTextureLoader.h>
#include "StreamManager.h"
#include "Shaders\InputLayout.hlsli"
namespace RendererD3D
{

	ID3D11Device* Renderer::theDevicePtr = nullptr;
	ID3D11DeviceContext* Renderer::theContextPtr = nullptr;
	IDXGISwapChain* Renderer::theSwapChainPtr = nullptr;
	ID3D11RenderTargetView* Renderer::theRenderTargetViewPtr = nullptr;
	ID3D11Texture2D* Renderer::theBackBufferPtr = nullptr;
	ID3D11Texture2D* Renderer::theDepthStencilBufferPtr = nullptr;
	ID3D11DepthStencilView* Renderer::theDepthStencilViewPtr = nullptr;
	D3D11_VIEWPORT Renderer::theScreenViewport;
	UINT Renderer::resolutionWidth = 0;
	UINT Renderer::resolutionHeight = 0;
	UINT Renderer::theRenderCounter = 1;
	cbPerObject Renderer::thePerObjectData;
	ID3D11Buffer* Renderer::thePerObjectCBuffer;
	DirectX::XMMATRIX Renderer::viewMatrix;
	DirectX::XMMATRIX Renderer::proj;
	ID3D11Buffer* Renderer::vertexBuffer = nullptr;

	RenderContext*		Renderer::cubeContextPtr = nullptr;
	RenderShape*		Renderer::cubeShapePtr = nullptr;
	RenderMaterial*	Renderer::cubeMaterialPtr = nullptr;
	RenderSet* Renderer::rSetPtr = new RenderSet;
	ShaderManager* Renderer::shaderManagerPtr = nullptr;
	ID3D11SamplerState* Renderer::anisoWrapSampler = nullptr;
	ID3D11ShaderResourceView* Renderer::cubeSRV = nullptr;
	StreamManager* Renderer::streamManagerPtr = nullptr;
	std::vector<RenderShape>  Renderer::renderShapes;
	Camera Renderer::camera;
	Renderer& Renderer::GetRef()
	{
		static Renderer renderer;
		return renderer;
	}

	void Renderer::Initialize(HWND hWnd, UINT resWidth, UINT resHeight)
	{

		resolutionHeight = resHeight;
		resolutionWidth = resWidth;

		DXGI_SWAP_CHAIN_DESC swapchain_DESC;
		ZeroMemory(&swapchain_DESC, sizeof(swapchain_DESC));
		swapchain_DESC.BufferCount = 1;
		swapchain_DESC.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchain_DESC.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchain_DESC.BufferDesc.Height = resolutionHeight;
		swapchain_DESC.BufferDesc.Width = resolutionWidth;
		swapchain_DESC.OutputWindow = hWnd;
		swapchain_DESC.SampleDesc.Count = 1;
		swapchain_DESC.Windowed = true;



		D3D11CreateDeviceAndSwapChain
			(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_SINGLETHREADED,
				nullptr,
				NULL,
				D3D11_SDK_VERSION,
				&swapchain_DESC,
				&theSwapChainPtr,
				&theDevicePtr,
				nullptr,
				&theContextPtr
				);


		theSwapChainPtr->GetBuffer(0, _uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&theBackBufferPtr));
		theDevicePtr->CreateRenderTargetView(theBackBufferPtr, nullptr, &theRenderTargetViewPtr);
		ReleaseCOM(theBackBufferPtr);



		D3D11_TEXTURE2D_DESC ZBufferdesc;
		ZeroMemory(&ZBufferdesc, sizeof(ZBufferdesc));
		ZBufferdesc.Width = resolutionWidth;
		ZBufferdesc.Height = resolutionHeight;
		ZBufferdesc.MipLevels = 1;
		ZBufferdesc.ArraySize = 1;
		ZBufferdesc.SampleDesc.Count = swapchain_DESC.SampleDesc.Count;
		ZBufferdesc.SampleDesc.Quality = swapchain_DESC.SampleDesc.Quality;
		ZBufferdesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		ZBufferdesc.Usage = D3D11_USAGE_DEFAULT;
		ZBufferdesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		ZBufferdesc.CPUAccessFlags = 0;
		ZBufferdesc.MiscFlags = 0;
		theDevicePtr->CreateTexture2D(&ZBufferdesc, 0, &theDepthStencilBufferPtr);


		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDESC = CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D24_UNORM_S8_UINT);

		theDevicePtr->CreateDepthStencilView(theDepthStencilBufferPtr, &depthStencilViewDESC, &theDepthStencilViewPtr);


		theContextPtr->OMSetRenderTargets(1, &theRenderTargetViewPtr, theDepthStencilViewPtr);

		ZeroMemory(&theScreenViewport, sizeof(theScreenViewport));
		theScreenViewport.MaxDepth = 1.0f;
		theScreenViewport.MinDepth = 0.0f;
		theScreenViewport.TopLeftX = 0.0f;
		theScreenViewport.TopLeftY = 0.0f;
		theScreenViewport.Width = (float)resolutionWidth;
		theScreenViewport.Height = (float)resolutionHeight;
		theContextPtr->RSSetViewports(1, &theScreenViewport);


		shaderManagerPtr = &ShaderManager::GetRef();
		//Build Constant Buffer
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(cbPerObject);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		theDevicePtr->CreateBuffer(&bd, nullptr, &thePerObjectCBuffer);

		//Set sampler

		D3D11_SAMPLER_DESC desc;
		//anisoWrapSampler
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 16;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		theDevicePtr->CreateSamplerState(&desc, &anisoWrapSampler);
		theContextPtr->PSSetSamplers(0, 1, &anisoWrapSampler);
		
		//Build simple camera stuffs
		proj = camera.GetProj();
		viewMatrix = camera.GetView();


		cubeContextPtr = new RenderContext;
		cubeShapePtr = new RenderShape;
		cubeMaterialPtr = new RenderMaterial;
		renderShapes.push_back(RenderShape());
		renderShapes.push_back(RenderShape());
		renderShapes.push_back(RenderShape());

		for (size_t i = 0; i < renderShapes.size(); i++)
		{
			DirectX::XMStoreFloat4x4(&renderShapes[i].worldMatrix, DirectX::XMMatrixIdentity()* DirectX::XMMatrixScaling(0.05f, 0.05f, 0.05f));
		}
		//DirectX::XMStoreFloat4x4(&cubeShapePtr->worldMatrix, DirectX::XMMatrixIdentity());

		DirectX::XMStoreFloat4x4(&renderShapes[0].worldMatrix, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&renderShapes[1].worldMatrix, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&renderShapes[2].worldMatrix, DirectX::XMMatrixIdentity());
		

		//Get stream manager class
		streamManagerPtr = &StreamManager::GetRef();
		//Set Vertex buffer
		//streamManagerPtr->AddGStream(std::string("Leopard_2A6.AWBX"), *cubeShapePtr);

		
		streamManagerPtr->AddGStream(std::string("Leopard_2A6.AWBX"), renderShapes[0]);
		streamManagerPtr->AddGStream(std::string("Teddy_Idle.AWBX"), renderShapes[1]);
		streamManagerPtr->AddGStream(std::string("Jump.AWBX"), renderShapes[2]);


		//Set VertexBuffer
		/*D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = streamManagerPtr->numofGstream * sizeof (Gstream);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = streamManagerPtr->GstreamRawBufferPtr;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		theDevicePtr->CreateBuffer(&bufferDesc, &InitData, &vertexBuffer);*/

		UINT stripe = sizeof(VERIN_POSNORDIFF);
		UINT offset = 0;
		//theContextPtr->IASetVertexBuffers(0, 1, &vertexBuffer, &stripe, &offset);
		theContextPtr->IASetVertexBuffers(0, 1, &streamManagerPtr->GstreamBufferPtr, &stripe, &offset);

		//Set IndexBuffer 
		theContextPtr->IASetIndexBuffer(IndexBufferManager::GetRef().indexBufferPtr, DXGI_FORMAT_R32_UINT, 0);

		//Set InputLayout
		theContextPtr->IASetInputLayout(InputLayoutManager::GetRef().inputLayouts[InputLayoutManager::eVertex_POSNORDIFF]);

		

		rSetPtr->AddNode(cubeContextPtr);
		cubeContextPtr->renderSet.AddNode(cubeMaterialPtr);
		//cubeMaterialPtr->renderSet.AddNode(cubeShapePtr);
		
		for (size_t i = 0; i < renderShapes.size(); i++)
		{
			cubeMaterialPtr->renderSet.AddNode(&renderShapes[i]);
		}


	
		

		//Load texture for cube 
		
		DirectX::CreateDDSTextureFromFile(theDevicePtr, L"texture.dds", nullptr, &cubeSRV);
		theContextPtr->PSSetShaderResources(0, 1, &cubeSRV);

	}

	void  Renderer::SetResolution(UINT _width, UINT _height)
	{

		ReleaseCOM(theRenderTargetViewPtr);
		ReleaseCOM(theDepthStencilBufferPtr);
		ReleaseCOM(theDepthStencilViewPtr);
		ReleaseCOM(theBackBufferPtr);

		resolutionWidth = _width;
		resolutionHeight = _height;
		ResizeBuffers();
	}

	void  Renderer::Shutdown()
	{
		InputLayoutManager::DeleteInstance();
		RasterizerStateManager::DeleteInstance();
		DepthStencilStateManager::DeleteInstance();
		BlendStateManager::DeleteInstance();
		cubeContextPtr->renderSet.ClearSet();
		cubeMaterialPtr->renderSet.ClearSet();
		rSetPtr->ClearSet();
		delete cubeContextPtr;
		delete cubeShapePtr;
		delete cubeMaterialPtr;
		delete rSetPtr;
		streamManagerPtr->DeleteInstance();
		shaderManagerPtr->DeleteInstance();
		IndexBufferManager::DeleteInstance();
		ReleaseCOM(cubeSRV);
		ReleaseCOM(anisoWrapSampler);
		ReleaseCOM(vertexBuffer);
		ReleaseCOM(thePerObjectCBuffer);
		ReleaseCOM(theSwapChainPtr);
		ReleaseCOM(theRenderTargetViewPtr);
		ReleaseCOM(theDepthStencilViewPtr);
		ReleaseCOM(theDepthStencilBufferPtr);
		ReleaseCOM(theContextPtr);

		//CComPtr<ID3D11Debug> pDebug;
		//theDevicePtr->QueryInterface(IID_PPV_ARGS(&pDebug));
		//if (pDebug != nullptr)
		//{
		//	pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		//	pDebug = nullptr;
		//}
		ReleaseCOM(theDevicePtr);



	}
	void  Renderer::Render(RenderSet &set)
	{
		camera.UpdateView();
		viewMatrix = camera.GetView();
		static float rotSpeed = 0.001f;
		DirectX::XMStoreFloat4x4(&renderShapes[0].worldMatrix, DirectX::XMMatrixScaling(0.05f,0.05f,0.05f)* DirectX::XMMatrixRotationY(rotSpeed));
		DirectX::XMStoreFloat4x4(&renderShapes[1].worldMatrix, DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f)*DirectX::XMMatrixRotationY(rotSpeed));
		DirectX::XMStoreFloat4x4(&renderShapes[2].worldMatrix, DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f)*DirectX::XMMatrixRotationX(-90) * DirectX::XMMatrixRotationY(rotSpeed));
		rotSpeed += 0.0001f;
		RenderNode* item = set.GetHead();
		while (item)
		{
			item->RenderProcess();
			item = item->GetNext();
		}
	}

	RenderSet& Renderer::GetSet()
	{
		return *(rSetPtr);
	}


	void Renderer::Render(RenderSet &set, RenderFunc renderFuncOverride)
	{
	}

	void  Renderer::ResizeBuffers()
	{
		theSwapChainPtr->ResizeBuffers(0, resolutionWidth, resolutionHeight, DXGI_FORMAT_UNKNOWN, 0);
		theSwapChainPtr->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&theBackBufferPtr));
		theDevicePtr->CreateRenderTargetView(theBackBufferPtr, 0, &theRenderTargetViewPtr);
		ReleaseCOM(theBackBufferPtr);

		DXGI_SWAP_CHAIN_DESC swapchain_DESC;
		theSwapChainPtr->GetDesc(&swapchain_DESC);
		D3D11_TEXTURE2D_DESC ZBufferdesc;
		ZBufferdesc.Width = resolutionWidth;
		ZBufferdesc.Height = resolutionHeight;
		ZBufferdesc.MipLevels = 1;
		ZBufferdesc.ArraySize = 1;
		ZBufferdesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		ZBufferdesc.SampleDesc.Count = swapchain_DESC.SampleDesc.Count;
		ZBufferdesc.SampleDesc.Quality = swapchain_DESC.SampleDesc.Quality;
		ZBufferdesc.Usage = D3D11_USAGE_DEFAULT;
		ZBufferdesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		ZBufferdesc.CPUAccessFlags = 0;
		ZBufferdesc.MiscFlags = 0;
		theDevicePtr->CreateTexture2D(&ZBufferdesc, 0, &theDepthStencilBufferPtr);


		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDESC = CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D24_UNORM_S8_UINT);
		theDevicePtr->CreateDepthStencilView(theDepthStencilBufferPtr, &depthStencilViewDESC, &theDepthStencilViewPtr);
		theContextPtr->OMSetRenderTargets(1, &theRenderTargetViewPtr, theDepthStencilViewPtr);


		ZeroMemory(&theScreenViewport, sizeof(theScreenViewport));
		theScreenViewport.MaxDepth = 1.0f;
		theScreenViewport.MinDepth = 0.0f;
		theScreenViewport.TopLeftX = 0.0f;
		theScreenViewport.TopLeftY = 0.0f;
		theScreenViewport.Width = (float)resolutionWidth;
		theScreenViewport.Height = (float)resolutionHeight;
		theContextPtr->RSSetViewports(1, &theScreenViewport);

	}
	ID3D11ShaderResourceView * Renderer::GetDepthSRV()
	{
		return nullptr;
	}
	void Renderer::SetPerObjectData(float4x4& _world)
	{
		DirectX::XMMATRIX vp = DirectX::XMMatrixMultiply(viewMatrix, proj);
		thePerObjectData.gWorld = _world;
		DirectX::XMStoreFloat4x4(&thePerObjectData.gMVP, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&_world), vp));

		D3D11_MAPPED_SUBRESOURCE edit;
		theContextPtr->Map(thePerObjectCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &edit);
		memcpy(edit.pData, &thePerObjectData, sizeof(thePerObjectData));
		theContextPtr->Unmap(thePerObjectCBuffer, 0);

		theContextPtr->VSSetConstantBuffers(cbPerObject::REGISTER_SLOT, 1, &thePerObjectCBuffer);
		theContextPtr->PSSetConstantBuffers(cbPerObject::REGISTER_SLOT, 1, &thePerObjectCBuffer);
	}

	

}

