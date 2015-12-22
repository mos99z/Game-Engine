#include "stdafx.h"
#include "Renderer.h"


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

	void Renderer::Initialize(HWND hWnd, UINT resWidth, UINT resHeight)
	{

		resolutionHeight = resHeight;
		resolutionWidth = resWidth;

		DXGI_SWAP_CHAIN_DESC swapchain_DESC;
		ZeroMemory(&swapchain_DESC, sizeof(swapchain_DESC));
		swapchain_DESC.BufferCount = 1;
		swapchain_DESC.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchain_DESC.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchain_DESC.OutputWindow = hWnd;
		swapchain_DESC.SampleDesc.Count = 1;
		swapchain_DESC.Windowed = true;
		swapchain_DESC.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


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
	}

	void  Renderer::SetResolution(UINT _width, UINT _height)
	{


	}

	void  Renderer::Shutdown()
	{
		ReleaseCOM(theSwapChainPtr);
		ReleaseCOM(theRenderTargetViewPtr);
		ReleaseCOM(theDepthStencilViewPtr);
		ReleaseCOM(theDepthStencilBufferPtr);
		ReleaseCOM(theContextPtr);
		ReleaseCOM(theDevicePtr);
	}
	void  Renderer::Render()
	{
		float cle[4] { 1.0f, 0.2f, 0.4f, 1.0f };
		theContextPtr->ClearRenderTargetView(theRenderTargetViewPtr, cle);
		theSwapChainPtr->Present(0, 0);
	}
	void  Renderer::ResizeBuffers()
	{

	}
	ID3D11ShaderResourceView * Renderer::GetDepthSRV()
	{
		return nullptr;
	}
}