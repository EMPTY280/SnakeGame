#pragma once
#include <list>

using namespace std;

class Game
{
public:
	Game();
	~Game();
	void Start();
private:

	enum class Direction
	{
		UP, DOWN, LEFT, RIGHT
	};

	pair<int, int> snakePos;
	unsigned int tailLength = 5;
	Direction snakeDir = Direction::RIGHT;
	list<pair<int, int>> snakeTails;

	pair<int, int> applePos;

	bool gameOver = false;

	// 커서 숨기기
	void HideCursor();
	// 커서 이동
	void MoveCursor(int x, int y);
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

	void IsGameOver();

	void TryToEatApple();
};

