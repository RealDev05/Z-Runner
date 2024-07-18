#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include<SFML/System/Thread.hpp>
#include<SFML/System/Sleep.hpp>

#include "Object.h"

#include<unordered_map>;
#include <vector>

class Character : public Object
{
protected:
	int attackRate;

	Vector2f velocity;
	vector<RectangleShape> gunShots;
	void refreshAttack();
	Vector2f size;

	unordered_map<string, vector<Texture*>> textures;

	float health = 20;

	Thread* attackingTimer;

	template <typename T>

	T vectorDivide(T x, T y) {
		return Vector2f(x.x / y.x, x.y / y.y);
	}

	Vector2f toVector2f(Vector2u vector);

	unordered_map<string, int> textureIndex;

	Texture* getNextTexture(string textureName);

public :
	bool isFalling = true;
	bool canAttack = true;
	bool died = false;

	int isCollidingWith(RectangleShape shape, bool selfCheck, FloatRect itemToCheck = FloatRect());
	bool isOnGround(RectangleShape shape);

	void updatePosition(Vector2f position);
	void updateSize(Vector2f size);

	void receiveDamage(float damage);

	void setVelocity(Vector2f velocity);
	void addVelocity(Vector2f velocity);

	Vector2f getSize();
	Vector2f getPosition();
	Vector2f getVelocity();
};

