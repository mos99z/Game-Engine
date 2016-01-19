#pragma once
#include <Renderer.h>
#include <RenderContext.h>
#include <InputManager.h>

class GameEngine
{
	HWND mainGameWindowHWND;
	bool GameEnd = false;
	RendererD3D::Renderer& renderer = RendererD3D::Renderer::GetRef();;
	Input::InputManager inputManager;
	unsigned int mainGameWindowWidth = 1600;
	unsigned int mainGameWindowHeight = 1024;
	float deltaTime = 1.0f/60.0f;
	FLOAT clearColor[4]{ 0.0f,0.0f,0.0f,1.0f };
public:
	inline bool IsOver() { return GameEnd; }
	inline void GameOver() { GameEnd = true; }
	GameEngine();
	~GameEngine();
	void SetResolution(unsigned int _width,unsigned int _height);
	void Initialize(HWND _windowHWND);
	void ShutDown();
	void Update();
	void Render();

};

