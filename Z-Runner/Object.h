#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

using namespace std;
using namespace sf;

class Object
{
protected:
	Sprite self;
public:
	Sprite getObject();
};

