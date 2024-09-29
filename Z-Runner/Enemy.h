#pragma once

#define ATTACK_POWER 5

#include<iostream>
#include <functional>

#include "Platform.h"
#include "Projectile.h"

#include "PathFinder.h"
#include "PathFindingNode.h"

#include "Character.h"

class Enemy : public Character
{
private:
	void trueUpdate(float deltaTime, vector<Platform> platforms, vector<Projectile*> projectiles, FloatRect playerBounds, FloatRect ground,PathFinder pathFinder);
	
	Thread* updateThread;

	bool readyToUpdate = true;

public:
	bool canSeePlayer = false;

	Enemy(float width, float x, float y, unordered_map<string, vector<Texture*>> &texture);

	int attackPlayer();

	void update(float deltaTime,vector<Platform> platforms,vector<Projectile*> projectiles, FloatRect playerBounds, FloatRect ground,PathFinder pathFinder);

};

