#pragma once


#include <SFML/Graphics/RectangleShape.hpp>
#include<SFML/System/Thread.hpp>
#include<SFML/System/Sleep.hpp>

#include "Object.h"

#include<unordered_map>;
#include <vector>

class characters : public Object
{
protected:
	int attackRate;

	Vector2f velocity;
	vector<RectangleShape> gunShots;
	void refreshAttack(int waitTime);
	Vector2f size;

	unordered_map<string, vector<Texture*>> textures;

	Thread* attackingTimer;

	template <typename T>

	T vectorDivide(T x, T y) {
		return Vector2f(x.x / y.x, x.y / y.y);
	}

public :
	int isCollidingWith(RectangleShape shape, bool selfCheck, FloatRect itemToCheck = FloatRect());
	bool isOnGround(RectangleShape shape);

	void updatePosition(Vector2f position);
	void updateSize(Vector2f size);

	void setVelocity(Vector2f velocity);
	void addVelocity(Vector2f velocity);
	Vector2f getVelocity();
};

