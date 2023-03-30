#include <iostream>
#include "game.h"

using namespace std;

int main()
{
	Game* game;
	do
	{
		game = new Game();
		game->Start();
	} while (game->OnGameOver());

	delete game;
}