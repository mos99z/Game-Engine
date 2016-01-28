#include "stdafx.h"
#include "InputManager.h"
#include "MessageManager.h"

void PrintAMessage();
void Clear();
void ReleaseA();
void ChangeA();
void left();
void right();
void up();
void down();
void End();
void MessageProc(Messages mess);

Input::InputManager Iman;
bool run = true;

int main(int argc, char** argv)
{
	std::vector<Input::RawKey> board;
	Input::RawKey button;

	button.keyCode = Input::IM_A;
	button.KeyPressed = A_PRESSED;
	button.KeyHeld = NO_MESSAGE;// nullptr;
	button.KeyReleased = A_RELEASED;
	board.push_back(button);

	button.keyCode = Input::IM_S;
	button.KeyPressed = A_CHANGE;
	button.KeyHeld =  NO_MESSAGE;// nullptr;
	button.KeyReleased =  NO_MESSAGE;// nullptr;
	board.push_back(button);

	button.keyCode = VK_LEFT;
	button.KeyPressed =  NO_MESSAGE;// nullptr;
	button.KeyHeld = LEFT;
	button.KeyReleased =  NO_MESSAGE;// nullptr;
	board.push_back(button);

	button.keyCode = VK_RIGHT;
	button.KeyPressed = RIGHT;
	button.KeyHeld =  NO_MESSAGE;// nullptr;
	button.KeyReleased =  NO_MESSAGE;// nullptr;
	board.push_back(button);

	button.keyCode = VK_UP;
	button.KeyPressed = UP;
	button.KeyHeld =  NO_MESSAGE;// nullptr;
	button.KeyReleased =  NO_MESSAGE;// nullptr;
	board.push_back(button);

	button.keyCode = VK_DOWN;
	button.KeyPressed = DOWN;
	button.KeyHeld =  NO_MESSAGE;// nullptr;
	button.KeyReleased =  NO_MESSAGE;// nullptr;
	board.push_back(button);

	button.keyCode = VK_ESCAPE;
	button.KeyPressed = END;
	button.KeyHeld =  NO_MESSAGE;// nullptr;
	button.KeyReleased =  NO_MESSAGE;// nullptr;
	board.push_back(button);

	Iman.LoadKeyBoard(board);
	MessageManager::GetInstance()->AddProcessMessageFunc(MessageProc);

	while (run)
	{
		Iman.Update();
		MessageManager::GetInstance()->Update();
	}

	return EXIT_SUCCESS;
}

void MessageProc(Messages mess)
{
	switch (mess)
	{
	case NO_MESSAGE:
		break;
	case A_PRESSED:
		PrintAMessage();
		break;
	case A_RELEASED:
		ReleaseA();
		break;
	case A_CHANGE:
		ChangeA();
		break;
	case LEFT:
		left();
		break;
	case RIGHT:
		right();
		break;
	case UP:
		up();
		break;
	case DOWN:
		down();
		break;
	case END:
		End();
		break;
	default:
		std::cout << "UNKNOWN MESSAGE TYPE";
		break;
	}
}


void PrintAMessage()
{
	std::cout << "A pressed ";
}

void ReleaseA()
{
	std::cout << "A released ";
}

void Clear()
{
	//system("cls");
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