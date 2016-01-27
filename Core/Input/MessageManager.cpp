#include "MessageManager.h"

MessageManager* MessageManager::instance = nullptr;

MessageManager* MessageManager::GetInstance(void)
{
	if (instance == nullptr)
		instance = new MessageManager();
	
	return instance;
}

void MessageManager::DeleteInstance(void)
{
	delete instance;
	instance = nullptr;
}

MessageManager::MessageManager()
{
	
}

MessageManager::~MessageManager()
{
	DeleteInstance();
}

void MessageManager::AddMessage(Messages mess)
{
	messages.push(mess);
}

void MessageManager::AddProcessMessageFunc(void(*func)(Messages))
{
	processMessageFuncs.push_back(func);
}

void MessageManager::Update()
{
	while (!messages.empty())
	{
		for (size_t i = 0; i < processMessageFuncs.size(); i++)
			processMessageFuncs[i](messages.front());
		
		messages.pop();
	}
}
