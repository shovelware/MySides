//Weapon.hpp:
// Handles firing and cooling logic

#ifndef MS_WEAPON_HPP
#define MS_WEAPON_HPP

#include "Projectile.hpp"

#include <queue>

class Weapon {
public:
	Weapon(Entity* owner);
	
	virtual void trigger(b2Vec2 heading);
	bool pollFired(ProjectileDef& out);
	void clearFired();

	void setProjectile(ProjectileDef pd);
	void setOwner(Entity* owner);

	void update(int milliseconds);
private:
	Entity* owner_;
	virtual void fire(b2Vec2 heading);

	bool canFire();

	ProjectileDef output_;

	int refireTime_;
	int coolDown_;
	
	std::queue<ProjectileDef> fired_;
};

#endif