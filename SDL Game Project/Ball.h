#pragma once
#include "GameObject.h"
class Ball :
	public GameObject
{
public:
	float gravity;

	Ball();
	~Ball();

	//override
	void update(float dt);
	void draw();
};

