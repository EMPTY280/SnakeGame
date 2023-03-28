#include <iostream>
#include <ctime>
#include <Windows.h>

#include "game.h"

#define BOARD_W 20		// 게임 보드의 가로 길이
#define BOARD_H 20		// 게임 보드의 세로 길이
#define UPDATE_MS 300	// 업데이트 간격 (ms)

using namespace std;

Game::Game()
{
	snakePos = {BOARD_W * 0.5, BOARD_H * 0.5};
}

Game::~Game()
{
}

void Game::Start()
{
	double prevTime = clock();
	double updateDelay = 0;

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
			updateDelay = 0;
		}
	}
}

void Game::HideCursor()
{
	CONSOLE_CURSOR_INFO csrInfo = {0, };
	csrInfo.dwSize = 1;
	csrInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csrInfo);
}

void Game::MoveCursor(int x, int y)
{
	COORD coord = { x << 1, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Game::DrawWall()
{
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

void Game::RenderBefore(bool eraseLastTail)
{
	MoveCursor(snakePos.first, snakePos.second);
	cout << "  ";

	if ((snakeTails.size() >= tailLength) && eraseLastTail)
	{
		pair<int, int> lastTail = snakeTails.back();
		MoveCursor(lastTail.first, lastTail.second);
		cout << "  ";
	}

	MoveCursor(0, 0);
}

void Game::Render(bool drawFirstTail)
{
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
	MoveCursor(snakePos.first, snakePos.second);
	cout << headShape;

	if ((snakeTails.size() > 0) && drawFirstTail)
	{
		pair<int, int> firstTail = snakeTails.front();
		MoveCursor(firstTail.first, firstTail.second);
		cout << "■";
	}

	MoveCursor(0, 0);
}

void Game::ChangeDirection(Direction dir)
{
	RenderBefore(false);
	snakeDir = dir;
	Render(false);
}

void Game::Update()
{
	RenderBefore();

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
