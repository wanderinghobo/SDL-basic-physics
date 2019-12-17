#include "MouseHandler.h"



MouseHandler::MouseHandler()
{
	leftMousePressedDown = false;
}


MouseHandler::~MouseHandler()
{
}

void MouseHandler::update(SDL_Event * event)
{
	//CHECK if user clicked with mouse button ON hero
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
	{
		if (event->button.x >= hero->pos.x && event->button.x <= hero->pos.x + 32
			&& event->button.y >= hero->pos.y && event->button.y <= hero->pos.y + 32)
		{
			leftMousePressedDown = true;
		}
	}

	//CHECK if left mouse button was released
	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
	{
		leftMousePressedDown = false;
	}

	//if dragging mouse AND holding down left button
	if (leftMousePressedDown && event->type == SDL_MOUSEMOTION)
	{
		hero->pos.x = event->motion.x - 16;
		hero->pos.y = event->motion.y - 16;

		
	}


}
