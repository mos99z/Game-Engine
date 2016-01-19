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

		ReleaseAll();
	}

	//SLOW UPDATE
	/*void InputManager::SlowUpdate()
	{
		SlowUpdateKeyboard();
		KeyUpdates();
	}*/

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
			if (GetAsyncKeyState(key->first) & 0x1)
			{
				PressKey(key->first);
				//Sleep(200);
			}
			else
				ReleaseKey(key->first);
		}
	}

	//SLOW UPDATE KEYBOARD
	/*void InputManager::SlowUpdateKeyboard()
	{		
		if (timer < SLOW_TIMER_WAIT)
		{
			timer++;
			ReleaseAll();
			return;
		}

		timer = 0;

		if (GetAsyncKeyState(IM_0))
			PressKey(IM_0);
		else
			ReleaseKey(IM_0);

		if (GetAsyncKeyState(IM_1))
			PressKey(IM_1);
		else
			ReleaseKey(IM_1);

		if (GetAsyncKeyState(IM_2))
			PressKey(IM_2);
		else
			ReleaseKey(IM_2);

		if (GetAsyncKeyState(IM_3))
			PressKey(IM_3);
		else
			ReleaseKey(IM_3);

		if (GetAsyncKeyState(IM_4))
			PressKey(IM_4);
		else
			ReleaseKey(IM_4);

		if (GetAsyncKeyState(IM_5))
			PressKey(IM_5);
		else
			ReleaseKey(IM_5);

		if (GetAsyncKeyState(IM_6))
			PressKey(IM_6);
		else
			ReleaseKey(IM_6);

		if (GetAsyncKeyState(IM_7))
			PressKey(IM_7);
		else
			ReleaseKey(IM_7);

		if (GetAsyncKeyState(IM_8))
			PressKey(IM_8);
		else
			ReleaseKey(IM_8);

		if (GetAsyncKeyState(IM_9))
			PressKey(IM_9);
		else
			ReleaseKey(IM_9);

		if (GetAsyncKeyState(IM_A))
			PressKey(IM_A);
		else
			ReleaseKey(IM_A);

		if (GetAsyncKeyState(IM_B))
			PressKey(IM_B);
		else
			ReleaseKey(IM_B);

		if (GetAsyncKeyState(IM_C))
			PressKey(IM_C);
		else
			ReleaseKey(IM_C);

		if (GetAsyncKeyState(IM_D))
			PressKey(IM_D);
		else
			ReleaseKey(IM_D);

		if (GetAsyncKeyState(IM_E))
			PressKey(IM_E);
		else
			ReleaseKey(IM_E);

		if (GetAsyncKeyState(IM_F))
			PressKey(IM_F);
		else
			ReleaseKey(IM_F);

		if (GetAsyncKeyState(IM_G))
			PressKey(IM_G);
		else
			ReleaseKey(IM_G);

		if (GetAsyncKeyState(IM_H))
			PressKey(IM_H);
		else
			ReleaseKey(IM_H);

		if (GetAsyncKeyState(IM_I))
			PressKey(IM_I);
		else
			ReleaseKey(IM_I);

		if (GetAsyncKeyState(IM_J))
			PressKey(IM_J);
		else
			ReleaseKey(IM_J);

		if (GetAsyncKeyState(IM_K))
			PressKey(IM_K);
		else
			ReleaseKey(IM_K);

		if (GetAsyncKeyState(IM_L))
			PressKey(IM_L);
		else
			ReleaseKey(IM_L);

		if (GetAsyncKeyState(IM_M))
			PressKey(IM_M);
		else
			ReleaseKey(IM_M);

		if (GetAsyncKeyState(IM_N))
			PressKey(IM_N);
		else
			ReleaseKey(IM_N);

		if (GetAsyncKeyState(IM_O))
			PressKey(IM_O);
		else
			ReleaseKey(IM_O);

		if (GetAsyncKeyState(IM_P))
			PressKey(IM_P);
		else
			ReleaseKey(IM_P);

		if (GetAsyncKeyState(IM_Q))
			PressKey(IM_Q);
		else
			ReleaseKey(IM_Q);

		if (GetAsyncKeyState(IM_R))
			PressKey(IM_R);
		else
			ReleaseKey(IM_R);

		if (GetAsyncKeyState(IM_S))
			PressKey(IM_S);
		else
			ReleaseKey(IM_S);

		if (GetAsyncKeyState(IM_T))
			PressKey(IM_T);
		else
			ReleaseKey(IM_T);

		if (GetAsyncKeyState(IM_U))
			PressKey(IM_U);
		else
			ReleaseKey(IM_U);

		if (GetAsyncKeyState(IM_V))
			PressKey(IM_V);
		else
			ReleaseKey(IM_V);

		if (GetAsyncKeyState(IM_W))
			PressKey(IM_W);
		else
			ReleaseKey(IM_W);

		if (GetAsyncKeyState(IM_X))
			PressKey(IM_X);
		else
			ReleaseKey(IM_X);

		if (GetAsyncKeyState(IM_Y))
			PressKey(IM_Y);
		else
			ReleaseKey(IM_Y);

		if (GetAsyncKeyState(IM_Z))
			PressKey(IM_Z);
		else
			ReleaseKey(IM_Z);

		if (GetAsyncKeyState(VK_NUMLOCK))
			PressKey(VK_NUMLOCK);
		else
			ReleaseKey(VK_NUMLOCK);

		if (GetAsyncKeyState(VK_NUMPAD0))
			PressKey(VK_NUMPAD0);
		else
			ReleaseKey(VK_NUMPAD0);

		if (GetAsyncKeyState(VK_NUMPAD1))
			PressKey(VK_NUMPAD1);
		else
			ReleaseKey(VK_NUMPAD1);

		if (GetAsyncKeyState(VK_NUMPAD2))
			PressKey(VK_NUMPAD2);
		else
			ReleaseKey(VK_NUMPAD2);

		if (GetAsyncKeyState(VK_NUMPAD3))
			PressKey(VK_NUMPAD3);
		else
			ReleaseKey(VK_NUMPAD3);

		if (GetAsyncKeyState(VK_NUMPAD4))
			PressKey(VK_NUMPAD4);
		else
			ReleaseKey(VK_NUMPAD4);

		if (GetAsyncKeyState(VK_NUMPAD5))
			PressKey(VK_NUMPAD5);
		else
			ReleaseKey(VK_NUMPAD5);

		if (GetAsyncKeyState(VK_NUMPAD6))
			PressKey(VK_NUMPAD6);
		else
			ReleaseKey(VK_NUMPAD6);

		if (GetAsyncKeyState(VK_NUMPAD7))
			PressKey(VK_NUMPAD7);
		else
			ReleaseKey(VK_NUMPAD7);

		if (GetAsyncKeyState(VK_NUMPAD8))
			PressKey(VK_NUMPAD8);
		else
			ReleaseKey(VK_NUMPAD8);

		if (GetAsyncKeyState(VK_NUMPAD9))
			PressKey(VK_NUMPAD9);
		else
			ReleaseKey(VK_NUMPAD9);

		if (GetAsyncKeyState(VK_ADD))
			PressKey(VK_ADD);
		else
			ReleaseKey(VK_ADD);

		if (GetAsyncKeyState(VK_SUBTRACT))
			PressKey(VK_SUBTRACT);
		else
			ReleaseKey(VK_SUBTRACT);

		if (GetAsyncKeyState(VK_DIVIDE))
			PressKey(VK_DIVIDE);
		else
			ReleaseKey(VK_DIVIDE);

		if (GetAsyncKeyState(VK_MULTIPLY))
			PressKey(VK_MULTIPLY);
		else
			ReleaseKey(VK_MULTIPLY);

		if (GetAsyncKeyState(VK_DECIMAL))
			PressKey(VK_DECIMAL);
		else
			ReleaseKey(VK_DECIMAL);

		if (GetAsyncKeyState(VK_BACK))
			PressKey(VK_BACK);
		else
			ReleaseKey(VK_BACK);

		if (GetAsyncKeyState(VK_TAB))
			PressKey(VK_TAB);
		else
			ReleaseKey(VK_TAB);

		if (GetAsyncKeyState(VK_CANCEL))
			PressKey(VK_CANCEL);
		else
			ReleaseKey(VK_CANCEL);

		if (GetAsyncKeyState(VK_CLEAR))
			PressKey(VK_CLEAR);
		else
			ReleaseKey(VK_CLEAR);

		if (GetAsyncKeyState(VK_RETURN))
			PressKey(VK_RETURN);
		else
			ReleaseKey(VK_RETURN);

		if (GetAsyncKeyState(VK_CAPITAL))
			PressKey(VK_CAPITAL);
		else
			ReleaseKey(VK_CAPITAL);

		if (GetAsyncKeyState(VK_ESCAPE))
			PressKey(VK_ESCAPE);
		else
			ReleaseKey(VK_ESCAPE);

		if (GetAsyncKeyState(VK_SPACE))
			PressKey(VK_SPACE);
		else
			ReleaseKey(VK_SPACE);

		if (GetAsyncKeyState(VK_PRIOR))
			PressKey(VK_PRIOR);
		else
			ReleaseKey(VK_PRIOR);

		if (GetAsyncKeyState(VK_NEXT))
			PressKey(VK_NEXT);
		else
			ReleaseKey(VK_NEXT);

		if (GetAsyncKeyState(VK_PAUSE))
			PressKey(VK_PAUSE);
		else
			ReleaseKey(VK_PAUSE);

		if (GetAsyncKeyState(VK_END))
			PressKey(VK_END);
		else
			ReleaseKey(VK_END);

		if (GetAsyncKeyState(VK_HOME))
			PressKey(VK_HOME);
		else
			ReleaseKey(VK_HOME);

		if (GetAsyncKeyState(VK_LEFT))
			PressKey(VK_LEFT);
		else
			ReleaseKey(VK_LEFT);

		if (GetAsyncKeyState(VK_UP))
			PressKey(VK_UP);
		else
			ReleaseKey(VK_UP);

		if (GetAsyncKeyState(VK_RIGHT))
			PressKey(VK_RIGHT);
		else
			ReleaseKey(VK_RIGHT);

		if (GetAsyncKeyState(VK_DOWN))
			PressKey(VK_DOWN);
		else
			ReleaseKey(VK_DOWN);

		if (GetAsyncKeyState(VK_SELECT))
			PressKey(VK_SELECT);
		else
			ReleaseKey(VK_SELECT);

		if (GetAsyncKeyState(VK_DELETE))
			PressKey(VK_DELETE);
		else
			ReleaseKey(VK_DELETE);

		if (GetAsyncKeyState(VK_LSHIFT))
			PressKey(VK_LSHIFT);
		else
			ReleaseKey(VK_LSHIFT);

		if (GetAsyncKeyState(VK_RSHIFT))
			PressKey(VK_RSHIFT);
		else
			ReleaseKey(VK_RSHIFT);

		if (GetAsyncKeyState(VK_LWIN))
			PressKey(VK_LWIN);
		else
			ReleaseKey(VK_LWIN);

		if (GetAsyncKeyState(VK_RWIN))
			PressKey(VK_RWIN);
		else
			ReleaseKey(VK_RWIN);

		if (GetAsyncKeyState(VK_LCONTROL))
			PressKey(VK_LCONTROL);
		else
			ReleaseKey(VK_LCONTROL);

		if (GetAsyncKeyState(VK_RCONTROL))
			PressKey(VK_RCONTROL);
		else
			ReleaseKey(VK_RCONTROL);

		if (GetAsyncKeyState(VK_LMENU))
			PressKey(VK_LMENU);
		else
			ReleaseKey(VK_LMENU);

		if (GetAsyncKeyState(VK_RMENU))
			PressKey(VK_RMENU);
		else
			ReleaseKey(VK_RMENU);

		if (GetAsyncKeyState(IM_SEMI))
			PressKey(IM_SEMI);
		else
			ReleaseKey(IM_SEMI);
		
		if (GetAsyncKeyState(IM_EQUALS))
			PressKey(IM_EQUALS);
		else
			ReleaseKey(IM_EQUALS);
		
		if (GetAsyncKeyState(IM_COMMA))
			PressKey(IM_COMMA);
		else
			ReleaseKey(IM_COMMA);
		
		if (GetAsyncKeyState(IM_DASH))
			PressKey(IM_DASH);
		else
			ReleaseKey(IM_DASH);
		
		if (GetAsyncKeyState(IM_PERIOD))
			PressKey(IM_PERIOD);
		else
			ReleaseKey(IM_PERIOD);
		
		if (GetAsyncKeyState(IM_FORWARDSLASH))
			PressKey(IM_FORWARDSLASH);
		else
			ReleaseKey(IM_FORWARDSLASH);
		
		if (GetAsyncKeyState(IM_TILDA))
			PressKey(IM_TILDA);
		else
			ReleaseKey(IM_TILDA);
			
		if (GetAsyncKeyState(IM_OSQBRACKET))
			PressKey(IM_OSQBRACKET);
		else
			ReleaseKey(IM_OSQBRACKET);
		
		if (GetAsyncKeyState(IM_BACKSLASH))
			PressKey(IM_BACKSLASH);
		else
			ReleaseKey(IM_BACKSLASH);
		
		if (GetAsyncKeyState(IM_CSQBRACKET))
			PressKey(IM_CSQBRACKET);
		else
			ReleaseKey(IM_CSQBRACKET);

		if (GetAsyncKeyState(IM_QUOTE))
			PressKey(IM_QUOTE);
		else
			ReleaseKey(IM_QUOTE);

		if (GetAsyncKeyState(VK_EXECUTE))
			PressKey(VK_EXECUTE);
		else
			ReleaseKey(VK_EXECUTE);
		
		if (GetAsyncKeyState(VK_SNAPSHOT))
			PressKey(VK_SNAPSHOT);
		else
			ReleaseKey(VK_SNAPSHOT);
	}*/

	void InputManager::PressKey(int keycode)
	{
		if (keyboard[keycode]->prevState != RELEASED)
			keyboard[keycode]->currState = HELD;
		else
			keyboard[keycode]->currState = PRESSED;
		keyboard[keycode]->prevState = keyboard[keycode]->currState;
	}

	void InputManager::ReleaseKey(int keycode)
	{
		keyboard[keycode]->prevState = keyboard[keycode]->currState;
		keyboard[keycode]->currState = RELEASED;
	}

	KeyStates InputManager::GetKeyState(int keycode)
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.find(keycode);

		if (key != keyboard.end())
			return key->second->currState;

		return RELEASED;
	}

	KeyStates InputManager::GetPreviousKeyState(int keycode)
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.find(keycode);

		if (key != keyboard.end())
			return key->second->prevState;

		return RELEASED;
	}

	void InputManager::ReleaseAll()
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.begin();
		for (; key != keyboard.end(); key++)
		{
			key->second->prevState = key->second->currState;
			key->second->currState = RELEASED;
		}
	}

	void InputManager::SetKeyPressed(int keycode, void(*function)())
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.find(keycode);

		if (key != keyboard.end())
			keyboard[keycode]->KeyPressed = function;

		else
		{
			Key* temp = new Key();
			temp->prevState = RELEASED;
			temp->currState = RELEASED;
			temp->KeyPressed = function;
			temp->KeyHeld = nullptr;
			temp->KeyReleased = nullptr;
			keyboard[keycode] = temp;
		}
	}

	void InputManager::SetKeyHeld(int keycode, void(*function)())
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.find(keycode);

		if (key != keyboard.end())
			keyboard[keycode]->KeyHeld = function;

		else
		{
			Key* temp = new Key();
			temp->prevState = RELEASED;
			temp->currState = RELEASED;
			temp->KeyPressed = nullptr;
			temp->KeyHeld = function;
			temp->KeyReleased = nullptr;
			keyboard[keycode] = temp;
		}
	}

	void InputManager::SetKeyReleased(int keycode, void(*function)())
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.find(keycode);

		if (key != keyboard.end())
			keyboard[keycode]->KeyReleased = function;

		else
		{
			Key* temp = new Key();
			temp->prevState = RELEASED;
			temp->currState = RELEASED;
			temp->KeyPressed = nullptr;
			temp->KeyHeld = nullptr;
			temp->KeyReleased = function;
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
					if (key->second->KeyPressed != nullptr)
						key->second->KeyPressed();
					break;
				case HELD:
					if (key->second->KeyHeld != nullptr)
						key->second->KeyHeld();
					break;
				case RELEASED:
					if (key->second->KeyReleased != nullptr)
						key->second->KeyReleased();					
					break;
			}
		}
	}

	void InputManager::NullAllFunctionPointers()
	{
		std::unordered_map<int, Key*>::iterator key = keyboard.begin();
		for (; key != keyboard.end(); key++)
		{
			key->second->KeyPressed = nullptr;
			key->second->KeyHeld = nullptr;
			key->second->KeyReleased = nullptr;
		}
	}
}



