#ifndef GAME_H_
#define GAME_H_

#include <SDL.h>
#include "Graphics.h"
#include "Input.h"


struct Player;
struct Bat;
struct Map;


struct Game
{
	Game();
	~Game();

	static int kTileSize;
	static int kHalfTile;
	static int kScreenWidth;
	static int kScreenHeight;
private:
	void eventLoop();
	void update(int elapsedTimeMs);
	void draw(Graphics &graphics);
	void handleInput(Input &input);

	Player *player_;
	Bat *bat_;
	Map *map_;

	bool running;
	
};

#endif // GAME_H_