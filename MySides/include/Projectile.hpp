#ifndef MS_PROJECTILE_HPP
#define MS_PROJECTILE_HPP

#include "Entity.hpp"

struct ProjectileDef {
public:
	ProjectileDef() :
		body(nullptr),
		origin(b2Vec2_zero),
		heading(b2Vec2_zero),
		inVelocity(b2Vec2_zero),
		velScale(1),
		maxHP(1),
		size(1),
		damage(1),
		damageScale(1),
		lifeTime(1000),
		owner(nullptr),
		target(nullptr)
	{}

	ProjectileDef(b2Vec2 origin, b2Vec2 heading, b2Vec2 inVelocity = b2Vec2_zero) :
		body(nullptr),
		origin(origin),
		heading(heading),
		inVelocity(inVelocity),
		velScale(1),
		maxHP(1),
		size(1),
		damage(1),
		damageScale(1),
		lifeTime(1000),
		owner(nullptr),
		target(nullptr)
	{}

	ProjectileDef(const ProjectileDef& pd) :
		body(nullptr),
		origin(b2Vec2_zero),
		heading(b2Vec2_zero),
		inVelocity(pd.inVelocity),
		velScale(pd.velScale),
		maxHP(pd.maxHP),
		size(pd.size),
		damage(pd.damage),
		damageScale(pd.damageScale),
		lifeTime(pd.lifeTime),
		owner(nullptr),
		target(nullptr)
	{}

	//Some projDef fields should be manually set every time, 
	//to avoid confusion while copying

	//Not copied
	b2Body* body;
	
	//Not copied
	b2Vec2 origin;
	b2Vec2 heading;
	b2Vec2 inVelocity;

	//Velocity handled by projectile
	float velScale;

	unsigned int maxHP;
	float size;

	float damage;
	float damageScale;

	int lifeTime;

	//Not copied
	Entity* owner;
	Entity* target;

	bool isValid() { return body != nullptr; }
};

class Projectile : public Entity{
public:
	Projectile(b2Body* body, b2Vec2 heading);
	Projectile(ProjectileDef& def);

	void fire(float mult);
	void impact();
	void takeDamage(unsigned int damage);
	int getDamage() const;

	Entity* getOwner();
	void setOwner(Entity* o);

	Entity* getTarget();
	void setTarget(Entity* s);

	void update(int milliseconds);
	bool collide(Entity* other, b2Contact& contact);

private:
	void setAsBullet(float size);

	void addMaterial(b2FixtureDef& def); //Abstract into entity?

	float size_;
	float speed_;
	float damage_;

	Entity* owner_;
	Entity* target_;
	b2Vec2 origin_;
	b2Vec2 heading_;

	int lifeTime_;

	bool fired_;
	bool impacted_;

	//Bullets have hp
	//Could take 1 for every impact
	//Simple bounce rounds
	int hp_;
	unsigned int maxHP_;
};

#endif