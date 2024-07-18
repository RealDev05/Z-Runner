#include "Player.h"

void Player::refreshShooting(int waitTime)
{
	sf::sleep(milliseconds(waitTime));
	canShoot = true;
}

Player::Player()
{
	shootingTimer = nullptr;
}

void Player::initialize(float width, float height, float x, float y, Color color)
{
	player.setPosition(x, y);
	player.setSize(Vector2f(width, height));
	player.setFillColor(color);
}

bool Player::isOnGround(RectangleShape shape) {

	RectangleShape groundChecker;
	groundChecker.setSize(Vector2f(player.getSize().x * 0.9, 10));
	groundChecker.setPosition(player.getPosition().x + player.getSize().x * 0.05, player.getPosition().y + player.getSize().y - 10);

	return groundChecker.getGlobalBounds().intersects(shape.getGlobalBounds());
}


int Player::isCollidingWith(RectangleShape shape)
{
	FloatRect playerBounds = player.getGlobalBounds();
	FloatRect objectBounds = shape.getGlobalBounds();

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
void Player::updatePosition(Vector2f position)
{
	player.setPosition(position);
}

void Player::updateSize(Vector2f size)
{
	player.setSize(size);
}

void Player::receiveDamage(float damage)
{
	health -= damage;
	isDead = health <= 0;
}

void Player::setVelocity(Vector2f velocity)
{
	if (!isFalling && velocity.y>0) {
		velocity.y = 0;
		this->velocity.y = 0;
	}
	this->velocity = velocity;
}

void Player::setColor(Color color)
{
	player.setFillColor(color);
}

void Player::addVelocity(Vector2f velocity)
{
	if (!isFalling && velocity.y > 0) {
		velocity.y = 0;
		this->velocity.y = 0;
	}

	if (velocity.x < -MOVEMENT_SPEED * 2) {
		velocity.x = -MOVEMENT_SPEED * 2;
	}
	else if(velocity.x> MOVEMENT_SPEED * 2){
		velocity.x = MOVEMENT_SPEED * 2;
	}
	this->velocity += velocity;
}

Projectile* Player::shootAt(Vector2f target)
{
	delete shootingTimer;
	canShoot = false;
	shootingTimer =new Thread(bind(& Player::refreshShooting, this, 1000 /SHOOTING_RATE));
	shootingTimer->launch();

	target -= player.getPosition();
	target = target*(BULLET_SPEED/sqrtf(target.x * target.x + target.y * target.y)) ;
	return new Projectile(10, target, Vector2f(5, 2),player.getPosition(), Color::Yellow,false);
}

Vector2f Player::getSize()
{
	return player.getSize();
}

Vector2f Player::getPosition()
{
	return player.getPosition();
}

Vector2f Player::getVelocity()
{
	return velocity;
}

RectangleShape Player::getObject()
{
	return player;
}
