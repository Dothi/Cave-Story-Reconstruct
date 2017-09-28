#include "Game.h"
#include <cstdio>

namespace
{
	const int kFps = 60;
}

//static
int Game::kTileSize = 32;

Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_ShowCursor(0);
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

	player_ = new Player(graphics, 320, 240);


	bool running = true;
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

		if (input.wasKeyPressed(SDLK_ESCAPE))
		{
			running = false;
		}

		// Player movement
		if (input.isKeyHeld(SDLK_a) && input.isKeyHeld(SDLK_d))
		{
			player_->stopMoving();
		}
		else if (input.isKeyHeld(SDLK_a))
		{
			player_->startMovingLeft();
		}
		else if (input.isKeyHeld(SDLK_d))
		{
			player_->startMovingRight();
		}
		else
		{
			player_->stopMoving();
		}

		// Player Jump
		if (input.wasKeyPressed(SDLK_SPACE))
		{
			player_->startJump();
		}
		else if (input.wasKeyReleased(SDLK_SPACE))
		{
			player_->stopJump();
		}

		const int currentTimeMs = SDL_GetTicks();
		Game::update(currentTimeMs - lastUpdateTime);
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
	player_->update(elapsedTimeMs);
}

void Game::draw(Graphics &graphics)
{
	graphics.clear();
	player_->draw(graphics);

	graphics.updateWindowSurface();

}