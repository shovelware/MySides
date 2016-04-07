// Weapon.hpp
// Interface, controls firing logic, cooldowns and projectile types for shapes

#ifndef MS_WEAPON_HPP
#define MS_WEAPON_HPP

#include <stdafx.h>
#include <vector>
#include <functional>

#include <Box2D\Common\b2Draw.h>

#include "ProjectileDef.hpp"

//!	@class	Weapon
//!	@brief	Controls firing of projectiles
//!	@see	Projectile, ProjectileDef

//!		Abstract Weapon interface that provides base functionality for
//!		weapons such as colours, a trigger for firing, reloads and time-based updates.

class Shape;

namespace Weapon {
	typedef std::function<void(std::vector<ProjectileDef>& defs, std::string id)> FireFunc;

	class WeaponI {
	public:
		void trigger(b2Vec2 &direction);	//!< Pulls the trigger on the weapon. The only exposed piece!
		void release();						//!< Releases the trigger, for weapons that need it

		virtual void reup(bool instant = false) = 0;//!< Manual reload, if available
		void setProjectile(ProjectileDef const &pd);//!< Sets the passed Projectile to be the Weapon's output

		void setLevel(int level);		//!< Sets weapon level
		int getLevel() const;			//!< Gets weapon levelw

		void setOwner(Shape* owner);	//!< Sets the owner of the Weapon to be a passed Shape
		Shape* getOwner() const;		//!< Gets the owner of the weapon

		b2Color getPrimary() const;		//!< Gets Primary Colour 
		b2Color getSecondary() const;	//!< Gets Secondary Colour
		b2Color getTertiary() const;	//!< Gets Tertiary Colour 

		void setPrimary(b2Color col);	//!< Sets Primary Colour 
		void setSecondary(b2Color col);	//!< Sets Secondary Colour
		void setTertiary(b2Color col);	//!< Sets Tertiary Colour 

		void setID(std::string id);		//!< Sets a new id for the weapon
		std::string getID();			//!< Gets the id of the weapon

		void setFaction(int faction); //!< Sets faction
		int getFaction() const; //!< Gets faction

		virtual int getBarMAX() const = 0;	//!< Max of status bar
		virtual int getBar() const = 0;		//!< Current fill of status bar

		virtual bool isUpping() const = 0;		//!< Charging/Reloading stat
		virtual bool canFire() const = 0;		//!< Checks if a Weapon can fire
		virtual bool canTrigger() const = 0;	//!< Checks if a Weapon can trigger

		virtual void update(int ms) = 0;	//!< Updates weapon

	protected:
		WeaponI(FireFunc& callback, ProjectileDef const &ammo, std::string ids);

		std::string id_;
		Shape* owner_;

		ProjectileDef output_;
		FireFunc fireCallback_;

		bool pin_;
		b2Vec2 barrel_;

		int level_;

		virtual void fire(b2Vec2 &heading) = 0;
	};
}
#include "Shape.hpp"


#endif