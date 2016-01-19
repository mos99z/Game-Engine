#pragma once
#include "stdafx.h"

#ifdef INPUTMANAGERDLL_EXPORTS
#define INPUTMANAGERDLL// __declspec(dllexport) 
#else
#define INPUTMANAGERDLL //__declspec(dllimport) 
#endif


#include <unordered_map>
//#define SLOW_TIMER_WAIT 500

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

	struct Key
	{
		bool pressed;
		bool held;
		bool released;
		void (*KeyPressed) ();
		void (*KeyHeld) ();
		void (*KeyReleased) ();
	};

	struct RawKey
	{
		int keyCode;
		void(*KeyPressed) ();
		void(*KeyHeld) ();
		void(*KeyReleased) ();
	};

	class InputManager
	{
		public:
			INPUTMANAGERDLL InputManager();
			INPUTMANAGERDLL ~InputManager();

			INPUTMANAGERDLL void LoadKeyBoard(std::vector<RawKey> board);

			INPUTMANAGERDLL void Update();

			INPUTMANAGERDLL void SetKeyPressed(int keycode, void(*function)());
			INPUTMANAGERDLL void SetKeyHeld(int keycode, void(*function)());
			INPUTMANAGERDLL void SetKeyReleased(int keycode, void(*function)());
			INPUTMANAGERDLL void RemoveKey(int keycode);

		private:
			std::unordered_map<int, Key*> keyboard;
			//int timer = TIMER_WAIT;

			void DisableAll();
			void NullAllFunctionPointers();
			void UpdateKeyboard();
			void KeyUpdates();
			void Clear();
	};
}


