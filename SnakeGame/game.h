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

	// Ŀ�� �����
	void HideCursor();
	// Ŀ�� �̵�
	void MoveCursor(int x, int y);
	// ��� ���� ����
	void SetColor(Color c);

	// ���� ���� �ֺ��� �� �׸���
	void DrawWall();
	// �Է� �ޱ�
	void Input();
	// ȭ�� �׸��� ���� ���� �͸� �����
	void PreRender(bool eraseLastTail = true);
	// ȭ�� �׸���
	void Render(bool drawFirstTail = true);
	// ���� �� Render(before) ȣ��
	void Update();
	// ���� ������ �ٲ�
	void ChangeDirection(Direction dir);

	void MoveSnake(int x, int y);

	bool IsGameOver();

	void PlaceApple();

	bool TryToEatApple();
};

