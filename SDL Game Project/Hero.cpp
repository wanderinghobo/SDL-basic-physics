#include "Hero.h"



Hero::Hero()
{
	animation = NULL;
	faceRight = true;
}


Hero::~Hero()
{
}

void Hero::update(float dt)
{
	
	//use basic game object vector movement
	updateMovement(dt);

	//update facing directio based on velocity
	if (velocity.x > 0)
		faceRight = true;
	if (velocity.x < 0)
		faceRight = false;

	//update our animation
	if(velocity.x != 0 || velocity.y != 0)
		animation->update(dt);
}

void Hero::draw()
{
	if (animation != NULL) {
		if (faceRight)
			animation->draw(pos.x, pos.y);
		else
			animation->draw(pos.x, pos.y, true);
	}
}

void Hero::shoot()
{
	Bullet* bullet = new Bullet();
	bullet->renderer = renderer;
	bullet->setPosition(pos);
	bullet->angle = rand() % 360;//rand gives random int between min and max int value, modulus it into 360 degree value
	bullet->movementSpeed = 200;

	//add to gameObjects list
	GameObject::gameObjects->push_back(bullet);

}
