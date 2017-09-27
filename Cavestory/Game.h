#ifndef GAME_H_
#define GAME_H_

#include <SDL.h>
#include "Graphics.h"
#include "Input.h"
#include "Player.h"

struct Player;

struct Game
{
	Game();
	~Game();

	static int kTileSize;
private:
	void eventLoop();
	void update(int elapsedTimeMs);
	void draw(Graphics &graphics);

	Player* player_;
	
};

#endif // GAME_H_