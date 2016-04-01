#ifndef MS_ARMORY_HPP
#define MS_ARMORY_HPP

#include "ProjectileDef.hpp"

#include "Weapon.hpp"

#include "WeapSemiMag.hpp"
#include "WeapAutoMag.hpp"
#include "WeapSpreadMag.hpp"
#include "WeapAutoBat.hpp"
#include "WeapSemiBat.hpp"
//#include "WeapSpreadBat.hpp"

namespace Weapon {
	class Armory {
		public:
			Armory(std::function<void(std::vector<ProjectileDef>& defs, std::string id)> fireCallback);

			//Weapons
			WeaponI* getPistol(int level, int projLevel = 0);
			WeaponI* getRifle(int level, int projLevel = 0);
			WeaponI* getMagnum(int level, int projLevel = 0);
			WeaponI* getCannon(int level, int projLevel = 0);
			WeaponI* getShotgun(int level, int projLevel = 0);

			WeaponI* getShotLaser(int level, int projLevel = 0);
			WeaponI* getCharger(int level, int projLevel = 0);
			WeaponI* getCoilgun(int level, int projLevel = 0);
			WeaponI* getRailgun(int level, int projLevel = 0);

			//Projectiles
			//ProjectileDef getShrapnel(Projectile& proj);
			ProjectileDef getShrapnel(int level = 0);

			ProjectileDef getPellet(int level = 0);
			ProjectileDef getNinMil(int level = 0);
			ProjectileDef getBullet(int level = 0);
			ProjectileDef getDumDum(int level= 0);
			ProjectileDef getCannonball(int level = 0);

			ProjectileDef getGrenade(int level = 0);
			ProjectileDef getRocket(int level = 0);
			ProjectileDef getLaser(int level = 0);
			ProjectileDef getSlug(int level = 0);

		private:
			std::function<void(std::vector<ProjectileDef>& defs, std::string id)> fireCallback_;
	};
}

#endif