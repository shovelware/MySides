#ifndef MS_ENEMY_HPP
#define MS_ENEMY_HPP

#include <list>
#include <functional>

#include "Shape.hpp"
#include "EnemyDef.hpp"

class Enemy : public Shape
{
public:
	Enemy(b2Body* body, const EnemyDef& def, std::function<void(SideDef&)>& callback, std::function<Shape*()> &player);
	~Enemy();

	void update(int milliseconds);
	bool collide(Entity* other, b2Contact& contact, std::string tag);

	//Information
	float getRange() const;
	int getVertices() const;

	//AI Functions
	void blindFire();
	void classicAI();
	void flockTest();
	void behavTest();
	void ljpTest();

	//AI Behaviours
	b2Vec2 seek(b2Vec2 target, float dist = 0);		//!< Max Speed to target
	b2Vec2 arrive(b2Vec2 target, float dist = 0);	//!< Adjust speed to target
	b2Vec2 flee(b2Vec2 pursuer, float dist = 0);	//!< Max speed from target

	b2Vec2 pursue(b2Vec2 target, b2Vec2 tVel = b2Vec2_zero, float dist = 0);	//!< Predict target position and go there
	b2Vec2 evade(b2Vec2 pursuer, b2Vec2 pVel = b2Vec2_zero, float dist = 0);	//!< Predict target position and go away from there

	b2Vec2 wander(b2Vec2 curDir, float wanderRate);

	//Flocking behaviours
	b2Vec2 flockToSame(float A, float B, float N, float M);
	b2Vec2 flockToAll(float A, float B, float N, float M);
	b2Vec2 flockToPlayer(float A, float B, float N, float M);
	b2Vec2 flockToPoint(b2Vec2 target, float A, float B, float N, float M);

	b2Vec2 Enemy::LenardJonesPotential(const Shape* const other, int& count) const;
	b2Vec2 Enemy::LJP(b2Vec2& pos, float A, float B, float N, float M) const;

	//Debug
	int aistate;

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

	void ai3();
	void ai4();
	void ai5();
	void ai6();
	void ai7();
	void ai8();
	void ai9();

	float minDist(Shape* other);
	b2Vec2 between(Shape* other);

	float visRange_ = 10.f;
};

#endif