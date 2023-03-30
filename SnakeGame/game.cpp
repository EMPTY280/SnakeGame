#include <iostream>
#include <ctime>
#include <Windows.h>

#include "game.h"

#define BOARD_W 20		// 게임 보드의 가로 길이
#define BOARD_H 20		// 게임 보드의 세로 길이
#define UPDATE_MS 200	// 업데이트 간격 (ms)

using namespace std;

void Game::Start()
{
	system("cls");

	double prevTime = clock();
	double updateDelay = 0;

	snakePos = { BOARD_W * 0.5, BOARD_H * 0.5 };
	PlaceApple();

	HideCursor();
	DrawWall();
	Render();

	while (true)
	{
		double crntTime = clock();
		double elapsed = crntTime - prevTime;
		prevTime = crntTime;
		updateDelay += elapsed;

		Input();

		if (updateDelay >= UPDATE_MS)
		{
			Update();
			if (IsGameOver())
				break;
			updateDelay = 0;
		}
	}
}

void Game::HideCursor()
{
	CONSOLE_CURSOR_INFO csrInfo = {0, };
	csrInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csrInfo);
}

void Game::MoveCursor(int x, int y)
{
	COORD coord = { x << 1, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Game::SetColor(Color c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)c);
}

void Game::DrawWall()
{
	SetColor(Color::BLUE);
	for (int i = 1; i <= BOARD_W; i++)
	{
		MoveCursor(i, 0);
		cout << "■";
		MoveCursor(i, BOARD_H + 1);
		cout << "■";
	}

	for (int i = 1; i <= BOARD_H; i++)
	{
		MoveCursor(0, i);
		cout << "■";
		MoveCursor(BOARD_W + 1, i);
		cout << "■";
	}

	SetColor(Color::WHITE);
	cout << endl << endl;
}

void Game::Input()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		ChangeDirection(Direction::UP);
		return;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		ChangeDirection(Direction::DOWN);
		return;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		ChangeDirection(Direction::LEFT);
		return;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		ChangeDirection(Direction::RIGHT);
		return;
	}
}

void Game::PreRender(bool eraseLastTail)
{
	MoveCursor(snakePos.first + 1, snakePos.second + 1);
	cout << "  ";

	if ((snakeTails.size() >= tailLength) && eraseLastTail && tailLength != 0)
	{
		pair<int, int> lastTail = snakeTails.back();
		MoveCursor(lastTail.first + 1, lastTail.second + 1);
		cout << "  ";
	}

	MoveCursor(applePos.first + 1, applePos.second + 1);
	cout << "  ";

	MoveCursor(0, 0);
}

void Game::Render(bool drawFirstTail)
{
	// 뱀 그리기
	SetColor(Color::GREEN);
	string headShape;
	switch (snakeDir)
	{
	case Direction::UP:
		headShape = "▲";
		break;
	case Direction::DOWN:
		headShape = "▼";
		break;
	case Direction::LEFT:
		headShape = "◀";
		break;
	case Direction::RIGHT:
	default:
		headShape = "▶";
		break;
	}
	MoveCursor(snakePos.first + 1, snakePos.second + 1);
	cout << headShape;

	if ((snakeTails.size() > 0) && drawFirstTail)
	{
		pair<int, int> firstTail = snakeTails.front();
		MoveCursor(firstTail.first + 1, firstTail.second + 1);
		cout << "■";
	}

	// 사과 그리기
	SetColor(Color::RED);
	MoveCursor(applePos.first + 1, applePos.second + 1);
	cout << "●";
	SetColor(Color::WHITE);

	MoveCursor(0, BOARD_H + 3);

	cout << "[ 조작법 ] : 방향키로 회전" << endl;
	cout << "[ 점수 ] : " << tailLength << endl;
}

void Game::ChangeDirection(Direction dir)
{
	if (snakeTails.size() > 0)
	{
		int xx = 0;
		int yy = 0;

		switch (dir)
		{
		case Game::Direction::UP:
			yy = -1;
			break;
		case Game::Direction::DOWN:
			yy = 1;
			break;
		case Game::Direction::LEFT:
			xx = -1;
			break;
		case Game::Direction::RIGHT:
		default:
			xx = 1;
			break;
		}

		if (snakePos.first + xx == snakeTails.front().first && snakePos.second + yy + snakeTails.front().second)
			return;
	}

	PreRender(false);
	this->snakeDir = dir;
	Render(false);
}

void Game::Update()
{
	PreRender();

	switch (snakeDir)
	{
	case Direction::UP:
		MoveSnake(0, -1);
		break;
	case Direction::DOWN:
		MoveSnake(0, 1);
		break;
	case Direction::LEFT:
		MoveSnake(-1, 0);
		break;
	case Direction::RIGHT:
	default:
		MoveSnake(1, 0);
		break;
	}

	if (TryToEatApple())
		PlaceApple();

	Render();
}

void Game::MoveSnake(int x, int y)
{
	snakeTails.push_front(make_pair(snakePos.first, snakePos.second));

	snakePos.first += x;
	snakePos.second += y;

	while (snakeTails.size() > tailLength)
	{
		snakeTails.pop_back();
	}
}

bool Game::IsGameOver()
{
	if (snakePos.first < 0 ||
		snakePos.first >= BOARD_W ||
		snakePos.second < 0 ||
		snakePos.second >= BOARD_H)
		return true;

	for (pair<int, int> pos : snakeTails)
	{
		if (snakePos == pos)
			return true;
	}

	return false;
}

bool Game::OnGameOver()
{
	cout << "게임 끝! 다시 플레이합니까 ? (Y/N) : ";
	char input;
	cin >> input;
	if (input == 'Y' || input == 'y')
	{
		cin.clear();
		cin.ignore(255, '\n');
		return true;
	}
	return false;
}

void Game::PlaceApple()
{
	bool repeat = true;

	while (repeat)
	{
		applePos = make_pair(rand() % BOARD_W, rand() % BOARD_H);
		repeat = false;

		if (applePos == snakePos)
			repeat = true;
		for (pair<int, int> pos : snakeTails)
		{
			if (applePos == pos)
			{
				repeat = true;
				break;
			}
		}
	}
}

bool Game::TryToEatApple()
{
	if (snakePos == applePos)
	{
		tailLength++;
		PlaceApple();
		return true;
	}

	return false;
}