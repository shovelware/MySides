#ifndef MS_ENEMY_HPP
#define MS_ENEMY_HPP

#include <list>
#include <functional>

#include "Shape.hpp"
#include "EnemyDef.hpp"

class Enemy : public Shape
{
public:

	struct Reaction {
		b2Vec2 move = b2Vec2(0, 0);
		b2Vec2 look = b2Vec2(0, 0);
		bool fire = false;
	};

	enum State {
		FLEE = -1,
		IDLE = 0,
		CHASE = 1
	};

	Enemy(b2Body* body, const EnemyDef& def, std::function<void(SideDef&)>& callback, std::function<Shape*()> &player);
	~Enemy();

	void update(int milliseconds);
	bool collide(Entity* other, b2Contact& contact, std::string tag);

	//Information
	float getRange() const;
	int getVertices() const;

	//AIDef
	void imprint(const AIDef& def);

	//Debug
	int aistate;
	int getStateOfMind() const;
	float getTemp() const;

private:
	static std::list<Enemy*> swarm_; //List of all enemies
	std::function<Shape*()>& getPlayer_; //Gettable player

	//Struct for holding flocking vars ? 
	struct LJVal {
		float attractForce = 0;
		float sepForce = 0;
		float attrAtten = 0;
		float sepAtten = 0;
	};

	//AI Profiles
	void blindFire();
	void classicAI();
	void flockTest();
	void behavTest();
	void ljpTest();

	//AI Animaton
	b2Vec2 spin(float speed);

	//Finale
	void execute(const Reaction& reaction);
	Reaction ruminate();
	Reaction flee();
	Reaction idle();
	Reaction chase();

	b2Vec2 fleeFlock();
	b2Vec2 idleFlock();
	b2Vec2 chaseFlock();

	bool idleFire(b2Vec2& rFireV);
	bool chaseFire(b2Vec2& rFireV);

	//AI Behaviours
	b2Vec2 seek(b2Vec2 target, float dist = 0);		//!< Max Speed to target
	b2Vec2 arrive(b2Vec2 target, float dist = 0);	//!< Adjust speed to target
	b2Vec2 flee(b2Vec2 pursuer, float dist = 0);	//!< Max speed from target

	b2Vec2 pursue(b2Vec2 target, b2Vec2 tVel = b2Vec2_zero, float dist = 0);	//!< Predict target position and go there
	b2Vec2 evade(b2Vec2 pursuer, b2Vec2 pVel = b2Vec2_zero, float dist = 0);	//!< Predict target position and go away from there

	b2Vec2 wander(b2Vec2 curDir, float wanderRate);

	//Combat
	bool fireAt(const b2Vec2& target, float spread);

	//Flocking behaviours
	b2Vec2 flockToSame(float A, float B, float N, float M) const;
	b2Vec2 flockToAll(float A, float B, float N, float M) const;
	b2Vec2 flockToPlayer(float A, float B, float N, float M) const;
	b2Vec2 flockToPoint(b2Vec2& target, float A, float B, float N, float M) const;
	b2Vec2 LJP(b2Vec2& pos, float A, float B, float N, float M) const;

	b2Vec2 flockToSame(LJVal val) const;
	b2Vec2 flockToAll(LJVal val) const;
	b2Vec2 flockToPlayer(LJVal val) const;
	b2Vec2 flockToPoint(b2Vec2& target, LJVal val) const;
	b2Vec2 LJP(b2Vec2& pos, LJVal val) const;

	b2Vec2 LenardJonesPotential(const Shape* const other, int& count) const;

	//Testing ai
	void ai3();
	void ai4();
	void ai5();
	void ai6();
	void ai7();
	void ai8();
	void ai9();

	//Mindist betwen shape and self
	float minDist(Shape* other);
	b2Vec2 between(Shape* other);

	//AI Variables
	State stateOfMind_;

	AIDef brainStem_;

	//Ranges for actions
	float flockRange_ = 10.f; //Flocking range
	float visRange_ = 20.f; //Vision range

	float chaseRange_ = 10.f; //Chase Player range
	float chaseMAXRange_ = 20.f; //Stop Chase Player range

	bool panic_ = false;
	float fearRange_ = 10.f; //Flee player range
	float fearRangeMIN_ = 15; //Stop Flee Player range

	float minDist_ = 0;

	float accuracy_ = 0.4f; //Spread of fire
	bool fired_; //If we've fired
	
	//LastDamage in enemy
	int startingHP_;

	//Temperature control
	float angry_ = 0;
	float boilingPoint_ = 50.f;
	float angryMAX_ = 100.f;
	
	float triggerSatisfaction_ = 4.f;

	float chill_ = 0;
	float freezingPoint_ = -50.f;
	float chillMIN_ = -100.f;

	float ouch_ = 0.2f; //When to flee

	bool chasing_ = false;

	Shape* player_;
	bool play_ = false;
};

#endif