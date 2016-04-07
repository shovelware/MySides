#include "Enemy.hpp"
#include "Bounds.hpp"
#include "Projectile.hpp"
#include "Side.hpp"
#include "Pickup.hpp"

Enemy::Enemy(b2Body* body, const ShapeDef& def, std::function<void(SideDef&)>& callback, std::function<Shape*()> &player) :
	Shape(body, def, callback),
	getPlayer_(player),
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

		if (getArmed())
		{
			if (getWeaponReady())
			{
				trigger(b2Vec2(0, 0));
			}

			else
			{
				release();
			}
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

			else if (getArmed() && between.Length() < 0.1f * (getHPMax() - getHP()) / hpScale_) 
			{
				orient(-between);
				move(-between);
			}

			else if (!getArmed() && between.Length() < size_ * 1.5f)
			{
				orient(-between);
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
	bool handled = Shape::collide(other, contact, tag);

	if (!handled)
	{

	}

	return handled;
}