#pragma once
#include <SDL.h>
#include "Vector.h"
#include <list>

using namespace std;

class GameObject
{

public:
	SDL_Renderer* renderer;
	
	Vector pos; //xy position
	Vector velocity; //how fast and where to move from this pos (pixels per second)
	Vector acceleration; //how much to increase/decrease velocity (pixels per second per second)

	GameObject();
	~GameObject();

	void setPosition(Vector pos);
	void setVelocity(Vector pos);

	virtual void update(float dt);
	virtual void updateMovement(float dt);
	virtual void draw();

	//static - exists for entire program life time, is only 1 of this variable, but shared amoungst all GameObject objects
	static list<GameObject*> *gameObjects;
};

