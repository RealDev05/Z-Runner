#include "Projectile.h"

Projectile::Projectile(float damage, Vector2f velocity, Vector2f size,Vector2f init, Color color,bool isHostile)
{
	this->damage = damage;
	this->velocity = velocity;
	this->isHostile = isHostile;
	projectile.setSize(size);
	projectile.setFillColor(color);
	projectile.setPosition(init);

	updateThread = nullptr;
}

void Projectile::update(float deltaTime, vector<RectangleShape> shapes)
{
	if (destroyed) {
		return;
	}
	projectile.setPosition(projectile.getPosition()+velocity * deltaTime);
	projectile.setRotation(atan2f(velocity.y, velocity.x) * 57.3f);
	delete updateThread;
	updateThread = new Thread(bind(&Projectile::checkForCollision, this, shapes));
	updateThread->launch();
}
void Projectile::checkForCollision(vector<RectangleShape> shapes)
{
	for (const RectangleShape& shape : shapes) {
		float rect1[4] = { projectile.getPosition().x,projectile.getPosition().y,projectile.getSize().x,projectile.getSize().y };
		rect1[2] += rect1[0];
		rect1[3] += rect1[1];

		float rect2[4] = { shape.getPosition().x,shape.getPosition().y,shape.getSize().x,shape.getSize().y };
		rect1[2] += rect1[0];
		rect1[3] += rect1[1];
		if (projectile.getGlobalBounds().intersects(shape.getGlobalBounds()) || (rect1[0] > rect2[0] && rect1[0]<rect2[2] && rect1[1]>rect2[1] && rect1[1] < rect2[3])) {
			destruct();
		}
	}
	
}

void Projectile::destruct()
{
	destroyed = true;
	velocity=Vector2f(0,0);
}

float Projectile::getDamage()
{
	return damage;
}

RectangleShape Projectile::getObject()
{
	return projectile;
}
