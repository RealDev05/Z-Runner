#include "Character.h"

void Character::refreshAttack()
{
	sleep(milliseconds(1000.f/attackRate));
	canAttack = true;
}

Vector2f Character::toVector2f(Vector2u vector)
{
	return Vector2f(vector.x, vector.y);
}

Texture* Character::getNextTexture(string textureName)
{
	try {
		textureIndex[textureName] = (textureIndex[textureName] + 1) % textures[textureName].size();
	}
	catch (exception e) {
		cout << e.what() << endl;
		textureIndex[textureName] = 0;
	}

	return textures[textureName][textureIndex[textureName]];
}

int Character::isCollidingWith(RectangleShape shape, bool selfCheck, FloatRect itemToCheck)
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

bool Character::isOnGround(RectangleShape shape)
{
	RectangleShape groundChecker;
	groundChecker.setSize(Vector2f(size.x * 0.9, 10));
	groundChecker.setPosition(self.getPosition().x + size.x * 0.05, self.getPosition().y + size.y - 10);

	return groundChecker.getGlobalBounds().intersects(shape.getGlobalBounds());
}

void Character::updatePosition(Vector2f position)
{
	self.setPosition(position);
}

void Character::updateSize(Vector2f size)
{
	self.setScale(vectorDivide(size, toVector2f(self.getTexture()->getSize())));
}

void Character::receiveDamage(float damage)
{
	health -= damage;
	died = health <= 0;
}

void Character::setVelocity(Vector2f velocity)
{
	if (!isFalling && velocity.y > 0) {
		velocity.y = 0;
		this->velocity.y = 0;
	}
	this->velocity = velocity;
}

void Character::addVelocity(Vector2f velocity)
{
	if (!isFalling && velocity.y > 0) {
		velocity.y = 0;
		this->velocity.y = 0;
	}
	this->velocity += velocity;
}

Vector2f Character::getSize()
{
	return size;
}

Vector2f Character::getPosition()
{
	return self.getPosition();
}

Vector2f Character::getVelocity()
{
	return velocity;
}
