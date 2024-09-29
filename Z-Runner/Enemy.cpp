#include "Enemy.h"

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
		setVelocity(Vector2f(70 * (playerBounds.getPosition().x - self.getPosition().x > 0 ? 1.f : -1.f), velocity.y));
	}

	isFalling = !self.getGlobalBounds().intersects(ground);
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

		if(self.getGlobalBounds().intersects(projectile->getObject().getGlobalBounds()) && !projectile->destroyed) {
			receiveDamage(projectile->getDamage());
			projectile->destruct();
		}
	}

	if (self.getGlobalBounds().intersects(playerBounds)) {
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


	if (velocity.x != 0) {
		self.setTexture(*getNextTexture("move"),true);
		updateSize(size);
		self.setScale(-self.getScale().x*velocity.x / abs(velocity.x), self.getScale().y);
	}

	readyToUpdate = true;

}


Enemy::Enemy(float width, float x, float y, unordered_map<string, vector<Texture*>> &texture)
{
	self.setPosition(x, y);

	this->textures = texture;
	self.setTexture(*texture["idle"][0]);
	
	size = Vector2f(width, width);
	updateSize(size);

	attackingTimer = nullptr;
	updateThread = nullptr;
	attackRate = 2;
}

int Enemy::attackPlayer()
{
	if (!canAttack) return 0;
	delete attackingTimer;
	canAttack = false;
	attackingTimer = new Thread(bind( & Enemy::refreshAttack, this));
	attackingTimer->launch();
	return ATTACK_POWER;
}

void Enemy::update(float deltaTime, vector<Platform> platforms, vector<Projectile*> projectiles, FloatRect playerBounds,FloatRect ground,PathFinder pathFinder)
{
	if (died || !readyToUpdate) return;
	
	//trueUpdate(deltaTime, platforms, projectiles, playerBounds);
	delete updateThread;
	updateThread = new Thread(bind(&Enemy::trueUpdate, this, deltaTime, platforms, projectiles, playerBounds,ground, pathFinder));
	updateThread->launch();
}