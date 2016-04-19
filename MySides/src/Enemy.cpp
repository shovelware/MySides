#include "Enemy.hpp"

std::list<Enemy*> Enemy::swarm_ = std::list<Enemy*>();

Enemy::Enemy(b2Body* body, const EnemyDef& def, std::function<void(SideDef&)>& callback, std::function<Shape*()> &player) :
	Shape(body, def, callback),
	getPlayer_(player),
	aistate(def.ai)
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

//Gets current weapon's range
float Enemy::getRange() const {	return (getArmed() ? weapon_->getRange() : 0.f); }

//Gets the current vertices
int Enemy::getVertices() const { return vertices_; }

void Enemy::update(int milliseconds)
{
	Shape::update(milliseconds);

	//Don't do AI things if we're not alive
	if (!alive_) return;

	//Still
	if (aistate == 0);

	//Blind fire
	else if (aistate == 1) blindFire();

	//Classic AI
	else if (aistate == 2) classicAI();

	//Testing funcs
	else if (aistate == 3) ai3();
	else if (aistate == 4) ai4();
	else if (aistate == 5) ai5();
	else if (aistate == 6) ai6();
	else if (aistate == 7) ai7();
	else if (aistate == 8) ai8();
	else if (aistate == 9) ai9();
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

#pragma region Testing

void Enemy::ai3()
{
	b2Vec2 steer;
	Shape* player = getPlayer_();
	float dist = visRange_ * 2;
	
	if (player)
	{
		dist = (player->getPosition() - getPosition()).Length();

		if (dist < visRange_)
		{
			steer = pursue(player->getPosition(), player->getVelocity(), minDist(player));
		}
	}

	if (dist > visRange_)
	{
		steer = wander(getOrientation(), 1.f);
	}

	move(steer);
	orient(steer);
}

void Enemy::ai4()
{}

void Enemy::ai5()
{}

void Enemy::ai6()
{}

void Enemy::ai7()
{}

void Enemy::ai8()
{}

void Enemy::ai9()
{}

#pragma endregion

#pragma region AI Profiles
//Fire forward and brake
void Enemy::blindFire()
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

//Towards player, shoot if close, run if low hp
void Enemy::classicAI()
{
	Shape* player = getPlayer_();
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

//Flocking together
void Enemy::flockTest()
{
	b2Vec2 sum(0, 0);
	b2Vec2 fire(0, 0);

	int count = 0;
	for (Enemy* v : swarm_)
	{
		if (v != this && v->getVertices() == vertices_)
		{
			b2Vec2 between = v->getPosition() - getPosition();
			float dist = between.Length();
			if (dist <= 5.f) {
				sum += LenardJonesPotential(v, count);
			}
		}
	}

	Shape* player = getPlayer_();
	if (player)
	{
		if ((player->getPosition() - getPosition()).Length() < visRange_)
		{
			//swarmTarget_ = player;
			//sum += LenardJonesPotential(player, count);
			//fire = FiringPotential(player);
		}
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

			if ((weapon_->getBar() / weapon_->getBarMAX()) <= 0.1f)
			{
				reup();
			}
		}
	}
}

//AI 4
void Enemy::behavTest()
{
	Shape* player = getPlayer_();
	if (player)
	{
		b2Vec2 pos = player->getPosition();
		b2Vec2 toward = getPosition() - pos;
		float dist = (player->getSize() + size_) * 1.5f;
		b2Vec2 vel = player->getBody()->GetLinearVelocity();

		b2Vec2 steer = pursue(pos, vel, dist);
		move(steer);
		orient(toward);
	}
}

//AI 5
void Enemy::ljpTest()
{
	b2Vec2 sum(0, 0);
	b2Vec2 fire(0, 0);

	int count = 0;
	for (Enemy* v : swarm_)
	{
		if (v != this)
		{
			b2Vec2 myPos = getPosition();
			b2Vec2 theirPos = v->getPosition();
			b2Vec2 between = theirPos - myPos;
			float minDist = v->getSize() + size_;
			float dist = between.Length();

			if (v->getVertices() == vertices_)
			{
				b2Vec2 steer = LJP(theirPos, 100, 300, 1.0, 1.8);
				
				if (steer.x != 0 && steer.y != 0)
				{
					sum += steer;
					count++;
				}
			}
		}
	}

	Shape* player = getPlayer_();
	if (player)
	{
		b2Vec2 myPos = getPosition();
		b2Vec2 pPos = player->getPosition();

		if ((pPos - myPos).Length() < visRange_)
		{
			b2Vec2 steer = LJP(player->getPosition(), 100, 100, 1.0, 1.8);

			if (steer.x != 0 && steer.y != 0)
			{
				sum += LJP(pPos, 100, 0, 100, 0);
			}
			//fire = FiringPotential(player);
		}
	}

	//get average
	if (count > 0)
	{
		sum.x /= static_cast<float>(count);
		sum.y /= static_cast<float>(count);
		orient(sum);
		move(sum);
	}
}
#pragma endregion

#pragma region Behaviours
b2Vec2 Enemy::seek(b2Vec2 target, float dist)
{
	b2Vec2 between = target - getPosition();
	between.Normalize();
	return between;
}

b2Vec2 Enemy::arrive(b2Vec2 target, float dist)
{
	b2Vec2 between = target - getPosition();

	//Far away, max speed
	if (between.Length() > dist)
		between.Normalize();

	//Really close, stop
	else if (between.Length() < dist)
	{
		between = b2Vec2_zero;
	}

	//In between, scale speed
	else
	{
		float len = between.Length() / maxVel_;
		between.Normalize();
		between *= len;
	}

	return between;
}

b2Vec2 Enemy::flee(b2Vec2 pursuer, float dist)
{
	b2Vec2 between = getPosition() - pursuer;
	between.Normalize();
	return between;
}

b2Vec2 Enemy::pursue(b2Vec2 target, b2Vec2 tVel, float dist)
{
	b2Vec2 between = target - getPosition();
	float distance = between.Length();
	float speed = body_->GetLinearVelocity().Length();
	int maxTimePrediction = 1000;
	int timePrediction = 1000;

	b2Vec2 steer(0, 0);

	if (speed <= distance / maxTimePrediction) {
		timePrediction = maxTimePrediction;
		steer = arrive(target, dist);
	}

	else
	{
		timePrediction = distance / speed;
		b2Vec2 newTarget(tVel);
		newTarget *= timePrediction;
		newTarget += target;
		steer = arrive(newTarget, dist);
	}
	
	return steer;
}

b2Vec2 Enemy::evade(b2Vec2 pursuer, b2Vec2 pVel, float dist)
{
	return b2Vec2();
}

b2Vec2 Enemy::wander(b2Vec2 curDir, float wanderRate)
{
	b2Vec2 wanderOrientation = curDir;
	float rotation = atan2f(wanderOrientation.y, wanderOrientation.x);
	rotation += randFloat(-0.1f, 0.1f) * wanderRate;

	b2Vec2 target(0, 0);

	target.x = cosf(rotation);
	target.y = sinf(rotation);

	target *= (size_ * 2.f);
	target += getPosition();

	return target;
}
#pragma endregion

#pragma region Flocking
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
		//if (other == swarmTarget_)
		//{
		//	//apply no separation to swarmTarget_
		//	U = (-A / powf(D, N));
		//}

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

b2Vec2 Enemy::LJP(b2Vec2& pos, float attractForce, float sepForce, float attrAtten, float sepAtten) const
{
	int A = attractForce;
	int B = sepForce;
	int N = attrAtten;
	int M = sepAtten;

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

	R = getPosition() - pos;
	D = R.Length();

	if (D > 1)	//1 instead of 0, just in case of rounding errors
	{
		//Lenard-Jones Potential
		if (A > 0 && B > 0)
		{
			U = (-A / powf(D, N)) + (B / powf(D, M));
		}

		//Attraction only
		else if (A > 0 && B <= 0)
		{
			U = (-A / powf(D, N));
		}

		//Separation only
		else if (A <= 0 && B > 0)
		{
			U = (B / powf(D, M));
		}

		//No Forces, no movement 
		else U = 0;

		R.Normalize();

		R *= U;
	}

	else
	{
		R = b2Vec2_zero;
	}

	return R;
}

b2Vec2 Enemy::flockToSame(float A, float B, float N, float M)
{
	b2Vec2 sum(0, 0);

	int count = 0;
	for (Enemy* v : swarm_)
	{
		if (v != this)
		{
			b2Vec2 myPos = getPosition();
			b2Vec2 theirPos = v->getPosition();
			b2Vec2 between = theirPos - myPos;
			float minDist = v->getSize() + size_;
			float dist = between.Length();

			if (v->getVertices() != vertices_) continue;
			b2Vec2 steer = LJP(theirPos, A, B, N, M);
			if (steer.x != 0 && steer.y != 0)
			{
				sum += steer;
				count++;
			}
		}
	}

	//get average
	if (count > 0)
	{
		sum.x /= static_cast<float>(count);
		sum.y /= static_cast<float>(count);
	}

	return sum;
}

b2Vec2 Enemy::flockToAll(float A, float B, float N, float M)
{
	b2Vec2 sum(0, 0);

	int count = 0;
	for (Enemy* v : swarm_)
	{
		if (v != this)
		{
			b2Vec2 myPos = getPosition();
			b2Vec2 theirPos = v->getPosition();
			b2Vec2 between = theirPos - myPos;
			float minDist = v->getSize() + size_;
			float dist = between.Length();

			b2Vec2 steer = LJP(theirPos, A, B, N, M);

			if (steer.x != 0 && steer.y != 0)
			{
				sum += steer;
				count++;
			}
		}
	}

	//get average
	if (count > 0)
	{
		sum.x /= static_cast<float>(count);
		sum.y /= static_cast<float>(count);
	}

	return sum;
}

b2Vec2 Enemy::flockToPlayer(float A, float B, float N, float M)
{
	b2Vec2 sum(0, 0);

	Shape* player = getPlayer_();

	//If there's no player, return
	if (!player) return sum;

	int count = 0;
	b2Vec2 myPos = getPosition();
	b2Vec2 theirPos = player->getPosition();
	b2Vec2 between = theirPos - myPos;
	float minDist = player->getSize() + size_;
	float dist = between.Length();

	b2Vec2 steer = LJP(theirPos, A, B, N, M);

	if (steer.x != 0 && steer.y != 0)
	{
		sum += steer;
		count++;
	}

	//get average
	if (count > 0)
	{
		sum.x /= static_cast<float>(count);
		sum.y /= static_cast<float>(count);
	}

	return sum;
}

b2Vec2 Enemy::flockToPoint(b2Vec2 target, float A, float B, float N, float M)
{
	b2Vec2 sum(0, 0);

	int count = 0;
	b2Vec2 myPos = getPosition();
	b2Vec2 theirPos = target;
	b2Vec2 between = theirPos - myPos;
	float minDist = size_;
	float dist = between.Length();

	b2Vec2 steer = LJP(theirPos, A, B, N, M);

	if (steer.x != 0 && steer.y != 0)
	{
		sum += steer;
		count++;
	}
	
	//get average
	if (count > 0)
	{
		sum.x /= static_cast<float>(count);
		sum.y /= static_cast<float>(count);
	}

	return sum;
}
#pragma endregion

#pragma region Information

float Enemy::minDist(Shape * other)
{
	return other->getSize() + size_;
}

b2Vec2 Enemy::between(Shape * other)
{
	return other->getPosition() - getPosition();
}

#pragma endregion