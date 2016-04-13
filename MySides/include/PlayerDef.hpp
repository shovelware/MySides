#ifndef MS_PLAYERDEF_HPP
#define MS_PLAYERDEF_HPP

#include "ShapeDef.hpp"

class PlayerDef : public ShapeDef
{
public:
	PlayerDef() :
		ShapeDef(ShapeDef()),
		weapon(""),
		weaponLevel(0),
		bombTime(-1),
		bombRadius(0.f)
	{}

	explicit PlayerDef(const ShapeDef& sd) :
		ShapeDef(sd),
		weapon(""),
		weaponLevel(0),
		bombTime(-1),
		bombRadius(0.f)
	{}

	PlayerDef(const PlayerDef& pd) :
		ShapeDef(pd),
		weapon(pd.weapon),
		weaponLevel(pd.weaponLevel),
		bombTime(pd.bombTime),
		bombRadius(pd.bombRadius)
	{}

	std::string weapon;
	int weaponLevel;

	int bombTime;
	float bombRadius;
};

#endif