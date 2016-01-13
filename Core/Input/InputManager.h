#pragma once
#include "stdafx.h"

#ifdef INPUTMANAGERDLL_EXPORTS
#define INPUTMANAGERDLL __declspec(dllexport) 
#else
#define INPUTMANAGERDLL __declspec(dllimport) 
#endif




#include <unordered_map>
#define TIMER_WAIT 500

namespace Input 
{
	 enum Keys
	{
		IM_0 = 0x30,
		IM_1,
		IM_2,
		IM_3,
		IM_4,
		IM_5,
		IM_6,
		IM_7,
		IM_8,
		IM_9,
		IM_A = 0x41,
		IM_B,
		IM_C,
		IM_D,
		IM_E,
		IM_F,
		IM_G,
		IM_H,
		IM_I,
		IM_J,
		IM_K,
		IM_L,
		IM_M,
		IM_N,
		IM_O,
		IM_P,
		IM_Q,
		IM_R,
		IM_S,
		IM_T,
		IM_U,
		IM_V,
		IM_W,
		IM_X,
		IM_Y,
		IM_Z,
		IM_SEMI = 0xBA,
		IM_EQUALS,
		IM_COMMA,
		IM_DASH,
		IM_PERIOD,
		IM_FORWARDSLASH,
		IM_TILDA,
		IM_OSQBRACKET = 0xDB,
		IM_BACKSLASH,
		IM_CSQBRACKET,
		IM_QUOTE,
	};

	 enum KeyStates {PRESSED, HELD, RELEASED};

	struct Key
	{
		KeyStates currState;
		KeyStates prevState;
		void(*KeyPressed)();
		void(*KeyHeld)();
		void(*KeyReleased)();
	};

	class InputManager
	{
		public:
			INPUTMANAGERDLL InputManager();
			INPUTMANAGERDLL ~InputManager();

			INPUTMANAGERDLL void Update();

			INPUTMANAGERDLL void UpdateKeyboard();
			INPUTMANAGERDLL void PressKey(int keycode);
			INPUTMANAGERDLL void ReleaseKey(int keycode);
			INPUTMANAGERDLL KeyStates GetKeyState(int keycode);
			INPUTMANAGERDLL KeyStates GetPreviousKeyState(int keycode);
			INPUTMANAGERDLL void SetKeyPressed(int keycode, void(*function)());
			INPUTMANAGERDLL void SetKeyHeld(int keycode, void(*function)());
			INPUTMANAGERDLL void SetKeyReleased(int keycode, void(*function)());
			INPUTMANAGERDLL void KeyUpdates();

		private:
			std::unordered_map<int, Key> keyboard;
			int timer = TIMER_WAIT;

			void ReleaseAll();
			void NullAllFunctionPointers();
	};
}

