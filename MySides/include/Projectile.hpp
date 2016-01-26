#ifndef MS_PROJECTILE_HPP
#define MS_PROJECTILE_HPP

#include "Entity.hpp"
#include "ProjectileDef.hpp"

class Projectile : public Entity{
public:
	Projectile(b2Body* body, b2Vec2 heading);
	Projectile(b2Body* body, ProjectileDef& def);

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
	void setAsBullet(float size, float damageScale);

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