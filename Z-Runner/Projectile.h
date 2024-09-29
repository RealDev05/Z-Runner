#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Thread.hpp>
#include <iostream>
#include <functional>
#include <vector>

using namespace sf;
using namespace std;

class Projectile
{
private:
	float damage;
	Vector2f velocity;
	RectangleShape projectile;
	Thread* updateThread;

public:
	Projectile(float damage, Vector2f velocity, Vector2f size,Vector2f init, Color color,bool isHostile);
	bool destroyed = false;
	bool isHostile = false;

	void update(float deltaTime , vector<RectangleShape> shapes);
	void checkForCollision(vector<RectangleShape> shapes);
	
	void destruct();
	float getDamage();

	RectangleShape getObject();
};

