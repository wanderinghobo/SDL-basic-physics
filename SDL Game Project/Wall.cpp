#include "Wall.h"



Wall::Wall()
{
}


Wall::~Wall()
{
}

void Wall::draw()
{
	SDL_Rect rect = { pos.x,pos.y,w,h };
	//set render draw colour
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	//draw the rectangle
	SDL_RenderFillRect(renderer, &rect);

}
