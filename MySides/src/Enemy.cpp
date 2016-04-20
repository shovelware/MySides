#include "Enemy.hpp"

std::list<Enemy*> Enemy::swarm_ = std::list<Enemy*>();

float const Enemy::visRange_[5] = { 16.f, 18.f, 20.f, 22.f, 24.f };

float const Enemy::chaseRange_[5] = { 1.f, 2.f, 4.f, 6.f, 8.f };
float const Enemy::chaseRangeMAX_[5] = { 8.f, 10.f, 12.f, 14.f, 16.f};

float const Enemy::fearRange_[5] = { 0.f, 2.f, 4.f, 6.f, 8.f };
float const Enemy::fearRangeMIN_[5] = { 8.f, 9.f, 10.f, 12.f, 14.f };

float const Enemy::flockN_[5] = { 1.8f, 1.4f, 1.2f, 1.0f, 0.9f };

float const Enemy::moodMult_[5] = { 0.5f, 0.75f, 1.0f, 1.25f, 1.5f };

float const Enemy::accuracy_[5] = { 0.5f, 0.4f, 0.3f, 0.2f, 0.1f };
float const Enemy::triggerSatisfaction_[5] = { 50.f, 20.f, 10.f, 4.f, 1.f };

float const Enemy::ouch_[5] = { 0.5f, 0.4f, 0.2f, 0.1f, 0 };

float const Enemy::dance_[5] = { 0.025f, 0.05f, 0.1f, 0.2f, 0.5f };

Enemy::LJVal const Enemy::sepOnlyLJ = Enemy::LJVal(0, 100, 0, 1.8f);
Enemy::LJVal const Enemy::flockSameLJ = Enemy::LJVal(75, 300, 1.0, 1.8f);
Enemy::LJVal const Enemy::fleeSameLJ = Enemy::LJVal(75, 200, 1.0, 1.8f);
Enemy::LJVal const Enemy::fleeOtherLJ = Enemy::LJVal(75, 100, 0.5, 1.8f);

Enemy::Enemy(b2Body* body, const EnemyDef& def, std::function<void(SideDef&)>& callback, std::function<Shape*()> &player) :
	Shape(body, def, callback),
	getPlayer_(player),
	aistate(def.ai),
	brainStem_(def.brain),
	stateOfMind_(IDLE),
	panic_(false),
	chasing_(false),
	fired_(false)
{
	imprint(brainStem_);
	startingHP_ = getHP();
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

void Enemy::imprint(const AIDef& def)
{
	brainStem_ = def;
	brainStem_.validate();

}

int Enemy::getStateOfMind() const
{
	switch (stateOfMind_)
	{
	case FLEE:	return (int)FLEE;	break;
	case IDLE:	return (int)IDLE;	break;
	case CHASE:	return (int)CHASE;	break;
	}

	return 8;
}

float Enemy::getTemp() const
{
	return chill_ + angry_;
}

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

	//Noveaux Inteilligience Aritificiele
	else execute(ruminate());
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
		if ((player->getPosition() - getPosition()).Length() < visRange_[3])
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

//Behaviour based AI
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
		orient(-toward);
	}
}

//Lennard-Jones based AI
void Enemy::ljpTest()
{
	b2Vec2 sum(0, 0);
	b2Vec2 flock(0, 0);
	b2Vec2 chase(0, 0);
	b2Vec2 fire(0, 0);

	//Chase and fire at player?
	Shape* player = getPlayer_();
	if (player)
	{
		b2Vec2 myPos = getPosition();
		b2Vec2 pPos = player->getPosition();
		b2Vec2 between = (pPos - myPos);
		float dist = between.Length();

		if (!chasing_ && dist < chaseRange_[3])
		{
			chasing_ = true;
		}

		if (chasing_ && dist < chaseRangeMAX_[4])
		{
			chase = LJP(pPos, 100.f, 200.f, 1.0, 1.8);
		}

		else chasing_ = false;

		//Fire if in visible range
		if (dist < visRange_[3])
		{
			fire = LJP(pPos, 100, 0, 2.0, 0);
		}
	}

	if (chasing_) orient(chase);

	//Swarm to shapes
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

			if (between.Length() < flockRange_)
			{
				b2Vec2 steer(0, 0);
				if (!chasing_) steer = LJP(theirPos, 75.f, 300.f, 1.0f, 1.8f);

				else if (v->getVertices() == vertices_) 
					 steer = LJP(theirPos, 0, 100.f, 0, 1.8f);

				else steer = LJP(theirPos, 0, 100.f, 0, 1.8f);

				if (steer.x != 0 && steer.y != 0)
				{
					sum += steer;
					count++;
				}
			}
		}
	}


	//get average
	if (count > 0)
	{
		sum.x /= static_cast<float>(count);
		sum.y /= static_cast<float>(count);
	}

	else sum = b2Vec2_zero;

	//If we didn't want to fire
	float seePlayer = fire.Length();
	if (seePlayer == 0)
	{
		angry_ -= angryMAX_ / triggerSatisfaction_[4];
	}

	else
	{
		orient(fire);
		chill_ = 0;
		angry_ += seePlayer;
	}

	//If we're mad enough
	float ammo = (float)getWeaponBar() / (float)getWeaponBarMAX();
	if (angry_ > angryMAX_ && seePlayer != 0)
	{
		if (fireAt(fire, 0.2f))
		{
			angry_ -= triggerSatisfaction_[4];
		}
		else release();

		
		if (ammo< 0.1f)
			reup();
	}
	angry_ = fmax(angry_, 0);

	//Chill out when idle
	if (angry_ == 0 && seePlayer == 0)
	{
		chill_ -= 1;

		if (chill_ <= chillMIN_)
		{
			if (ammo < 1.f)
				reup();

			spin(-0.05f);
		}
	}
	chill_ = fmax(chill_, chillMIN_);

	move(chase + sum);
}
#pragma endregion

#pragma region Animation
b2Vec2 Enemy::spin(float speed)
{
	b2Vec2 orientation = getOrientation();

	float rotation = atan2f(orientation.y, orientation.x);
	
	b2Vec2 newDir(cosf(rotation + speed), sinf(rotation + speed));
	return newDir;
}
#pragma endregion

#pragma region Finale

//Execute reaction
void Enemy::execute(const Reaction& reaction)
{
	move(reaction.move);

	if (reaction.fire)
		fireAt(reaction.look, accuracy_[brainStem_.weapon_training]); //*// Weapon training

	else orient(reaction.look);
}

//Return a reaction based on state
Enemy::Reaction Enemy::ruminate()
{
	//Update player presence
	player_ = getPlayer_();
	play_ = (player_ != nullptr);

	//Change states based on behaviour
	float hpf = (float)getHP() / (float)(startingHP_);
	
	//Distance to player, if she's there
	float between = FLT_MAX;
	if (play_) between = (getPosition() - player_->getPosition()).Length();

	//If the player's dead, hang ten
	if (!play_) stateOfMind_ = IDLE;

	//Flee if hurt
	else if (hpf < ouch_[brainStem_.pain_tolerance]) stateOfMind_ = FLEE;

	//Otherwise think about what we should do
	else switch (stateOfMind_)
	{
	//If we're running, check if we're healthy enough to stop
	case FLEE:
		if (hpf > ouch_[brainStem_.pain_tolerance]) stateOfMind_ = IDLE;
		
	//If we're chilling, check if the player is messing with the bull
	case IDLE:
		if (play_ && between < 128 && between <= chaseRange_[brainStem_.personal_space] * size_ || angry_ == angryMAX_) stateOfMind_ = CHASE;
	
	//If we're chasing, make sure we can still see him
	case CHASE:
			if (play_ && between > chaseRangeMAX_[brainStem_.perserverance] * size_) stateOfMind_ = IDLE;
	}

	//Execute our current mood
	switch (stateOfMind_)
	{
	case IDLE:
		//std::cout << "idle" << std::endl;
		return idle();
		break;
	case CHASE:
		//std::cout << "chase" << std::endl;
		return chase();
		break;
	case FLEE:
		//std::cout << "flee" << std::endl;
		return flee();
		break;
	}

	return Reaction();
}

//Idle
Enemy::Reaction Enemy::idle()
{
	Reaction r;
	r.move = idleFlock();
	r.fire = idleFire(r.look);

	if (r.look.Length() <= 0)
	{
		r.look = spin(dance_[brainStem_.dancer] * brainStem_.CCW ? -1 : 1);
	}
	
	std::cout << "A" << angry_ << " C " << chill_ << std::endl;

	return r;
}

//Flee
Enemy::Reaction Enemy::flee()
{
	//Can't flee what's not there
	if (!play_) return Reaction();

	b2Vec2 rFleeV(0, 0);
	b2Vec2 rFlockV(0, 0);

	b2Vec2 pPos = player_->getPosition();
	b2Vec2 pVel = player_->getVelocity();
	float dist = (pPos - getPosition()).Length();

	//If we're scared but out of range
	if (panic_ && dist >= fearRangeMIN_[brainStem_.paranoia]) panic_ = false;

	//Or scared and still in range
	else if (panic_ && dist < fearRangeMIN_[brainStem_.paranoia])
	{
		rFleeV = LJP(pPos, 0, 300, 0, 1.0);
	}

	//If we're not scared and she's RIGHT THERE ARGH
	else if (!panic_ && dist < fearRange_[brainStem_.evasiveness])
	{
		rFleeV = LJP(pPos, 0, 300, 0, 1.0);
		panic_ = true;
	}

	rFlockV = fleeFlock();

	Reaction r = Reaction();
	r.move = rFleeV + rFlockV;
	r.look = r.move;
	r.fire = false;

	return r;
}

//Chase
Enemy::Reaction Enemy::chase()
{
	if (chill_ != 0) chill_ = 0;
	b2Vec2 rFlockV(0, 0);
	b2Vec2 rChaseV(0, 0);

	//Chase player?
	if (play_) rChaseV = LJP(player_->getPosition(), 100.f, 200.f, 1.0, 1.5);

	//Gentle flocking
	rFlockV = chaseFlock();

	Reaction r = Reaction();
	r.move = rChaseV + rFlockV;
	r.fire = chaseFire(r.look);

	return r;
}

//Idle flocking
b2Vec2 Enemy::idleFlock()
{	
	b2Vec2 sum(0, 0);
	//Swarm to shapes, similar or no
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

			//Don't flock to cowards, and only those in range
			if (v->getStateOfMind() != FLEE && between.Length() < flockRange_)
			{
				b2Vec2 steer(0, 0);

				//If we're same, attract
				if (v->getVertices() == vertices_)
				{
					LJVal val(flockSameLJ);
					val.attrAtten *= flockN_[brainStem_.friendliness];
					steer = LJP(theirPos, val); //*//Friendliness 
				}
				
				//Otherwise just separate
				else steer = LJP(theirPos, sepOnlyLJ); 

				if (steer.x != 0 && steer.y != 0)
				{
					sum += steer;
					count++;
				}
			}
		}
	}


	//get average
	if (count > 0)
	{
		sum.x /= static_cast<float>(count);
		sum.y /= static_cast<float>(count);
	}

	else sum = b2Vec2_zero;

	return sum;
}

//Chase flocking
b2Vec2 Enemy::chaseFlock()
{	
	b2Vec2 sum(0, 0);

	//Swarm to shapes
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

			if (between.Length() < flockRange_)
			{
				b2Vec2 steer(0, 0);

				if (v->getVertices() == vertices_)
					steer = LJP(theirPos, sepOnlyLJ); //*//friendliness?

				else steer = LJP(theirPos, sepOnlyLJ); //*// personal space?

				if (steer.x != 0 && steer.y != 0)
				{
					sum += steer;
					count++;
				}
			}
		}
	}


	//get average
	if (count > 0)
	{
		sum.x /= static_cast<float>(count);
		sum.y /= static_cast<float>(count);
	}

	else sum = b2Vec2_zero;

	return sum;
}

//Flee flocking
b2Vec2 Enemy::fleeFlock()
{
	b2Vec2 sum(0, 0);
	//Swarm to shapes, similar or no
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

			if (between.Length() < flockRange_)
			{
				b2Vec2 steer(0, 0);

				//If we're same, attract
				if (v->getVertices() == vertices_)
					steer = LJP(theirPos, fleeSameLJ); //*//Friendliness 

				//Otherwise just separate
				else steer = LJP(theirPos, fleeOtherLJ); //*//personal space?

				if (steer.x != 0 && steer.y != 0)
				{
					sum += steer;
					count++;
				}
			}
		}
	}


	//get average
	if (count > 0)
	{
		sum.x /= static_cast<float>(count);
		sum.y /= static_cast<float>(count);
	}

	else sum = b2Vec2_zero;

	return sum;
}

//Idle firing
bool Enemy::idleFire(b2Vec2& rFireV)
{
	bool trigger = false;

	//Did we fire?
	if (fired_)
	{
		angry_ = fmaxf(angry_ - triggerSatisfaction_[brainStem_.weapon_training], 0);
		fired_ = false;
	}

	//Fire at player?
	if (play_)
	{
		b2Vec2 myPos = getPosition();
		b2Vec2 pPos = player_->getPosition();
		b2Vec2 between = (pPos - myPos);
		float dist = between.Length();

		//Fire if in visible range and 
		if (dist < visRange_[brainStem_.sensitivity])
		{
			rFireV = LJP(pPos, 100, 0, 2.0, 0); //*//Fire LJP const?
		}
	}

	//DECIDE TO FIRE
	float seePlayer = rFireV.Length();
	float ammo = (float)getWeaponBar() / (float)getWeaponBarMAX();

	//If we didn't want to fire, ccool down fasst
	if (seePlayer == 0)	angry_ = fmaxf(angry_ - angryMAX_ / triggerSatisfaction_[brainStem_.weapon_training], 0);

	//Otherwise, heat up, chill first
	else
	{
		orient(rFireV);
		if (chill_ != 0) chill_ = fminf(chill_ += seePlayer * moodMult_[brainStem_.aggression] * 2, 0);
		else angry_ = fminf(angry_ + seePlayer * moodMult_[brainStem_.aggression], angryMAX_);
	}

	//If we're mad enough and we can see the player, fire
	if (angry_ > boilingPoint_ && seePlayer != 0)
	{
		trigger = true;
	}

	//Reload if we're cool enough
	if (angry_ - boilingPoint_ > (angryMAX_ - boilingPoint_) * accuracy_[brainStem_.weapon_training] * moodMult_[brainStem_.aggression]) //*//weapon training, aggression
	{
		if (ammo < accuracy_[brainStem_.weapon_training]) //*// weapon training causes later reloads
			reup();
	}

	//Chill out when idle
	if (angry_ == 0 && seePlayer == 0)
	{
		chill_ -= 1 * moodMult_[brainStem_.inner_peace];

		if (chill_ <= freezingPoint_)
		{
			if (ammo < 1.f)
				reup();

			spin(-0.05f);
		}
	}
	chill_ = fmax(chill_, chillMIN_);


	std::cout << seePlayer << "\t" << angry_ << "\t" << chill_ << std::endl;
	return trigger;
}

//Chase firing
bool Enemy::chaseFire(b2Vec2 & rFireV)
{
	bool trigger = false;

	//Did we fire?
	if (fired_)
	{
		angry_ = fmaxf(angry_ - triggerSatisfaction_[brainStem_.weapon_training], 0);
		fired_ = false;
	}

	//Fire at player?
	if (play_)
	{
		b2Vec2 myPos = getPosition();
		b2Vec2 pPos = player_->getPosition();
		b2Vec2 between = (pPos - myPos);
		float dist = between.Length();

		//Fire if in visible range and 
		if (dist < visRange_[brainStem_.sensitivity] && chill_ > freezingPoint_)
		{
			rFireV = LJP(pPos, 100, 0, 2.0, 0); //*//Fire LJP const?
		}
	}

	//DECIDE TO FIRE
	float seePlayer = rFireV.Length();
	float ammo = (float)getWeaponBar() / (float)getWeaponBarMAX();

	//Otherwise, heat up, chill first
	if (seePlayer != 0)
	{
		orient(rFireV);
		if (chill_ != 0) chill_ = fminf(chill_ += seePlayer * 4 * moodMult_[brainStem_.aggression], 0);
		else angry_ = fminf(angry_ + seePlayer * moodMult_[brainStem_.aggression], angryMAX_);
	}

	//If we're mad enough and we can see the player, fire
	if (angry_ > boilingPoint_ && seePlayer != 0)
	{
		trigger = true;
	}

	//Reload if we're cool enough
	if (angry_ - boilingPoint_ > (angryMAX_ - boilingPoint_) * accuracy_[brainStem_.weapon_training] * moodMult_[brainStem_.aggression]) //*//weapon training, aggression
	{
		if (ammo < accuracy_[brainStem_.weapon_training]) //*// weapon training causes later reloads
			reup();
	}

	//Chill out when idle
	if (angry_ == 0 && seePlayer == 0)
	{
		chill_ -= 1 * moodMult_[brainStem_.inner_peace];

		if (chill_ <= freezingPoint_)
		{
			if (ammo < 1.f)
				reup();

			spin(-0.05f);
		}
	}
	chill_ = fmax(chill_, chillMIN_);

	return trigger;
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
	b2Vec2 between = pursuer - getPosition();
	between.Normalize();
	return -between;
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
	b2Vec2 between = pursuer - getPosition();
	float distance = between.Length();
	float speed = body_->GetLinearVelocity().Length();
	int maxTimePrediction = 1000;
	int timePrediction = 1000;

	b2Vec2 steer(0, 0);

	if (speed <= distance / maxTimePrediction) {
		timePrediction = maxTimePrediction;
		steer = arrive(pursuer, dist);
	}

	else
	{
		timePrediction = distance / speed;
		b2Vec2 newPursuer(pVel);
		newPursuer *= timePrediction;
		newPursuer += pursuer;
		steer = flee(newPursuer, dist);
	}

	return steer;
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

#pragma region Combat

bool Enemy::fireAt(const b2Vec2& target, float spread)
{
	if (getWeaponReady())
	{
		float rotation = atan2f(target.y, target.x);
		float adjust = randFloat(-0.4, 0.4);
		b2Vec2 newDir(cosf(rotation + adjust), sinf(rotation + adjust));

		orient(target);
		trigger(newDir);
	
		fired_ = true;
		return true;
	}

	else return false;
}

#pragma endregion

#pragma region Flocking
b2Vec2 Enemy::LenardJonesPotential(const Shape* const other, int& count) const
{
	static const float A = 75.0f;	//force of attraction
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
	float A = attractForce;
	float B = sepForce;
	float N = attrAtten;
	float M = sepAtten;

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
		if (A != 0 && B != 0)
		{
		U = (-A / powf(D, N)) + (B / powf(D, M));
		}
		
		//Attraction only
		else if (A != 0 && B == 0)
		{
			U = (-A / powf(D, N));
		}
		
		//Separation only
		else if (A == 0 && B != 0)
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

b2Vec2 Enemy::flockToSame(float A, float B, float N, float M) const
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

b2Vec2 Enemy::flockToAll(float A, float B, float N, float M) const
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

b2Vec2 Enemy::flockToPlayer(float A, float B, float N, float M) const
{
	b2Vec2 sum(0, 0);

	//If there's no player, return
	if (!play_) return sum;

	int count = 0;
	b2Vec2 myPos = getPosition();
	b2Vec2 theirPos = player_->getPosition();
	b2Vec2 between = theirPos - myPos;
	float minDist = player_->getSize() + size_;
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

b2Vec2 Enemy::flockToPoint(b2Vec2& target, float A, float B, float N, float M) const
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

b2Vec2 Enemy::flockToSame(LJVal val) const { return flockToSame(val.attractForce, val.sepForce, val.attrAtten, val.sepAtten); }
b2Vec2 Enemy::flockToAll(LJVal val) const { return flockToAll(val.attractForce, val.sepForce, val.attrAtten, val.sepAtten); }
b2Vec2 Enemy::flockToPlayer(LJVal val) const { return flockToPlayer(val.attractForce, val.sepForce, val.attrAtten, val.sepAtten); }
b2Vec2 Enemy::flockToPoint(b2Vec2& target, LJVal val) const { return flockToPoint(target, val.attractForce, val.sepForce, val.attrAtten, val.sepAtten); }
b2Vec2 Enemy::LJP(b2Vec2& pos, LJVal val) const { return LJP(pos, val.attractForce, val.sepForce, val.attrAtten, val.sepAtten); }
#pragma endregion

#pragma region Information

float Enemy::minDist(Shape * other)
{
	if (other == nullptr) return 0;
	return other->getSize() + size_;
}

b2Vec2 Enemy::between(Shape * other)
{
	if (other == nullptr) return b2Vec2_zero;
	return other->getPosition() - getPosition();
}

#pragma endregion