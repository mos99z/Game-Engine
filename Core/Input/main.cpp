#include "stdafx.h"
#include "InputManager.h"

void PrintAMessage();
void Clear();
void ChangeA();
void left();
void right();
void up();
void down();
void End();
Input::InputManager Iman;
bool run = true;

int main(int argc, char** argv)
{
	std::vector<Input::RawKey> board;
	Input::RawKey button;

	button.keyCode = Input::IM_A;
	button.KeyPressed = PrintAMessage;
	button.KeyHeld = Clear;
	button.KeyReleased = nullptr;
	board.push_back(button);

	button.keyCode = Input::IM_S;
	button.KeyPressed = ChangeA;
	button.KeyHeld = nullptr;
	button.KeyReleased = nullptr;
	board.push_back(button);

	button.keyCode = VK_LEFT;
	button.KeyPressed = left;
	button.KeyHeld = nullptr;
	button.KeyReleased = nullptr;
	board.push_back(button);

	button.keyCode = VK_RIGHT;
	button.KeyPressed = right;
	button.KeyHeld = nullptr;
	button.KeyReleased = nullptr;
	board.push_back(button);

	button.keyCode = VK_UP;
	button.KeyPressed = up;
	button.KeyHeld = nullptr;
	button.KeyReleased = nullptr;
	board.push_back(button);

	button.keyCode = VK_DOWN;
	button.KeyPressed = down;
	button.KeyHeld = nullptr;
	button.KeyReleased = nullptr;
	board.push_back(button);

	button.keyCode = VK_ESCAPE;
	button.KeyPressed = End;
	button.KeyHeld = nullptr;
	button.KeyReleased = nullptr;
	board.push_back(button);

	Iman.LoadKeyBoard(board);

	while (run)
		Iman.Update();

	return EXIT_SUCCESS;
}

void PrintAMessage()
{
	std::cout << "A pressed ";
}

void Clear()
{
	system("cls");
	std::cout << "Cleared ";
}

void ChangeA()
{
	std::cout << "A removed ";
	Iman.RemoveKey(Input::IM_A);
}

void left()
{
	std::cout << "left ";
}

void right()
{
	std::cout << "right ";
}

void up()
{
	std::cout << "up ";
}

void down()
{
	std::cout << "down ";
}

void End()
{
	std::cout << "GOODBYE";
	run = false;
}