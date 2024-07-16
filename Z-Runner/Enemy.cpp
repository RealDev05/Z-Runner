#include "Enemy.h"

void Enemy::refreshAttack(int waitTime)
{
	sf::sleep(milliseconds(waitTime));
	canAttack = true;
}

void Enemy::trueUpdate(float deltaTime, vector<Platform> platforms, vector<Projectile*> projectiles,FloatRect playerBounds,FloatRect ground,PathFinder pathFinder)
{
	readyToUpdate = false;

	/*PathFindingNode startNode(0, 900, true);
	PathFindingNode targetNode(100, 900, true);

	std::vector<PathFindingNode*> path = pathFinder.findPath(startNode,targetNode,enemy.getSize().x,enemy.getSize().y,10,2,10000);
	for (int i = 0; i < 10; i++) {

		enemy.setPosition(path[i]->x,path[i]->y);
		cout << enemy.getPosition().x << " x " << enemy.getPosition().x<<endl;
	}*/

	if (canSeePlayer && !isFalling) {
		setVelocity(Vector2f(70 * (playerBounds.getPosition().x - enemy.getPosition().x > 0 ? 1.f : -1.f), velocity.y));
	}

	isFalling = !enemy.getGlobalBounds().intersects(ground);
	for (Platform platform : platforms) {
		int collidingSide = isCollidingWith(platform.getObject(),true);

		isFalling = isFalling && collidingSide != 0;

		if (collidingSide == 0) continue;

		if (collidingSide == 1) {
			setVelocity(Vector2f(velocity.x < 0 ? 0 : velocity.x, velocity.y));
			if(!isFalling) addVelocity(Vector2f(0, -50));
		}
		else if (collidingSide == 3) {
			setVelocity(Vector2f(velocity.x > 0 ? 0 : velocity.x, velocity.y));
			if (!isFalling) addVelocity(Vector2f(0, -100));
		}
		else if (collidingSide == 2 && !platform.ispassThrough) {
			setVelocity(Vector2f(velocity.x, velocity.y < 0 ? 0 : velocity.y));
		}
	}

	for (Projectile* projectile : projectiles) {
		if (projectile->isHostile) continue;

		if(enemy.getGlobalBounds().intersects(projectile->getObject().getGlobalBounds()) && !projectile->destroyed) {
			receiveDamage(projectile->getDamage());
			projectile->destruct();
		}
	}

	if (enemy.getGlobalBounds().intersects(playerBounds)) {
		velocity.x = 0;
	}

	if (isFalling) {
		addVelocity(Vector2f(0, 50) * deltaTime);
		setVelocity(Vector2f(0, velocity.y));
	}

	if (!isFalling && getVelocity().y > 0) {
		setVelocity(Vector2f(getVelocity().x, 0));
	}

	Vector2f newPosition = getPosition() + velocity * deltaTime;
	FloatRect afterMovement(Vector2f(newPosition), getSize());
	for (Platform platform : platforms) {
		int collidingSide = isCollidingWith(platform.getObject(), false,FloatRect(newPosition,getSize()));

		if (collidingSide == 1) {
			newPosition.x = platform.getObject().getSize().x + platform.getObject().getPosition().x;
		}
		else if (collidingSide == 3) {
			newPosition.x = platform.getObject().getPosition().x-getSize().x;
		}
		else if (collidingSide == 2 && !platform.ispassThrough) {
			newPosition.y= platform.getObject().getSize().y + platform.getObject().getPosition().y;
		}
		else if (collidingSide == 0) {
			newPosition.y = platform.getObject().getPosition().y-getSize().y;
		}
	}

	updatePosition(getPosition() + velocity * deltaTime);

	readyToUpdate = true;

}

Enemy::Enemy(float width, float height, float x, float y, Color color)
{
	enemy.setPosition(x, y);
	enemy.setSize(Vector2f(width, height));
	enemy.setFillColor(color);

	attackingTimer = nullptr;
	updateThread = nullptr;
}


int Enemy::isCollidingWith(RectangleShape shape,bool selfCheck,FloatRect itemToCheck)
{
	FloatRect playerBounds = enemy.getGlobalBounds();
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

void Enemy::updatePosition(Vector2f position)
{
	
	enemy.setPosition(position);
}

void Enemy::updateSize(Vector2f size)
{
	enemy.setSize(size);
}

void Enemy::setVelocity(Vector2f velocity)
{
	if (!isFalling && velocity.y > 0) {
		velocity.y = 0;
		this->velocity.y = 0;
	}
	this->velocity = velocity;
}

void Enemy::addVelocity(Vector2f velocity)
{
	if (!isFalling && velocity.y > 0) {
		velocity.y = 0;
		this->velocity.y = 0;
	}
	this->velocity += velocity;
}

int Enemy::attackPlayer()
{
	if (!canAttack) return 0;
	delete attackingTimer;
	canAttack = false;
	attackingTimer = new Thread(bind(&Enemy::refreshAttack, this, 1000 / ATTACK_RATE));
	attackingTimer->launch();
	return ATTACK_POWER;
}

void Enemy::receiveDamage(float damage)
{
	health -= damage;
	died = health <= 0;
}

void Enemy::update(float deltaTime, vector<Platform> platforms, vector<Projectile*> projectiles, FloatRect playerBounds,FloatRect ground,PathFinder pathFinder)
{
	if (died || !readyToUpdate) return;
	
	//trueUpdate(deltaTime, platforms, projectiles, playerBounds);
	delete updateThread;
	updateThread = new Thread(bind(&Enemy::trueUpdate, this, deltaTime, platforms, projectiles, playerBounds,ground, pathFinder));
	updateThread->launch();
}
Vector2f Enemy::getSize()
{
	return enemy.getSize();
}

Vector2f Enemy::getPosition()
{
	return enemy.getPosition();
}

Vector2f Enemy::getVelocity()
{
	return velocity;
}

RectangleShape Enemy::getObject()
{
	return enemy;
}
