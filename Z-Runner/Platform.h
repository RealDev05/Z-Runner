#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

using namespace sf;

class Platform
{
private:
	RectangleShape shape,bounds;

public :
	bool ispassThrough;

	Platform();
	void initialize(float width,float height,float x,float y, Color color,bool isPassThrough);

	void updateColor(Color color);
	void updateSize(int width, int height);
	void updatePosition(float x, float y);

	Vector2f getPosition();
	RectangleShape getObject();
	RectangleShape getBoundaryObject();
};

