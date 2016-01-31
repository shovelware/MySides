//Weapon.hpp:
// Handles firing and cooling logic

#ifndef MS_WEAPON_HPP
#define MS_WEAPON_HPP

#include "ProjectileDef.hpp"
#include <stdafx.h>
#include <Box2D\Box2D.h>

///class ProjectileDef;
class Entity; // Forward declarations

class Weapon {
public:
	Weapon(Entity* owner);
	
	virtual void trigger(b2Vec2 &heading);

	void setProjectile(ProjectileDef &pd);
	void setOwner(Entity* owner);

	void update(int milliseconds);
private:
	Entity* owner_;
	virtual void fire(b2Vec2 &heading);

	bool canFire();

	ProjectileDef output_;

	int refireTime_;
	int coolDown_;
};
//#include <ProjectileDef.hpp>
#include <Entity.hpp>
#endif