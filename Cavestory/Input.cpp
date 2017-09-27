#include "Input.h"

void Input::beginNewFrame()
{
	pressedKeys_.clear();
	releasedKeys_.clear();
}

void Input::keyDownEvent(const SDL_Event &event)
{
	pressedKeys_[event.key.keysym.sym] = true;
	heldKeys_[event.key.keysym.sym] = true;
}

void Input::keyUpEvent(const SDL_Event &event)
{
	releasedKeys_[event.key.keysym.sym] = true;
	heldKeys_[event.key.keysym.sym] = false;
}

bool Input::wasKeyPressed(SDL_Keycode key)
{
	return pressedKeys_[key];
}

bool Input::wasKeyReleased(SDL_Keycode key)
{
	return releasedKeys_[key];
}

bool Input::isKeyHeld(SDL_Keycode key)
{
	return heldKeys_[key];
}