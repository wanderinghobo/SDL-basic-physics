#include "Ball.h"



Ball::Ball()
{
	velocity.x = 0;
	velocity.y = 0;
	gravity = 400;//400 pixels per second per second
}


Ball::~Ball()
{
}

void Ball::update(float dt)
{
	//if ball is in the air, adhere to gravity
	if (pos.y < 720) {
		//accelerate via gravity
		acceleration.y = gravity;
	}
	else
	{
		//treating bottom of screen as ground, so we dont want to keep falling from here
		acceleration.y = 0;
		//if velocity indicates we are moving down, then rebound
		if (velocity.y > 0.01)
		{
			//bounce
			velocity.y = velocity.y * -0.9;//loses 10% of its energy when hitting stuff
		}
		else
		{
			//dont have enough velocity to bounce again, so lets reset velocity to zero
			velocity.y = 0;
			velocity.x = velocity.x * 0.99;//apply some friction when sliding on the ground
		}
	}

	//bouncing off walls
	//if hit left wall and moving left    OR    if hit right wall and moving right
	if ((pos.x <= 0 && velocity.x <= 0) || (pos.x >= 1280 && velocity.x >= 0))
	{
		velocity.x = velocity.x * -0.9;//reverse direction and lose 10% of movement energy
	}

	updateMovement(dt);
}

void Ball::draw()
{
	SDL_SetRenderDrawColor(renderer, 163, 73, 164, 255);
	SDL_Rect ballRect = { pos.x -25, pos.y -25, 50, 50 };

	SDL_RenderFillRect(renderer, &ballRect);
}
