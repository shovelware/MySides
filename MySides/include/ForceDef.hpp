#ifndef MS_FORCEDEF_HPP
#define MS_FORCEDEF_HPP

#include <Box2D/Box2D.h>

class ForceDef {
public:
	ForceDef() :
		pos(b2Vec2_zero),
		force(0),
		radius(0),
		lifeTime(0),
		faction(0),
		sides(false),
		shapes(true),
		projectiles(true),
		pickups(false),
		primary(b2Color(1.f, 0.5f, 0.25f)),
		secondary(b2Color(0.5f, 0.5f, 0.25f)),
		tertiary(b2Color(1.f, 1.f, 1.f))
		{}

	ForceDef(const ForceDef& other) :
		pos(other.pos),
		force(other.force),
		radius(other.radius),
		lifeTime(other.lifeTime),
		faction(other.faction),
		sides(other.sides),
		shapes(other.shapes),
		projectiles(other.projectiles),
		pickups(other.pickups),
		primary(other.primary),
		secondary(other.secondary),
		tertiary(other.tertiary)
	{}

	bool isValid() const { return (lifeTime > 0 && radius > 0); }

b2Vec2 pos;
float force;
float radius;

int lifeTime;
int faction;

bool sides;
bool shapes;
bool projectiles;
bool pickups;

b2Color primary;
b2Color secondary;
b2Color tertiary;
};
#endif