#pragma once
#include "InputHandler.h"
class MouseHandler :
	public InputHandler
{
public:
	bool leftMousePressedDown;


	MouseHandler();
	~MouseHandler();

	//override 
	virtual void update(SDL_Event* event);
};

