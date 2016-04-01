#include "Enemy.hpp"
#include "Bounds.hpp"
#include "Projectile.hpp"
#include "Side.hpp"
#include "Pickup.hpp"

Enemy::Enemy(b2Body* body, ShapeDef def, std::function<void(SideDef&)>& callback, std::function<Shape*()> &player) :
	Shape(body, def, callback),
	getPlayer_(player),
	collector_(false)
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
	
		//orient(between);
	
		if (between.Length() > 40 || between.Length() < 4.f)
		{
	
		}
	
		else if (between.Length() < 25 && (getHP() >= getHPMax() / 2))
		{
			//move(between);
			//fire(between);
		}
	
		else if (between.Length() < 10 * (getHPMax() - getHP()))
		{
			//move(-between);
		}
	}

	//fire(b2Vec2_zero + body_->GetPosition());
	if (getWeaponReady())
	{
		trigger(b2Vec2(0, 0));
	}

	else
	{
		release();
	}
}

void Enemy::setCollector(bool collect)
{
	collector_ = collect;
}

//Only deals with the effects of this collision on this entity
bool Enemy::collide(Entity * other, b2Contact& contact, std::string tag)
{
	bool handled = false;

	if (tag == "projectile")
	{
		Projectile* proj = static_cast<Projectile*>(other);

		if (proj->getOwner() != this)
		{
			takeDamage(proj->getDamage(), proj->getDirection());
		}

		else contact.SetEnabled(false);

		handled = true;
	}

	else if (tag == "side")
	{
		if (collector_)
		{
			char* tagA = static_cast<char*>(contact.GetFixtureA()->GetUserData());
			char* tagB = static_cast<char*>(contact.GetFixtureB()->GetUserData());

			if (tagA == "side" || tagB == "side")
			{
				Side* side = static_cast<Side*>(other);
				collect(side->getValue());
				side->collect();
			}
		}
		handled = true;
	}


	return handled;
}