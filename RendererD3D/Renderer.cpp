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
namespace RendererD3D
{

	ID3D11Device* Renderer::theDevicePtr = nullptr;
	ID3D11DeviceContext* Renderer::theContextPtr = nullptr;
	IDXGISwapChain* Renderer::theSwapChainPtr = nullptr;
	ID3D11RenderTargetView* Renderer::theRenderTargetViewPtr = nullptr;



	ID3D11RenderTargetView* Renderer::depthRTVPtr = nullptr;
	ID3D11RenderTargetView* Renderer::diffuseRTVPtr = nullptr;
	ID3D11RenderTargetView* Renderer::normalRTVPtr = nullptr;
	ID3D11RenderTargetView* Renderer::specRTVPtr = nullptr;
	ID3D11ShaderResourceView* Renderer::depthSRVPtr = nullptr;
	ID3D11ShaderResourceView* Renderer::diffuseSRVPtr = nullptr;
	ID3D11ShaderResourceView* Renderer::normalSRVPtr = nullptr;
	ID3D11ShaderResourceView* Renderer::specSRVPtr = nullptr;
	ID3D11Texture2D* Renderer::depthResourcePtr;
	ID3D11Texture2D* Renderer::diffuseResourcePtr;
	ID3D11Texture2D* Renderer::normalResourcePtr;
	ID3D11Texture2D* Renderer::specResourcePtr;



	ID3D11Texture2D* Renderer::theBackBufferPtr = nullptr;
	ID3D11Texture2D* Renderer::theDepthStencilBufferPtr = nullptr;
	ID3D11DepthStencilView* Renderer::theDepthStencilViewPtr = nullptr;
	D3D11_VIEWPORT Renderer::theScreenViewport;
	UINT Renderer::resolutionWidth = 0;
	UINT Renderer::resolutionHeight = 0;
	UINT Renderer::theRenderCounter = 1;
	cbPerObject Renderer::thePerObjectData;
	cbPerCamera Renderer::thePerCameraData;
	ID3D11Buffer* Renderer::thePerObjectCBuffer = nullptr;
	ID3D11Buffer* Renderer::thePerCameraCBuffer = nullptr;
	ID3D11Buffer* Renderer::thePerDirLightCBuffer = nullptr;

	ID3D11Buffer* Renderer::vertexBuffer = nullptr;

	RenderContext*		Renderer::GBufferPackingContextPtr = nullptr;
	RenderMaterial*	Renderer::GBufferPackingMaterialPtr = nullptr;
	RenderContext*	Renderer::GBufferUnPackingContextPtr = nullptr;
	RenderShape* Renderer::GBufferUnPackingShapePtr = nullptr;
	RenderMaterial*Renderer::GBufferUnPackingMaterialPtr = nullptr;

	RenderSet* Renderer::rSetPtr = new RenderSet;
	ShaderManager* Renderer::shaderManagerPtr = nullptr;

	//Samplers
	ID3D11SamplerState* Renderer::pointSampler = nullptr;
	ID3D11SamplerState* Renderer::anisoClampSampler = nullptr;
	ID3D11SamplerState* Renderer::anisoWrapSampler = nullptr;



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
		swapchain_DESC.SampleDesc.Quality = 0;//D3D11_STANDARD_MULTISAMPLE_PATTERN;
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
		//Gbuffers
		//DepthSRV
		D3D11_SHADER_RESOURCE_VIEW_DESC GBufferDesc;
		ZeroMemory(&GBufferDesc, sizeof(GBufferDesc));
		GBufferDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		GBufferDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		GBufferDesc.Texture2D.MipLevels = 1;
		theDevicePtr->CreateShaderResourceView(theDepthStencilBufferPtr, &GBufferDesc, &depthSRVPtr);

		//DiffuseSRV
		D3D11_TEXTURE2D_DESC BufferDesc;
		ZeroMemory(&BufferDesc, sizeof(BufferDesc));
		BufferDesc.Width = resolutionWidth;
		BufferDesc.Height = resolutionHeight;
		BufferDesc.MipLevels = 1;
		BufferDesc.ArraySize = 1;
		BufferDesc.SampleDesc.Count = swapchain_DESC.SampleDesc.Count;
		BufferDesc.SampleDesc.Quality = swapchain_DESC.SampleDesc.Quality;
		BufferDesc.Format = DXGI_FORMAT_R32_FLOAT;
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		BufferDesc.CPUAccessFlags = 0;
		BufferDesc.MiscFlags = 0;


		theDevicePtr->CreateTexture2D(&BufferDesc, 0, &depthResourcePtr);
		theDevicePtr->CreateRenderTargetView(depthResourcePtr, nullptr, &depthRTVPtr);

		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		theDevicePtr->CreateTexture2D(&BufferDesc, 0, &diffuseResourcePtr);
		GBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		theDevicePtr->CreateShaderResourceView(diffuseResourcePtr, &GBufferDesc, &diffuseSRVPtr);
		theDevicePtr->CreateRenderTargetView(diffuseResourcePtr, nullptr, &diffuseRTVPtr);
		//NormalSRV
		BufferDesc.Format = DXGI_FORMAT_R11G11B10_FLOAT;
		theDevicePtr->CreateTexture2D(&BufferDesc, 0, &normalResourcePtr);
		GBufferDesc.Format = DXGI_FORMAT_R11G11B10_FLOAT;
		theDevicePtr->CreateShaderResourceView(normalResourcePtr, &GBufferDesc, &normalSRVPtr);
		theDevicePtr->CreateRenderTargetView(normalResourcePtr, nullptr, &normalRTVPtr);
		//SpecSRV
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		theDevicePtr->CreateTexture2D(&BufferDesc, 0, &specResourcePtr);
		GBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		theDevicePtr->CreateShaderResourceView(specResourcePtr, &GBufferDesc, &specSRVPtr);
		theDevicePtr->CreateRenderTargetView(specResourcePtr, nullptr, &specRTVPtr);






		ZeroMemory(&theScreenViewport, sizeof(theScreenViewport));
		theScreenViewport.MaxDepth = 1.0f;
		theScreenViewport.MinDepth = 0.0f;
		theScreenViewport.TopLeftX = 0.0f;
		theScreenViewport.TopLeftY = 0.0f;
		theScreenViewport.Width = (float)resolutionWidth;
		theScreenViewport.Height = (float)resolutionHeight;
		theContextPtr->RSSetViewports(1, &theScreenViewport);

		//Load Shaders
		shaderManagerPtr = &ShaderManager::GetRef();
		//Build Constant Buffer
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(cbPerObject);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		theDevicePtr->CreateBuffer(&bd, nullptr, &thePerObjectCBuffer);


		bd.ByteWidth = sizeof(cbPerCamera);
		theDevicePtr->CreateBuffer(&bd, nullptr, &thePerCameraCBuffer);

		//Build Light Constant Buffer
		bd.ByteWidth = sizeof(cbDirLight);
		theDevicePtr->CreateBuffer(&bd, nullptr, &thePerDirLightCBuffer);


		cbDirLight DirLight;
		DirLight.DLightColor = float4{ 1.0f,0.0f,0.0f,1.0f };
		DirLight.lightPos = float4{ 0.0f, 100.0f,0.0f,1.0f };
		D3D11_MAPPED_SUBRESOURCE lightDataMap;
		theContextPtr->Map(thePerDirLightCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &lightDataMap);
		memcpy(lightDataMap.pData, &DirLight, sizeof(cbDirLight));
		theContextPtr->Unmap(thePerDirLightCBuffer, 0);

		theContextPtr->VSSetConstantBuffers(cbDirLight::REGISTER_SLOT, 1, &thePerDirLightCBuffer);
		theContextPtr->PSSetConstantBuffers(cbDirLight::REGISTER_SLOT, 1, &thePerDirLightCBuffer);



		//Set sampler
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
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
		theContextPtr->PSSetSamplers(1, 1, &anisoWrapSampler);

		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		theDevicePtr->CreateSamplerState(&desc, &anisoClampSampler);
		theContextPtr->PSSetSamplers(2, 1, &anisoClampSampler);

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		theDevicePtr->CreateSamplerState(&desc, &pointSampler);
		theContextPtr->PSSetSamplers(0, 1, &pointSampler);

		GBufferPackingContextPtr = new RenderContext;
		GBufferPackingContextPtr->RenderFunc = RenderContext::GBufferPacking;
		GBufferPackingMaterialPtr = new RenderMaterial;


		GBufferUnPackingContextPtr = new RenderContext;
		GBufferUnPackingContextPtr->RenderFunc = RenderContext::GBufferUnpacking;
		GBufferUnPackingShapePtr = new RenderShape;
		GBufferUnPackingMaterialPtr = new RenderMaterial;
		GBufferUnPackingMaterialPtr->RenderFunc = RenderMaterial::GBufferUnpacking;
		GBufferUnPackingShapePtr->RenderFunc = RenderShape::GBufferUnpacking;

		renderShapes.push_back(RenderShape());
		renderShapes.push_back(RenderShape());
		renderShapes.push_back(RenderShape());



		DirectX::XMStoreFloat4x4(&renderShapes[0].worldMatrix, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&renderShapes[1].worldMatrix, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&renderShapes[2].worldMatrix, DirectX::XMMatrixIdentity());


		//Get stream manager class
		streamManagerPtr = &StreamManager::GetRef();
		streamManagerPtr->AddGStream(std::string("Teddy_Idle.AWBX"), renderShapes[0].renderMesh);
		streamManagerPtr->AddGStream(std::string("Teddy_Idle.AWBX"), renderShapes[1].renderMesh);
		streamManagerPtr->AddGStream(std::string("Teddy_Idle.AWBX"), renderShapes[2].renderMesh);




		rSetPtr->AddNode(GBufferUnPackingContextPtr);
		GBufferUnPackingContextPtr->renderSet.AddNode(GBufferUnPackingMaterialPtr);
		GBufferUnPackingMaterialPtr->renderSet.AddNode(GBufferUnPackingShapePtr);

		rSetPtr->AddNode(GBufferPackingContextPtr);
	
		GBufferPackingContextPtr->renderSet.AddNode(GBufferPackingMaterialPtr);
		GBufferPackingMaterialPtr->renderSet.AddNode(&renderShapes[0]);
		GBufferPackingMaterialPtr->AddMaterial(L"Teddy_D.dds");

		



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
		GBufferPackingContextPtr->renderSet.ClearSet();
		GBufferPackingMaterialPtr->renderSet.ClearSet();
		rSetPtr->ClearSet();
		delete GBufferPackingContextPtr;
		delete GBufferPackingMaterialPtr;
		delete rSetPtr;
		streamManagerPtr->DeleteInstance();
		shaderManagerPtr->DeleteInstance();
		IndexBufferManager::DeleteInstance();
		ReleaseCOM(pointSampler);
		ReleaseCOM(anisoWrapSampler);
		ReleaseCOM(anisoClampSampler);
		ReleaseCOM(vertexBuffer);
		ReleaseCOM(thePerObjectCBuffer);
		ReleaseCOM(thePerCameraCBuffer);
		ReleaseCOM(thePerDirLightCBuffer);
		ReleaseCOM(theSwapChainPtr);
		ReleaseCOM(theRenderTargetViewPtr);
		ReleaseCOM(theDepthStencilViewPtr);
		ReleaseCOM(theDepthStencilBufferPtr);
		ReleaseCOM(theContextPtr);


		ReleaseCOM(depthRTVPtr);
		ReleaseCOM(diffuseRTVPtr);
		ReleaseCOM(normalRTVPtr);
		ReleaseCOM(specRTVPtr);
		ReleaseCOM(depthSRVPtr);
		ReleaseCOM(diffuseSRVPtr);
		ReleaseCOM(normalSRVPtr);
		ReleaseCOM(specSRVPtr);
		ReleaseCOM(depthResourcePtr);
		ReleaseCOM(diffuseResourcePtr);
		ReleaseCOM(normalResourcePtr);
		ReleaseCOM(specResourcePtr);




		/*CComPtr<ID3D11Debug> pDebug;
		theDevicePtr->QueryInterface(IID_PPV_ARGS(&pDebug));
		if (pDebug != nullptr)
		{
			pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			pDebug = nullptr;
		}*/
		ReleaseCOM(theDevicePtr);



	}
	void  Renderer::Render(RenderSet &set)
	{

		camera.UpdateView();
		//Build Camera Constant Buffer
		thePerCameraData.gCameraDir = camera.GetForward();
		thePerCameraData.gCameraPos = camera.GetPosition();
		float4x4 proj, viewInv;
		DirectX::XMStoreFloat4x4(&proj, camera.GetProj());
		DirectX::XMStoreFloat4x4(&proj, camera.GetView());
		DirectX::XMStoreFloat4x4(&viewInv, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&viewInv)));
		thePerCameraData.PerspectiveValues.x = 1.0f / proj._11;
		thePerCameraData.PerspectiveValues.y = 1.0f / proj._22;
		thePerCameraData.PerspectiveValues.z = proj._43;
		thePerCameraData.PerspectiveValues.w = proj._33;
		thePerCameraData.ViewInv = viewInv;
		D3D11_MAPPED_SUBRESOURCE cameraDataMap;
		theContextPtr->Map(thePerCameraCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &cameraDataMap);
		memcpy(cameraDataMap.pData, &thePerCameraData, sizeof(cbPerCamera));
		theContextPtr->Unmap(thePerCameraCBuffer, 0);
		theContextPtr->VSSetConstantBuffers(cbPerCamera::REGISTER_SLOT, 1, &thePerCameraCBuffer);
		theContextPtr->PSSetConstantBuffers(cbPerCamera::REGISTER_SLOT, 1, &thePerCameraCBuffer);





		static float rotSpeed = 0.001f;
		DirectX::XMStoreFloat4x4(&renderShapes[0].worldMatrix, DirectX::XMMatrixRotationY(rotSpeed));
		rotSpeed += 0.0001f;
		RenderNode* item = set.GetHead();
		while (item)
		{
			item->RenderProcess();
			item = item->GetNext();
		}




		theRenderCounter++;
	}

	RenderSet& Renderer::GetSet()
	{
		return *(rSetPtr);
	}


	void Renderer::Render(RenderSet &set, RenderFunc renderFuncOverride)
	{
		theRenderCounter++;
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
		DirectX::XMMATRIX vp = DirectX::XMMatrixMultiply(camera.GetView(), camera.GetProj());
		thePerObjectData.gWorld = _world;
		DirectX::XMStoreFloat4x4(&thePerObjectData.gMVP, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&_world), vp));

		D3D11_MAPPED_SUBRESOURCE edit;
		theContextPtr->Map(thePerObjectCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &edit);
		memcpy(edit.pData, &thePerObjectData, sizeof(thePerObjectData));
		theContextPtr->Unmap(thePerObjectCBuffer, 0);

		theContextPtr->VSSetConstantBuffers(cbPerObject::REGISTER_SLOT, 1, &thePerObjectCBuffer);
		theContextPtr->PSSetConstantBuffers(cbPerObject::REGISTER_SLOT, 1, &thePerObjectCBuffer);
	}


	void Renderer::WalkForward()
	{
		camera.WalkForward();
	}
	void Renderer::WalkBackward()
	{
		camera.WalkBackward();
	}
	void Renderer::StafeLeft()
	{
		camera.StafeLeft();
	}
	void Renderer::StafeRight()
	{
		camera.StafeRight();
	}


}

