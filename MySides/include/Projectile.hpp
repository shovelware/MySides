#ifndef MS_PROJECTILE_HPP
#define MS_PROJECTILE_HPP

#include "Entity.hpp"
#include "ProjectileDef.hpp"

class Projectile : public Entity{
public:
	Projectile(b2Body* body, const ProjectileDef& def);

	void fire(float mult);

	void takeDamage(unsigned int damage);
	int getDamage() const;
	int getShrapnel() const;

	Entity* getOwner();
	void setOwner(Entity* o);

	Entity* getTarget();
	void setTarget(Entity* s);

	b2Vec2 getDirection() const;

	void update(int milliseconds);
	bool collide(Entity* other, b2Contact& contact);

private:
	void setAsCircle(b2Vec2 size, float bounce, bool ghost);
	void setAsRect(b2Vec2 size, float bounce, bool ghost);

	void addMaterial(b2FixtureDef& def, float bounce); //Abstract into entity?

	b2Vec2 size_;
	float speed_;
	float damage_;
	int shrapnel_;

	Entity* owner_;
	Entity* target_;
	b2Vec2 origin_;
	b2Vec2 heading_;

	int lifeTime_;

	bool fired_;
	bool oneHit_;

	//Bullets have hp
	//Could take 1 for every impact
	//Simple bounce rounds
	int hp_;
	unsigned int maxHP_;
};

#endif