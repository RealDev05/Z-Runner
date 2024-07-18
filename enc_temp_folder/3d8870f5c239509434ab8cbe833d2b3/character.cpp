#include "character.h"

int characters::isCollidingWith(RectangleShape shape, bool selfCheck, FloatRect itemToCheck)
{
	FloatRect playerBounds = self.getGlobalBounds();
	FloatRect objectBounds = shape.getGlobalBounds();

	if (!selfCheck) {
		playerBounds = itemToCheck;
	}

	if (!playerBounds.intersects(objectBounds)) return -1;

	float playerRight = playerBounds.left + playerBounds.width;
	float playerBottom = playerBounds.top + playerBounds.height;
	float objectRight = objectBounds.left + objectBounds.width;
	float objectBottom = objectBounds.top + objectBounds.height;

	float overlapLeft = playerRight - objectBounds.left;
	float overlapRight = objectRight - playerBounds.left;
	float overlapTop = playerBottom - objectBounds.top;
	float overlapBottom = objectBottom - playerBounds.top;

	bool fromLeft = overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom;
	bool fromRight = overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom;
	bool fromTop = overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight;
	bool fromBottom = overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight;

	if (fromLeft) {
		return 3;
	}
	else if (fromRight) {
		return 1;
	}
	else if (fromTop) {
		return 0;
	}
	else if (fromBottom) {
		return 2;
	}
}

bool characters::isOnGround(RectangleShape shape)
{
	RectangleShape groundChecker;
	groundChecker.setSize(Vector2f(size.x * 0.9, 10));
	groundChecker.setPosition(self.getPosition().x + size.x * 0.05, self.getPosition().y + size.y - 10);

	return groundChecker.getGlobalBounds().intersects(shape.getGlobalBounds());
}

void characters::updatePosition(Vector2f position)
{
	self.setPosition(position);
}

void characters::updateSize(Vector2f size)
{
	self.setScale(vectorDivide(size, this->size));
}

void characters::setVelocity(Vector2f velocity)
{
}

Vector2f characters::getVelocity()
{
	return velocity;
}
