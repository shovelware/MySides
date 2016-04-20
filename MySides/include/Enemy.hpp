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
		LJVal(float a, float b, float n, float m) :
			attractForce(a),
			sepForce(b),
			attrAtten(n),
			sepAtten(m)
		{}


		float attractForce = 0;
		float sepForce = 0;
		float attrAtten = 0;
		float sepAtten = 0;
	};

	//AI Tests
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

	//Mindist betwen shape and self
	float minDist(Shape* other);
	b2Vec2 between(Shape* other);

	//AI Variables
	State stateOfMind_;
	int startingHP_;
	bool fired_; //If we've fired
	bool panic_;
	AIDef brainStem_;

	//Player awareness
	Shape* player_;
	bool play_ = false;

	//Ranges for actions

	//Flocking range
	float flockRange_ = 10.f; 

	//Temperature control
	float angry_ = 0;
	const static int boilingPoint_ = 50;
	const static int angryMAX_ = 100;
	
	float chill_ = 0;
	const static int freezingPoint_ = -50;
	const static int chillMIN_ = -100;

	//Vision range
	const static float visRange_[5];

	//Chase Player range
	const static float chaseRange_[5];
	//Stop Chase Player range
	const static float chaseRangeMAX_[5];

	//Flee player range
	const static float fearRange_[5];
	//Stop Flee Player range
	const static float fearRangeMIN_[5];

	//Flock attenuation
	const static float flockN_[5];

	//Mood multipliers
	const static float moodMult_[5];

	//Spread of fire
	const static float accuracy_[5];
	
	//Firing cooldown
	const static float triggerSatisfaction_[5];
	
	//Flee health percentage
	const static float ouch_[5];

	//Dance speed
	const static float dance_[5];

	const static LJVal fireLJ;
	const static LJVal sepOnlyLJ;
	const static LJVal flockSameLJ;
	const static LJVal fleeSameLJ;
	const static LJVal fleeOtherLJ;

	//Legay AI
	bool chasing_ = false;
};

#endif