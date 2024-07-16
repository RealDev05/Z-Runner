#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

using namespace sf;

class Obstacle
{
private :
	RectangleShape obstacle;
public:
	Obstacle();

	RectangleShape getObject();
};

