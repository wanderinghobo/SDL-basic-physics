#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Bullet.h"

class Hero : public GameObject
{
public:
	Animation* animation;
	bool faceRight;

	Hero();
	~Hero();

	//override update n draw
	void update(float dt);
	void draw();

	void shoot();
};

