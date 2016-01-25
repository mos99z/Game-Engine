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
	//Input::RawKey temp;
	//temp.keyCode = Input::IM_W;
	//temp.KeyHeld = RendererD3D::Renderer::WalkForward;
	//temp.KeyPressed = nullptr;
	//temp.KeyReleased = nullptr;
	//std::vector<Input::RawKey> vec;
	//vec.push_back(temp);
	//inputManager.LoadKeyBoard(vec);
	inputManager.SetKeyPressed(Input::IM_K, RendererD3D::RenderContext::ToggleWireFrame);
	inputManager.SetKeyHeld(Input::IM_W,RendererD3D::Renderer::WalkForward);
	inputManager.SetKeyHeld(Input::IM_S, RendererD3D::Renderer::WalkBackward);
	inputManager.SetKeyHeld(Input::IM_A, RendererD3D::Renderer::StafeLeft);
	inputManager.SetKeyHeld(Input::IM_D, RendererD3D::Renderer::StafeRight);
	ShowCursor(false);
}

void GameEngine::Update()
{
	using namespace std::chrono;
	auto beginTick = high_resolution_clock::now();
	inputManager.Update();

	RECT clientWindow;
	GetWindowRect(mainGameWindowHWND, &clientWindow);
	POINT center;
	center.x = (clientWindow.right + clientWindow.left) /2;
	center.y = (clientWindow.bottom + clientWindow.top) /2;
	
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		renderer.camera.Pitch(-(cursorPos.y - center.y)*0.15f);
		renderer.camera.RotateY((cursorPos.x - center.x)*0.15f);
		SetCursorPos(center.x, center.y);
	}


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