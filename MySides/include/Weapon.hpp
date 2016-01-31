// Weapon.hpp
// Interface, controls firing logic, cooldowns and projectile types for shapes

#ifndef MS_WEAPON_HPP
#define MS_WEAPON_HPP

#include <stdafx.h>
#include <functional>

#include "ProjectileDef.hpp"

class Shape;

namespace Weapon {
	class WeaponI {
	public:

		void setProjectile(ProjectileDef const &pd);
		void setOwner(Shape* owner);

		void trigger(b2Vec2 &direction);

		virtual void update(int dt) = 0;

	protected:
		WeaponI(Shape* owner, std::function<void(ProjectileDef&)>& callback, ProjectileDef const &ammo);
		Shape* owner_;
		ProjectileDef output_;
		std::function<void(ProjectileDef&)> fireCallback_;

		virtual void fire(b2Vec2 &heading) = 0;
		virtual bool canFire() = 0;
	};
}
#include "Shape.hpp"


#endif