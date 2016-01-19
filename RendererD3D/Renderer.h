#pragma once

#ifdef RENDERERDLL_EXPORTS
#define RENDERERDLL_API __declspec(dllexport) 
#else
#define RENDERERDLL_API __declspec(dllimport) 
#endif


#include "stdafx.h"
#include "SharedDefines.h"
#include "RenderNode.h"
#include "ConstantBuffer.h"
#include "Camera.h"


#define ReleaseCOM(x) { if(x){ x->Release(); x = nullptr; } }
namespace RendererD3D
{
	class RenderMaterial;
	class RenderShape;
	class RenderContext;
	class RenderSet;
	class InputLayoutManager;
	class ShaderManager;
	class StreamManager;
	class Renderer
	{
		Renderer(void) {};
	public:
		RENDERERDLL_API static Camera camera;
		RENDERERDLL_API static ID3D11Device * theDevicePtr;
		RENDERERDLL_API static ID3D11DeviceContext* theContextPtr;
		RENDERERDLL_API static IDXGISwapChain* theSwapChainPtr;
		RENDERERDLL_API static ID3D11RenderTargetView* theRenderTargetViewPtr;
		RENDERERDLL_API static ID3D11Texture2D* theBackBufferPtr;
		RENDERERDLL_API static ID3D11Texture2D* theDepthStencilBufferPtr;
		RENDERERDLL_API static ID3D11DepthStencilView* theDepthStencilViewPtr;
		RENDERERDLL_API static D3D11_VIEWPORT theScreenViewport;
		static ID3D11Buffer* vertexBuffer;
		static cbPerObject thePerObjectData;
		static cbPerCamera thePerCameraData;
		static ID3D11Buffer *thePerCameraCBuffer;
		static ID3D11Buffer *thePerObjectCBuffer;
		static ID3D11Buffer *thePerDirLightCBuffer;


		//Samplers
		static ID3D11SamplerState* pointSampler;
		static ID3D11SamplerState* anisoClampSampler;
		static ID3D11SamplerState* anisoWrapSampler;

		//Gbuffer
		RENDERERDLL_API static ID3D11RenderTargetView* depthRTVPtr;
		RENDERERDLL_API static ID3D11RenderTargetView* diffuseRTVPtr;
		RENDERERDLL_API static ID3D11RenderTargetView* normalRTVPtr;
		RENDERERDLL_API static ID3D11RenderTargetView* specRTVPtr;
		RENDERERDLL_API static  ID3D11ShaderResourceView* depthSRVPtr;
		RENDERERDLL_API static  ID3D11ShaderResourceView* diffuseSRVPtr;
		RENDERERDLL_API static  ID3D11ShaderResourceView* normalSRVPtr;
		RENDERERDLL_API static  ID3D11ShaderResourceView* specSRVPtr;
		RENDERERDLL_API static ID3D11Texture2D* depthResourcePtr;
		RENDERERDLL_API static ID3D11Texture2D* diffuseResourcePtr;
		RENDERERDLL_API static ID3D11Texture2D* normalResourcePtr;
		RENDERERDLL_API static ID3D11Texture2D* specResourcePtr;

		RENDERERDLL_API static Renderer& GetRef();

		RENDERERDLL_API inline static  UINT GetRenderNumber(void) { return theRenderCounter; }
		RENDERERDLL_API inline static  void IncrementRenderCounter(void) { ++theRenderCounter; }
		inline static  void ClearRenderTarget(const FLOAT clearColor[4])
		{
			theContextPtr->ClearRenderTargetView(theRenderTargetViewPtr, clearColor);
			theContextPtr->ClearRenderTargetView(depthRTVPtr, clearColor);
			theContextPtr->ClearRenderTargetView(diffuseRTVPtr, clearColor);
			theContextPtr->ClearRenderTargetView(normalRTVPtr, clearColor);
			theContextPtr->ClearRenderTargetView(specRTVPtr, clearColor);
		}
		RENDERERDLL_API inline static  void ClearDepthAndStencilTarget(
			UINT clearFlags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, FLOAT depth = 1.0f,
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
		static  ID3D11ShaderResourceView *GetDepthSRV();

		static void SetPerObjectData(float4x4& _world);


		static RenderContext*		cubeContextPtr;
		static RenderShape*		cubeShapePtr;
		static RenderMaterial*	cubeMaterialPtr;
		static RenderSet* rSetPtr;
		RENDERERDLL_API  RenderSet& GetSet();
		RENDERERDLL_API static void SwitchTo0();
		RENDERERDLL_API static void SwitchTo1();
		RENDERERDLL_API static void SwitchTo2();

	private:

		static UINT theRenderCounter;
		static UINT resolutionWidth;
		static UINT resolutionHeight;
		static ID3D11ShaderResourceView *theDepthStencilSRVPtr;
		static ShaderManager* shaderManagerPtr;
		static ID3D11ShaderResourceView* cubeSRV;
		static StreamManager* streamManagerPtr;
		static std::vector<RenderShape> renderShapes;

	};




}
