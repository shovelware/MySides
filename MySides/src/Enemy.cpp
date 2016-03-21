#include "Enemy.hpp"
#include "Bounds.hpp"
#include "Projectile.hpp"
#include "Side.hpp"

Enemy::Enemy(b2Body* body, ShapeDef def, std::function<void(SideDef&)>& callback, std::function<Shape*()> &player) : 
	Shape(body, def, callback),
	getPlayer_(player)
{
	collector_ = false;
}

void Enemy::update(int milliseconds)
{
	Shape::update(milliseconds);

	Shape* player = getPlayer_();

	//if (player)
	//{
	//	b2Vec2 playerPos = player->getPosition();
	//	b2Vec2 ePos = getPosition();
	//	b2Vec2 between = playerPos - ePos;
	//
	//	orient(between);
	//
	//	if (between.Length() > 40 || between.Length() < 4.f)
	//	{
	//
	//	}
	//
	//	else if (between.Length() < 25 && (getHP() >= getHPMax() / 2))
	//	{
	//		move(between);
	//		//fire(between);
	//	}
	//
	//	else if (between.Length() < 10 * (getHPMax() - getHP()))
	//	{
	//		move(-between);
	//	}
	//}

	//fire(b2Vec2_zero + body_->GetPosition());
}

//Only deals with the effects of this collision on this entity
bool Enemy::collide(Entity * other, b2Contact& contact)
{
	bool handled = false;

	if (Shape* shape = dynamic_cast<Shape*>(other))
	{
		handled = true;
	}

	else if (Projectile* proj = dynamic_cast<Projectile*>(other))
	{
		if (proj->getOwner() != this)
		{
			takeDamage(proj->getDamage(), proj->getDirection());
		}

		else contact.SetEnabled(false);

		handled = true;
	}

	else if (Side* side = dynamic_cast<Side*>(other))
	{
		//char* tagA = static_cast<char*>(contact.GetFixtureA()->GetUserData());
		//char* tagB = static_cast<char*>(contact.GetFixtureB()->GetUserData());
		//
		//if (tagA == "side" || tagB == "side")
		//{
		//	collect(side->getValue());
		//}

		handled = true;
	}

	else if (Bounds* bounds = dynamic_cast<Bounds*>(other))
	{
		handled = true;
	}

	return handled;
}