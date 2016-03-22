#ifndef MS_PICKUPDEF_HPP
#define MS_PICKUPDEF_HPP

#include <stdafx.h>
#include <Box2D\Common\b2Math.h>

enum Type {
	SIGHT,
	SHIELD,
	ATTRACT,
	WEAPON
};

class PickupDef {
public:
	PickupDef() :
		type(Type::SHIELD),
		position(b2Vec2_zero),
		strength(5.f),
		size(10.f),
		time(20000)
	{}

	PickupDef(Type type, b2Vec2 position, float strength, float size, int time) :
		type(type),
		position(position),
		strength(strength),
		size(size),
		time(time)
	{}

	Type type;
	b2Vec2 position;
	float strength;
	float size;
	int time;

private:
	PickupDef(const PickupDef& pd);
};

#endif