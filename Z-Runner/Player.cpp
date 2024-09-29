#include "Player.h"

Player::Player()
{
	attackingTimer = nullptr;
}

void Player::initialize(float width, float height, float x, float y, unordered_map<string, vector<Texture*>>& texture)
{
	self.setPosition(x, y);

	this->textures = texture;
	self.setTexture(*texture["idle"][0]);

	size = Vector2f(width, height);
	updateSize(size);

	attackingTimer = nullptr;
	attackRate = 2;
}


Projectile* Player::shootAt(Vector2f target)
{
	delete attackingTimer;
	canAttack = false;
	attackingTimer =new Thread(bind( & Player::refreshAttack, this));
	attackingTimer->launch();

	target -= self.getPosition();
	target = target*(BULLET_SPEED/sqrtf(target.x * target.x + target.y * target.y)) ;
	return new Projectile(10, target, Vector2f(5, 2),self.getPosition(), Color::Yellow,false);
}
