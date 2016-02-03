#include "Enemy.hpp"

Enemy::Enemy(b2Body* body, ShapeDef def, std::function<void(SideDef&)>& callback, std::function<Shape*()> &player) : 
	Shape(body, def, callback),
	getPlayer_(player)
{

}

void Enemy::update(int milliseconds)
{
	Shape::update(milliseconds);

	Shape* player = getPlayer_();
	
	if (player)
	{
		b2Vec2 playerPos = player->getPosition();
		b2Vec2 ePos = getPosition();
		b2Vec2 between = playerPos - ePos;

		if (between.Length() > 40 || between.Length() < 4.f)
		{

		}

		else if (between.Length() < 25 && (getHP() >= getHPMax() / 2))
		{
			move(between);
			fire(between);
		}

		else if (between.Length() < 10 * (getHPMax() - getHP()))
		{
			move(-between);
		}
	}

}