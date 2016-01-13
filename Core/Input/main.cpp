#include "stdafx.h"
#include "InputManager.h"

void PrintAMessage();
void ChangeA();
Input::InputManager Iman;

int main(int argc, char** argv)
{
	

	Iman.SetKeyPressed(Input::IM_A, PrintAMessage);

	while (true)
	{
		Iman.UpdateKeyboard();
		Iman.KeyUpdates();

		if (Iman.GetKeyState(VK_ESCAPE) == Input::PRESSED)
			break;
	}

	return EXIT_SUCCESS;
}

void PrintAMessage()
{
	std::cout << "A pressed ";
	Iman.SetKeyPressed(Input::IM_A, ChangeA);
}

void ChangeA()
{
	std::cout << "A changed ";
	Iman.SetKeyPressed(Input::IM_A, PrintAMessage);
}