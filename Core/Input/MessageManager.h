#pragma once
#include "stdafx.h"
#include <vector>
#include <queue>

#ifdef MESSAGEMANAGERDLL_EXPORTS
#define MESSAGEMANAGERDLL  __declspec(dllexport) 
#else
#define MESSAGEMANAGERDLL  __declspec(dllimport) 
#endif

enum Messages {NO_MESSAGE = -1, A_PRESSED, A_RELEASED, A_CHANGE, LEFT, RIGHT, UP, DOWN, END, TOGGLE_WIREFRAME, FORWARD, BACKWARD};

class MessageManager
{
public:
	MESSAGEMANAGERDLL static MessageManager* GetInstance(void);
	MESSAGEMANAGERDLL static void DeleteInstance(void);

	MESSAGEMANAGERDLL MessageManager();
	MESSAGEMANAGERDLL ~MessageManager();

	MESSAGEMANAGERDLL void AddMessage(Messages mess);
	MESSAGEMANAGERDLL void AddProcessMessageFunc(void(*func)(Messages));
	MESSAGEMANAGERDLL void Update();

private:
	//SINGLETON
	static MessageManager* instance;

	std::queue<Messages> messages;
	std::vector<void(*)(Messages)> processMessageFuncs;
};

