#include "stdafx.h"
#include "GameEngine.h"

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
	
	inputManager.SetKeyPressed(Input::IM_K, TOGGLE_WIREFRAME);
	inputManager.SetKeyHeld(Input::IM_W, FORWARD);
	inputManager.SetKeyHeld(Input::IM_S, BACKWARD);
	inputManager.SetKeyHeld(Input::IM_A, LEFT);
	inputManager.SetKeyHeld(Input::IM_D, RIGHT);
	MessageManager::GetInstance()->AddProcessMessageFunc(MessageProc);
	ShowCursor(false);
}

void GameEngine::Update(float _deltaTime)
{
	deltaTime = _deltaTime;
	
	renderer.camera.SetDeltaTime(_deltaTime);
	inputManager.Update();
	MessageManager::GetInstance()->Update();

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

	renderer.Update();

	
}


void GameEngine::Render()
{
	renderer.ClearRenderTargets(clearColor);
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

void GameEngine::MessageProc(Messages mess)
{
	switch (mess)
	{
	case NO_MESSAGE:
		break;
	case LEFT:
		RendererD3D::Renderer::StafeLeft();
		break;
	case RIGHT:
		RendererD3D::Renderer::StafeRight();
		break;
	case FORWARD:
		RendererD3D::Renderer::WalkForward();
		break;
	case BACKWARD:
		RendererD3D::Renderer::WalkBackward();
		break;
	case TOGGLE_WIREFRAME:
		RendererD3D::RenderContext::ToggleWireFrame();
		break;
	default:
		std::cout << "UNKNOWN MESSAGE TYPE";
		break;
	}
}
