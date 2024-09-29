#pragma once

#define BULLET_SPEED 250

#define MOVEMENT_SPEED 100
#define JUMP_HEIGHT 100

#include<iostream>
#include <functional>

#include "Projectile.h"

#include "Character.h"

class Player : public Character
{
private:
	vector<RectangleShape> gunShots;

public:

	Player();
	void initialize(float width, float height, float x, float y, unordered_map<string, vector<Texture*>>& texture);

	Projectile* shootAt(Vector2f target);

};

