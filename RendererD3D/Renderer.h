#ifdef RENDERERDLL_EXPORTS
#define RENDERERDLL_API __declspec(dllexport) 
#else
#define RENDERERDLL_API __declspec(dllimport) 
#endif

#pragma once
#include "SharedDefines.h"
#include "RenderNode.h"




namespace RendererD3D
{
#define ReleaseCOM(x) { if(x){ x->Release(); x = nullptr; } }
	class RenderSet;
	class Renderer
	{

	public:
		RENDERERDLL_API static ID3D11Device * theDevicePtr;
		RENDERERDLL_API static ID3D11DeviceContext* theContextPtr;
		RENDERERDLL_API static IDXGISwapChain* theSwapChainPtr;
		RENDERERDLL_API static ID3D11RenderTargetView* theRenderTargetViewPtr;
		RENDERERDLL_API static ID3D11Texture2D* theBackBufferPtr;
		RENDERERDLL_API static ID3D11Texture2D* theDepthStencilBufferPtr;
		RENDERERDLL_API static ID3D11DepthStencilView* theDepthStencilViewPtr;
		RENDERERDLL_API static D3D11_VIEWPORT theScreenViewport;

		Renderer(void) {}
		~Renderer(void) {}




		RENDERERDLL_API inline static  UINT GetRenderNumber(void) { return theRenderCounter; }
		RENDERERDLL_API inline static  void IncrementRenderCounter(void) { ++theRenderCounter; }
		RENDERERDLL_API inline static  void ClearRenderTarget(const FLOAT clearColor[4])
		{
			theContextPtr->ClearRenderTargetView(theRenderTargetViewPtr, clearColor);
		}
		 RENDERERDLL_API inline static  void ClearDepthAndStencilTarget(
			UINT clearFlags = D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, FLOAT depth = 1.0f,
			UINT8 stencil = 0)
		{
			theContextPtr->ClearDepthStencilView(theDepthStencilViewPtr, clearFlags, depth, stencil);
		}
		 RENDERERDLL_API inline static  void Present(UINT syncInterval = 0, UINT flags = 0)
		{
			theSwapChainPtr->Present(syncInterval, flags);
		}
		 RENDERERDLL_API inline static  UINT GetResolutionWidth() { return resolutionWidth; }
		 RENDERERDLL_API inline static  UINT GetResolutionHeight() { return resolutionHeight; }


		 RENDERERDLL_API static  void Initialize(HWND hWnd, UINT resWidth, UINT resHeight);
		 RENDERERDLL_API static  void SetResolution(UINT _width, UINT _height);
		 RENDERERDLL_API static  void Shutdown();
		 RENDERERDLL_API static  void Render(RenderSet &set);
		 RENDERERDLL_API static  void Render(RenderSet &set, RenderFunc renderFuncOverride);
		 RENDERERDLL_API static  void ResizeBuffers();
		 RENDERERDLL_API static  ID3D11ShaderResourceView *GetDepthSRV();

	private:
		static UINT theRenderCounter;
		static UINT resolutionWidth;
		static UINT resolutionHeight;
		static ID3D11ShaderResourceView *theDepthStencilSRVPtr;
	};
}
