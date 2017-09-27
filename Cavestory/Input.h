#ifndef INPUT_H_
#define INPUT_H_

#include <map>
#include <SDL.h>

struct Input
{
	void beginNewFrame();
	void keyDownEvent(const SDL_Event &event);
	void keyUpEvent(const SDL_Event &event);

	bool wasKeyPressed(SDL_Keycode key);
	bool wasKeyReleased(SDL_Keycode key);
	bool isKeyHeld(SDL_Keycode key);

private:
	std::map<SDL_Keycode, bool> heldKeys_;
	std::map<SDL_Keycode, bool> pressedKeys_;
	std::map<SDL_Keycode, bool> releasedKeys_;
};

#endif