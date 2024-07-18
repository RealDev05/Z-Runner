#pragma once

#define ATTACK_RATE 2
#define ATTACK_POWER 5

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include<SFML/System/Thread.hpp>
#include<SFML/System/Sleep.hpp>

#include<iostream>
#include <functional>

#include<unordered_map>;
#include <vector>

#include "Projectile.h"
#include "Platform.h"

#include "PathFinder.h"
#include "PathFindingNode.h"

using namespace sf;
using namespace std;

class Enemy
{
private:
	Vector2f velocity;
	vector<RectangleShape> gunShots;
	void refreshAttack(int waitTime); 
	void trueUpdate(float deltaTime, vector<Platform> platforms, vector<Projectile*> projectiles, FloatRect playerBounds, FloatRect ground,PathFinder pathFinder);
	Vector2f size;

	unordered_map<string, vector<Texture*>> textures;

	Thread* attackingTimer;
	Thread* updateThread;

	float health = 20;

	template <typename T>

	T vectorDivide(T x, T y) {
		return Vector2f(x.x / y.x, x.y / y.y);
	}

	Vector2f toVector2f(Vector2u vector);

	bool readyToUpdate = true;
public:
	bool isFalling = true;
	bool canAttack = true;
	bool canSeePlayer = false;
	bool died=false;
	Sprite enemy;

	Enemy(float width, float height, float x, float y, unordered_map<string, vector<Texture*>> &texture);

	int isCollidingWith(RectangleShape shape,bool selfCheck, FloatRect itemToCheck = FloatRect());
	bool isOnGround(RectangleShape shape);

	void updatePosition(Vector2f position);
	void updateSize(Vector2f size);

	void setVelocity(Vector2f velocity);

	void addVelocity(Vector2f velocity);
	int attackPlayer();

	void receiveDamage(float damage);

	void update(float deltaTime,vector<Platform> platforms,vector<Projectile*> projectiles, FloatRect playerBounds, FloatRect ground,PathFinder pathFinder);
	
	Vector2f getSize();
	Vector2f getPosition();
	Vector2f getVelocity();
	Sprite getObject();
};

