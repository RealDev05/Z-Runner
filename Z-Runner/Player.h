#pragma once

#define BULLET_SPEED 250
#define SHOOTING_RATE 2

#define MOVEMENT_SPEED 100
#define JUMP_HEIGHT 100

#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include<SFML/System/Thread.hpp>
#include<SFML/System/Sleep.hpp>
#include<iostream>
#include <functional>

#include "Projectile.h"

using namespace sf;
using namespace std;

class Player
{
private:
	RectangleShape player,groundChecker;
	Vector2f velocity;
	vector<RectangleShape> gunShots;
	void refreshShooting(int waitTime);

	Thread* shootingTimer;

	float health=20;

public:
	bool isFalling = false;
	bool canShoot = true;
	bool isDead = false;

	Player();
	void initialize(float width, float height, float x, float y, Color color);
	int isCollidingWith(RectangleShape shape);
	bool isOnGround(RectangleShape shape);

	void updatePosition(Vector2f position);
	void updateSize(Vector2f size);
	void receiveDamage(float damage);

	void setVelocity(Vector2f velocity);
	void setColor(Color color);

	void addVelocity(Vector2f velocity);
	Projectile* shootAt(Vector2f target);

	Vector2f getSize();
	Vector2f getPosition();
	Vector2f getVelocity();
	RectangleShape getObject();
};

