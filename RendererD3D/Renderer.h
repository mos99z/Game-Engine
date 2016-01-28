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


	/*!
	Renderer Class is the core of the rendering system.
	It is responsible for initialize DirectX 11.0 cores and pass the rendering data along the pipeline.
	Renderer will be a dynamic library to be utilized by other ends.
	*/
	class Renderer
	{
		Renderer(void) {};
	public:

		/** \name D3D11 core members
		*/
		//@{ 
		static ID3D11Device * theDevicePtr;
		static ID3D11DeviceContext* theContextPtr;
		static IDXGISwapChain* theSwapChainPtr;
		//@}
		static ID3D11RenderTargetView* theRenderTargetViewPtr;
		static ID3D11Texture2D* theBackBufferPtr;
		static ID3D11Texture2D* theDepthStencilBufferPtr;
		static ID3D11DepthStencilView* theDepthStencilViewPtr;
		static D3D11_VIEWPORT theScreenViewport;
		static ID3D11Buffer* vertexBuffer;
		static cbPerObject thePerObjectData;
		static cbPerCamera thePerCameraData;
		static ID3D11Buffer *thePerCameraCBuffer;
		static ID3D11Buffer *thePerObjectCBuffer;
		static ID3D11Buffer *thePerDirLightCBuffer;
	


		//! Camera
		RENDERERDLL_API static Camera camera;

		/** \name Samplers
		*/
		//@{ 
		static ID3D11SamplerState* pointSampler;
		static ID3D11SamplerState* anisoClampSampler;
		static ID3D11SamplerState* anisoWrapSampler;
		//@}

		/** \name Gbuffer
		* RTVs and SRVs
		*/
		//@{ 
		static ID3D11RenderTargetView* diffuseRTVPtr;
		static ID3D11RenderTargetView* normalRTVPtr;
		static ID3D11RenderTargetView* specRTVPtr;
		static  ID3D11ShaderResourceView* depthSRVPtr;
		static  ID3D11ShaderResourceView* diffuseSRVPtr;
		static  ID3D11ShaderResourceView* normalSRVPtr;
		static  ID3D11ShaderResourceView* specSRVPtr;
		//@}

		static RenderContext*	GBufferPackingContextPtr;
		static RenderMaterial*	GBufferPackingMaterialPtr;
		static RenderContext*	GBufferUnPackingContextPtr;
		static RenderShape*		GBufferUnPackingShapePtr;
		static RenderMaterial*	GBufferUnPackingMaterialPtr;
		static RenderSet* rSetPtr;


		//! Get the singleton instance of Renderer
		RENDERERDLL_API static Renderer& GetRef();

		RENDERERDLL_API inline static  UINT GetRenderNumber(void) { return theRenderCounter; }
		RENDERERDLL_API inline static  void IncrementRenderCounter(void) { ++theRenderCounter; }

		///
		/// \param clearColor[4] the color to clear all render targets {Backbuffer,diffuse, normal, specular}
		
		RENDERERDLL_API inline static  void ClearRenderTargets(const FLOAT clearColor[4])
		{
			theContextPtr->ClearRenderTargetView(theRenderTargetViewPtr, clearColor);
			theContextPtr->ClearRenderTargetView(diffuseRTVPtr, clearColor);
			theContextPtr->ClearRenderTargetView(normalRTVPtr, clearColor);
			theContextPtr->ClearRenderTargetView(specRTVPtr, clearColor);
		}

		/// Clear depth and stencil buffer
		/// \param clearFlags to specify clearing depth or stencil or both.
		/// \param depth Depth value to clear
		/// \param stencil stencil value to clear
		RENDERERDLL_API inline static  void ClearDepthAndStencilTarget(
			UINT clearFlags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, FLOAT depth = 1.0f,
			UINT8 stencil = 0)
		{
			theContextPtr->ClearDepthStencilView(theDepthStencilViewPtr, clearFlags, depth, stencil);
		}

		/// Swap backbuffer with frontbuffer
		RENDERERDLL_API inline static  void Present(UINT syncInterval = 0, UINT flags = 0)
		{
			theSwapChainPtr->Present(syncInterval, flags);
		}


		/// To initialize D3D11 pipeline and setup necessary members
		/// \param hWnd the window handle D3D will be working with
		RENDERERDLL_API static  void Initialize(HWND hWnd, UINT resWidth, UINT resHeight);
		/// Set resolution and resize buffers to match with new resolution
		RENDERERDLL_API static  void SetResolution(UINT _width, UINT _height);

		/// Release all D3D resources and free heap allocated memory 
		RENDERERDLL_API static  void Shutdown();

		/// Call renderFunc on each item in the set
		/// \param set a RenderSet that contains either RenderContext, RenderMaterial or RenderShape.
		RENDERERDLL_API static  void Render(RenderSet &set);
		RENDERERDLL_API static  void Render(RenderSet &set, RenderFunc renderFuncOverride);

		
		RENDERERDLL_API static  ID3D11ShaderResourceView *GetDepthSRV();
		RENDERERDLL_API inline static  UINT GetResolutionWidth() { return resolutionWidth; }
		RENDERERDLL_API inline static  UINT GetResolutionHeight() { return resolutionHeight; }


		
		static void SetPerObjectData(float4x4& _world);
		RENDERERDLL_API  RenderSet& GetSet();

		/// \name Camera Movement wrappers
		//@{
		RENDERERDLL_API static void WalkForward();
		RENDERERDLL_API static void WalkBackward();
		RENDERERDLL_API static void StafeLeft();
		RENDERERDLL_API static void StafeRight();
		//@}


	private:
		static UINT theRenderCounter;
		static UINT resolutionWidth;
		static UINT resolutionHeight;
		static ID3D11ShaderResourceView *theDepthStencilSRVPtr;
		static ShaderManager* shaderManagerPtr;
		static StreamManager* streamManagerPtr;
		static std::vector<RenderShape> renderShapes;

		static  void ResizeBuffers();
	};




}
