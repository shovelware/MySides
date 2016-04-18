#include "Enemy.hpp"

std::list<Enemy*> Enemy::swarm_ = std::list<Enemy*>();

Enemy::Enemy(b2Body* body, const EnemyDef& def, std::function<void(SideDef&)>& callback, std::function<Shape*()> &player) :
	Shape(body, def, callback),
	getPlayer_(player),
	aistate(def.ai),
	swarmTarget_(nullptr)
{
	body_->GetFixtureList()->SetUserData("enemy");
	shapeFixDef_.userData = "enemy";

	//Add to swarm
	swarm_.push_back(this);
}

Enemy::~Enemy()
{
	//Remove from swarm
	swarm_.erase(std::find(swarm_.begin(), swarm_.end(), this));

	Shape::~Shape();
}

void Enemy::update(int milliseconds)
{
	Shape::update(milliseconds);
	Shape* player = getPlayer_();
	b2Vec2 pos = getPosition();


	//Don't do AI things if we're not alive
	if (!alive_) return;

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
		if (player)
		{
			b2Vec2 playerPos = player->getPosition();
			b2Vec2 ePos = getPosition();
			b2Vec2 between = playerPos - ePos;

			//In long distance, look
			if (between.Length() < 40 || between.Length() < 4.f)
			{
				orient(between);
			}

			//In med distance, move towards, shoot at
			if (between.Length() < 25 && (getHP() >= verticesMIN_ * hpScale_))
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

			else if (getArmed() && between.Length() < verticesMIN_ * hpScale_)
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

	else if (aistate == 3)
	{
		b2Vec2 sum(0, 0);
		b2Vec2 fire(0, 0);

		int count = 0;
		for (Enemy* v : swarm_)
		{
			if (v != this)
			{
				b2Vec2 between = v->getPosition() - getPosition();
				float dist = between.Length();
				if (dist <= 5.f) {
					sum += LenardJonesPotential(v, count);
				}
			}
		}

		if (player)
		{
			swarmTarget_ = player;
			sum += LenardJonesPotential(player, count);
			//fire = FiringPotential(player);
		}

		//get average
		if (count > 0)
		{
			sum.x /= static_cast<float>(count);
			sum.y /= static_cast<float>(count);
			orient(sum);
			move(sum);

			if (getArmed())
			{
				if (getWeaponReady())
				{
					trigger(fire);
				}
			
				else release();
			
				if (weapon_->getBar() / weapon_->getBarMAX() <= 0.1f)
				{
					reup();
				}
			}
		}
	}
}

b2Vec2 Enemy::LenardJonesPotential(const Shape* const other, int& count) const
{
	static const float A = 100.0f;	//force of attraction
	static const float B = 300.0f;	//force of seperation
	static const float N = 1.0f;	//attraction attenuation
	static const float M = 1.8f;	//seperatation attenuation

									/*
									Lenard-Jones Potential function
									Vector R = me.position - you.position
									Real D = R.magnitude()
									Real U = -A / pow(D, N) + B / pow(D, M)
									R.normalise()
									force = force + R*U
									*/
	b2Vec2 R;
	float D, U;

	R = getPosition() - other->getPosition();
	D = R.Length();

	//if swarming toward target and distance to target is too far away for kamikaze...
	//if (other == swarmTarget_ && D > 100 && D < 600) {
	//	//R = getPosition() - extrapolate(other->getPosition(), other->getForward(), thor::length(other->getVelocity()) * D);
	//	//D = R.Length();
	//}
		if (D > 1)	//1 instead of 0, just in case of rounding errors
		{
			if (other == swarmTarget_)
			{
				//apply no separation to swarmTarget_
				U = (-A / powf(D, N));
			}

			++count;
			{
				U = (-A / powf(D, N)) + (B / powf(D, M));	//Lenard-Jones Potential
			}

			R.Normalize();

			//sum += R*U
			R *= U;
		}

		else
		{

			R = b2Vec2_zero;
		}

		return R;
}

b2Vec2 Enemy::FiringPotential(const Shape* const other) const
{
	float A = 40.f;	//force of attraction
	float B = 0.f;		//force of seperation
	float N = 1.0f;	//attraction attenuation
	float M = 1.8f;	//seperatation attenuation

									/*
									Lenard-Jones Potential function
									Vector R = me.position - you.position
									Real D = R.magnitude()
									Real U = -A / pow(D, N) + B / pow(D, M)
									R.normalise()
									force = force + R*U
									*/
	b2Vec2 R;
	float D, U;

	R = getPosition() - other->getPosition();
	D = R.Length();

	//if swarming toward target and distance to target is too far away for kamikaze...
	if (other == swarmTarget_ && D > 100 && D < 600) {
		//R = getPosition() - extrapolate(other->getPosition(), other->getForward(), thor::length(other->getVelocity()) * D);
		//D = R.Length();
	}
	if (D > 1)	//1 instead of 0, just in case of rounding errors
	{
		if (other == swarmTarget_)
		{
			//apply no separation to swarmTarget_
			U = (-A / powf(D, N));
		}

		{
			U = (-A / powf(D, N)) + (B / powf(D, M));	//Lenard-Jones Potential
		}

		R.Normalize();

		//sum += R*U
		R *= U;
	}

	else
	{

		R = b2Vec2_zero;
	}

	return R;
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

