#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <iostream>

using namespace std;
using namespace sf;

class Object
{
protected:
	Sprite self;
public:
	Sprite getObject();
};

