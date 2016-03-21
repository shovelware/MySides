// Weapon.hpp
// Interface, controls firing logic, cooldowns and projectile types for shapes

#ifndef MS_WEAPON_HPP
#define MS_WEAPON_HPP

#include <stdafx.h>
#include <vector>
#include <functional>

#include "ProjectileDef.hpp"

//!	@class	Weapon
//!	@brief	Controls firing of projectiles
//!	@see	Projectile, ProjectileDef

//!		Abstract Weapon interface that provides base functionality for
//!		weapons such as colours, a trigger for firing and time-based updates.

class Shape;

namespace Weapon {
	class WeaponI {
	public:
		void trigger(b2Vec2 &direction);	//!< Pulls the trigger on the weapon. The only exposed piece!

		void setProjectile(ProjectileDef const &pd);	//!< Sets the passed Projectile to be the Weapon's output

		void setOwner(Shape* owner);	//!< Sets the owner of the Weapon to be a passed Shape
		Shape* getOwner() const;		//!< Gets the owner of the weapon

		b2Color getPrimary() const;		//!< Gets Primary Colour 
		b2Color getSecondary() const;	//!< Gets Secondary Colour
		b2Color getTertiary() const;	//!< Gets Tertiary Colour 

		void setPrimary(b2Color col);	//!< Sets Primary Colour 
		void setSecondary(b2Color col);	//!< Sets Secondary Colour
		void setTertiary(b2Color col);	//!< Sets Tertiary Colour 

		std::string getID();

		virtual bool canFire() = 0;		//!< Checks if a Weapon can fire

		virtual void update(int dt) = 0;	//!< Updates weapon

	protected:
		WeaponI(std::function<void(std::vector<ProjectileDef>& defs, std::string id)>& callback, ProjectileDef const &ammo);

		std::string id_;
		Shape* owner_;

		ProjectileDef output_;
		std::function<void(std::vector<ProjectileDef>& defs, std::string id)> fireCallback_;

		virtual void fire(b2Vec2 &heading) = 0;
	};
}
#include "Shape.hpp"


#endif