#include "Bullet.h"



Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}

void Bullet::update(float dt)
{
	//convert angle in degress to radians so we can do trigonometry
	float angleInRadians = angle / 180 * M_PI;

	//convert radian and speed into vector values
	velocity.x = movementSpeed*cos(angleInRadians);
	velocity.y = movementSpeed*sin(angleInRadians);

	updateMovement(dt);
}

void Bullet::draw()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);//yellow
	SDL_Rect bulletRect = { pos.x - 10, pos.y - 10, 20, 20 }; //x,y,w,h
	SDL_RenderFillRect(renderer, &bulletRect);
}
