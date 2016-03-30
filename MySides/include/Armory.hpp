#ifndef MS_ARMORY_HPP
#define MS_ARMORY_HPP

#include "ProjectileDef.hpp"

#include "Weapon.hpp"

#include "WeapPistol.hpp"
#include "WeapRifle.hpp"
#include "WeapCoil.hpp"
#include "WeapShotgun.hpp"

//Ideally weapons should be this but effort
//#include "WeapAutoMag.hpp"
//#include "WeapSemiMag.hpp"
//#include "WeapAutoBat.hpp"
//#include "WeapSemiBat.hpp"

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
			ProjectileDef getPellet(int level);
			//ProjectileDef getShrapnel(Projectile& proj);

			ProjectileDef getNinMil(int level);
			ProjectileDef getBullet(int level);
			ProjectileDef getDumDum(int level);
			ProjectileDef getCannonball(int level);

			ProjectileDef getGrenade(int level);
			ProjectileDef getRocket(int level);
			ProjectileDef getLaser(int level);
			ProjectileDef getSlug(int level);

		private:

			std::function<void(std::vector<ProjectileDef>& defs, std::string id)> fireCallback_;
	};
}

#endif