#include "Game.h"
#include "Timer.h"
#include "Player.h"
#include "Bat.h"
#include "Map.h"

#include <cstdio>

namespace
{
	const int kFps = 60;
	const int kMaxFrameTime = 5 * 1000 / 60;
}

// Static
int Game::kTileSize = 32;
int Game::kHalfTile = Game::kTileSize / 2;
int Game::kScreenWidth = 1024;
int Game::kScreenHeight = 768;

Game::Game() : running(false)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	Game::eventLoop();
}

Game::~Game()
{
	SDL_Quit();
}

void Game::eventLoop()
{
	Graphics graphics;
	SDL_Event event;
	Input input;

	player_ = new Player(graphics, 350.0f, 240.0f);
	bat_ = new Bat(graphics, Vector2(150.0f, 490.0f));
	map_ = Map::createTestMap(graphics);


	running = true;
	int lastUpdateTime = SDL_GetTicks();

	while (running)
	{
		const int startTimeMs = SDL_GetTicks();
		input.beginNewFrame();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				input.keyDownEvent(event);
				break;
			case SDL_KEYUP:
				input.keyUpEvent(event);
				break;
			default:
				break;
			}
		}

		// Handle all player inputs
		Game::handleInput(input);

		const int currentTimeMs = SDL_GetTicks();
		const int elapsedTime = currentTimeMs - lastUpdateTime;

		Game::update(std::fmin(elapsedTime, kMaxFrameTime)); //TIP: Divide this for slowmo for debugging purposes

		lastUpdateTime = currentTimeMs;
		Game::draw(graphics);

		const int msPerFrame = 1000 / kFps;
		const int elapsedTimeMs = SDL_GetTicks() - startTimeMs;

		if (elapsedTimeMs < msPerFrame)
			SDL_Delay(msPerFrame - elapsedTimeMs); // 1000ms / 60 fps

			/*const float secondsPerFrame = (SDL_GetTicks() - startTimeMs) / 1000.0;
			const float fps = 1 / secondsPerFrame;
			printf("fps=%f\n", fps);*/
	}
}

void Game::update(int elapsedTimeMs)
{
	Timer::updateAll(elapsedTimeMs);

	player_->update(elapsedTimeMs, *map_);
	bat_->update(elapsedTimeMs, player_->getPosition());
	//map_->update(elapsedTimeMs);

	if (bat_->damageRectangle().collidesWith(player_->damageRectangle()))
	{
		player_->takeDamage();
	}
}

void Game::draw(Graphics &graphics)
{
	graphics.clear();

	map_->drawBackground(graphics);
	bat_->draw(graphics);
	player_->draw(graphics);
	map_->draw(graphics);

	player_->drawHUD(graphics); 

	graphics.updateWindowSurface();
}

void Game::handleInput(Input &input)
{
	if (input.wasKeyPressed(SDLK_ESCAPE))
	{
		running = false;
	}

	// Player movement
	if (input.isKeyHeld(SDLK_LEFT) && input.isKeyHeld(SDLK_RIGHT))
	{
		player_->stopMoving();
	}
	else if (input.isKeyHeld(SDLK_LEFT))
	{
		player_->startMovingLeft();
	}
	else if (input.isKeyHeld(SDLK_RIGHT))
	{
		player_->startMovingRight();
	}
	else
	{
		player_->stopMoving();
	}

	if (input.isKeyHeld(SDLK_UP) && input.isKeyHeld(SDLK_DOWN))
	{
		player_->lookHorizontal();
	}
	else if (input.isKeyHeld(SDLK_UP))
	{
		player_->lookUp();
	}
	else if (input.isKeyHeld(SDLK_DOWN))
	{
		player_->lookDown();
	}
	else
	{
		player_->lookHorizontal();
	}

	// Player Jump
	if (input.wasKeyPressed(SDLK_z) || input.wasKeyPressed(SDLK_SPACE))
	{
		player_->startJump();
	}
	else if (input.wasKeyReleased(SDLK_z) || input.wasKeyReleased(SDLK_SPACE))
	{
		player_->stopJump();
	}
	// Player Fire
	if (input.wasKeyPressed(SDLK_x))
	{
		player_->startFire();
	}
	else if (input.wasKeyReleased(SDLK_x))
	{
		player_->stopFire();
	}
}