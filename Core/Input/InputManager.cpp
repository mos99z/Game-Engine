#include "InputManager.h"
#include <iostream>

namespace Input
{
	InputManager::InputManager()
	{
		
	}

	InputManager::~InputManager()
	{
		Clear();
		keyboard.clear();
	}

	void InputManager::Clear()
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.begin();
		for (; key != keyboard.end(); key++)
			delete key->second;
	}

	void InputManager::LoadKeyBoard(std::vector<RawKey> board)
	{
		Clear();
		keyboard.clear();
		for (size_t i = 0; i < board.size(); i++)
		{
			Key* temp = new Key();
			temp->KeyPressed = board[i].KeyPressed;
			temp->KeyHeld = board[i].KeyHeld;
			temp->KeyReleased = board[i].KeyReleased;
			keyboard[board[i].keyCode] = temp;
		}

		DisableAll();
	}

	void InputManager::Update()
	{
		UpdateKeyboard();
		KeyUpdates();
	}
	
	void InputManager::UpdateKeyboard()
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.begin();
		for (; key != keyboard.end(); key++)
		{
			if (GetAsyncKeyState(key->first))
			{
				key->second->prevState = key->second->currState;
				key->second->currState = PRESSED;
				if (key->second->prevState == PRESSED || key->second->prevState == HELD)
					key->second->currState = HELD;
			}

			else
			{
				key->second->prevState = key->second->currState;
				key->second->currState = RELEASED;
				if (key->second->prevState == RELEASED || key->second->prevState == DISABLED)
					key->second->currState = DISABLED;
			}
		}
	}

	void InputManager::DisableAll()
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.begin();
		for (; key != keyboard.end(); key++)
		{
			key->second->prevState = DISABLED;
			key->second->currState = DISABLED;
		}
	}

	void InputManager::SetKeyPressed(int keycode, Messages mess)//void(*function)())
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.find(keycode);

		if (key != keyboard.end())
			keyboard[keycode]->KeyPressed = mess;//function;

		else
		{
			Key* temp = new Key();
			temp->prevState = DISABLED;
			temp->currState = DISABLED;
			temp->KeyPressed = mess;//function;
			temp->KeyHeld = NO_MESSAGE;// nullptr;
			temp->KeyReleased = NO_MESSAGE;// nullptr;
			keyboard[keycode] = temp;
		}
	}

	void InputManager::SetKeyHeld(int keycode, Messages mess)//void(*function)())
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.find(keycode);

		if (key != keyboard.end())
			keyboard[keycode]->KeyHeld = mess;//function;

		else
		{
			Key* temp = new Key();
			temp->prevState = DISABLED;
			temp->currState = DISABLED;
			temp->KeyPressed = NO_MESSAGE;// nullptr;
			temp->KeyHeld = mess;//function;
			temp->KeyReleased = NO_MESSAGE;// nullptr;
			keyboard[keycode] = temp;
		}
	}

	void InputManager::SetKeyReleased(int keycode, Messages mess)//void(*function)())
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.find(keycode);

		if (key != keyboard.end())
			keyboard[keycode]->KeyReleased = mess;//function;

		else
		{
			Key* temp = new Key();
			temp->prevState = DISABLED;
			temp->currState = DISABLED;
			temp->KeyPressed = NO_MESSAGE;// nullptr;
			temp->KeyHeld = NO_MESSAGE;// nullptr;
			temp->KeyReleased = mess;//function;
			keyboard[keycode] = temp;
		}
	}

	void InputManager::RemoveKey(int keycode)
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.find(keycode);

		if (key != keyboard.end())
		{
			delete key->second;
			keyboard.erase(keycode);
		}
	}

	void InputManager::KeyUpdates()
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.begin();
		for (; key != keyboard.end(); key++)
		{
			switch (key->second->currState)
			{
			case PRESSED:
				//if (key->second->KeyPressed != nullptr)
					MessageManager::GetInstance()->AddMessage(key->second->KeyPressed);
				break;
			case HELD:
				//if (key->second->KeyHeld != nullptr)
				MessageManager::GetInstance()->AddMessage(key->second->KeyHeld);
				break;
			case RELEASED:
				//if (key->second->KeyReleased != nullptr)
				MessageManager::GetInstance()->AddMessage(key->second->KeyReleased);
				break;
			default:
				break;
			}					
		}
	}

	void InputManager::NullAllFunctionPointers()
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.begin();
		for (; key != keyboard.end(); key++)
		{
			key->second->KeyPressed = NO_MESSAGE;// nullptr;
			key->second->KeyHeld = NO_MESSAGE;// nullptr;
			key->second->KeyReleased = NO_MESSAGE;// nullptr;
		}
	}
}



