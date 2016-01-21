#include "stdafx.h"
#include "GameEngine.h"
#include <chrono>
#include <Camera.h>

GameEngine::GameEngine()
{

}


GameEngine::~GameEngine()
{
}


void  GameEngine::Initialize(HWND _windowHWND)
{
	mainGameWindowHWND = _windowHWND;
	renderer.Initialize(mainGameWindowHWND, mainGameWindowWidth, mainGameWindowHeight);
	Input::RawKey temp;
	temp.keyCode = Input::IM_W;
	temp.KeyHeld = RendererD3D::Renderer::WalkForward;
	temp.KeyPressed = nullptr;
	temp.KeyReleased = RendererD3D::Renderer::WalkForward;
	std::vector<Input::RawKey> vec;
	vec.push_back(temp);
	inputManager.LoadKeyBoard(vec);
	inputManager.SetKeyPressed(Input::IM_K, RendererD3D::RenderContext::ToggleWireFrame);
	inputManager.SetKeyPressed(Input::IM_1, RendererD3D::Renderer::SwitchTo0);
	inputManager.SetKeyPressed(Input::IM_2, RendererD3D::Renderer::SwitchTo1);
	inputManager.SetKeyPressed(Input::IM_3, RendererD3D::Renderer::SwitchTo2);
	//inputManager.SetKeyHeld(Input::IM_W,RendererD3D::Renderer::WalkForward);
	inputManager.SetKeyHeld(Input::IM_S,RendererD3D::Renderer::WalkBackward);
	inputManager.SetKeyHeld(Input::IM_A,RendererD3D::Renderer::StafeLeft);
	inputManager.SetKeyHeld(Input::IM_D,RendererD3D::Renderer::StafeRight);
}

void GameEngine::Update()
{
	using namespace std::chrono;
	auto beginTick = high_resolution_clock::now();
	inputManager.Update();
	renderer.camera.UpdateView();
	renderer.camera.SetDeltaTime(deltaTime);
	auto endTick = high_resolution_clock::now();
	deltaTime = duration_cast<duration<float>>(endTick - beginTick).count();
}


void GameEngine::Render()
{
	renderer.ClearRenderTarget(clearColor);
	renderer.ClearDepthAndStencilTarget();
	renderer.Render(renderer.GetSet());
	renderer.Present();
}

void  GameEngine::SetResolution(unsigned int _width, unsigned int _height)
{
	renderer.SetResolution(_width, _height);
}


void  GameEngine::ShutDown()
{
	renderer.Shutdown();
}