#pragma once
#include <list>

using namespace std;

class Game
{
public:
	void Start();
	bool OnGameOver();
private:

	enum class Direction
	{
		UP, DOWN, LEFT, RIGHT
	};

	enum class Color
	{
		BLACK,
		DARK_BLUE,
		DARK_GREEN,
		DARK_SKYBLUE,
		DARK_RED,
		DARK_VOILET,
		DAKR_YELLOW,
		GRAY,
		DARK_GRAY,
		BLUE,
		GREEN,
		SKYBLUE,
		RED,
		VIOLET,
		YELLOW,
		WHITE
	};

	pair<int, int> snakePos;
	unsigned int tailLength = 0;
	Direction snakeDir = Direction::RIGHT;
	list<pair<int, int>> snakeTails;

	pair<int, int> applePos;

	// 커서 숨기기
	void HideCursor();
	// 커서 이동
	void MoveCursor(int x, int y);
	// 출력 색상 변경
	void SetColor(Color c);

	// 게임 보드 주변의 벽 그리기
	void DrawWall();
	// 입력 받기
	void Input();
	// 화면 그리기 전에 지울 것만 지우기
	void PreRender(bool eraseLastTail = true);
	// 화면 그리기
	void Render(bool drawFirstTail = true);
	// 연산 및 Render(before) 호출
	void Update();
	// 뱀의 방향을 바꿈
	void ChangeDirection(Direction dir);

	void MoveSnake(int x, int y);

	bool IsGameOver();

	void PlaceApple();

	bool TryToEatApple();
};

