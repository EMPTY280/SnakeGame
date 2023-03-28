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
	int tailLength = 5;
	Direction snakeDir = Direction::RIGHT;
	list<pair<int, int>> snakeTails;

	// Ŀ�� �����
	void HideCursor();
	// Ŀ�� �̵�
	void MoveCursor(int x, int y);
	// ���� ���� ������ �� �׸���
	void DrawWall();
	// �Է� �ޱ�
	void Input();
	// ȭ�� �׸��� ���� ���� �͸� �����
	void RenderBefore(bool eraseLastTail = true);
	// ȭ�� �׸���
	void Render(bool drawFirstTail = true);
	// ���� �� Render(before) ȣ��
	void Update();

	void ChangeDirection(Direction dir);

	void MoveSnake(int x, int y);
};

