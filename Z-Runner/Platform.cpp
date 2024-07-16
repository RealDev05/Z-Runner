#include "Platform.h"
Platform::Platform()
{
}
void Platform::initialize(float width,float height,float x,float y,Color color,bool isPassThrough)
{
	shape.setFillColor(color);
	shape.setSize(Vector2f(width, height));
	shape.setPosition(x, y);
	shape.setOutlineColor(Color::Transparent);
	shape.setOutlineThickness(1.f);
	shape.setOutlineColor(Color::Transparent);

	this->ispassThrough = isPassThrough;
}

void Platform::updateColor(Color color)
{
}

void Platform::updateSize(int width, int height)
{
}

void Platform::updatePosition(float x, float y) {
	shape.setPosition(x, y);
}
Vector2f Platform::getPosition()
{
	return shape.getPosition();
}
RectangleShape Platform::getBoundaryObject() {
	return bounds;
}
RectangleShape Platform::getObject()
{
	return shape;
}
