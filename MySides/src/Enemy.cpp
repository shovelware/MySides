#include "Enemy.hpp"
#include "Bounds.hpp"
#include "Projectile.hpp"
#include "Side.hpp"
#include "Pickup.hpp"

Enemy::Enemy(b2Body* body, ShapeDef def, std::function<void(SideDef&)>& callback, std::function<Shape*()> &player) :
	Shape(body, def, callback),
	getPlayer_(player),
	collector_(false),
	aistate(def.ai)
{
	body_->GetFixtureList()->SetUserData("enemy");
	shapeFixDef_.userData = "enemy";
}

void Enemy::update(int milliseconds)
{
	Shape::update(milliseconds);

	//Still
	if (aistate == 0);


	//Blind fire
	else if (aistate == 1)
	{
		move(b2Vec2_zero);

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

	//Classic AI
	else if (aistate == 2)
	{
		Shape* player = getPlayer_();

		if (player)
		{
			b2Vec2 playerPos = player->getPosition();
			b2Vec2 ePos = getPosition();
			b2Vec2 between = playerPos - ePos;

			if (between.Length() < 40 || between.Length() < 4.f)
			{
				orient(between);
			}

			if (between.Length() < 25 && (getHP() >= getHPMax() / 2))
			{
				if (getArmed())
				{
					move(between);
					orient(between);
					if (getWeaponReady())
					{

						float rotation = atan2f(between.y, between.x);
						float adjust = randFloat(-0.4, 0.4);
						b2Vec2 newDir(cosf(rotation + adjust), sinf(rotation + adjust));
						trigger(between);
					}
					else release();
				}
			}

			else if (between.Length() < 1.f * (getHPMax() - getHP()) / hpScale_)
			{
				orient(-between);
				move(-between);
			}

			else if (!getArmed() && between.Length() < 0.25f * (getHPMax() - getHP()) / hpScale_) 
			{
				orient(-between);
				move(-between);
			}

			else move(b2Vec2_zero);
		}
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
			if (alive_ == false)
				contact.SetEnabled(false);
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