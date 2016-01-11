#pragma once
#include "stdafx.h"

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
	};

	class InputManager
	{
		public:
			InputManager();
			~InputManager();

			void updateKeyboard();
			void PressKey(int keycode);
			void ReleaseKey(int keycode);
			KeyStates GetKeyState(int keycode);
			KeyStates GetPreviousKeyState(int keycode);
		private:
			std::unordered_map<int, Key> keyboard;
	};
}


