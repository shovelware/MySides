#ifndef MS_ARMORY_HPP
#define MS_ARMORY_HPP

#include "ProjectileDef.hpp"

#include "WeapSemiMag.hpp"
#include "WeapAutoMag.hpp"
#include "WeapSpreadMag.hpp"
#include "WeapAutoBat.hpp"
#include "WeapSemiBat.hpp"
#include "WeapSpreadBat.hpp"

namespace Weapon {
	class Armory {
		public:
			Armory(FireFunc& fireCallback);

			//For use with text-based levels
			WeaponI* requisition(std::string name, int code = 0);

			//New weapon happens here
			WeaponI* getPistol(int level = 0, int projLevel = -1);
			WeaponI* getRifle(int level = 0, int projLevel = -1);
			WeaponI* getCannon(int level = 0, int projLevel = -1);
			WeaponI* getShotgun(int level = 0, int projLevel = -1);

			WeaponI* getWerfer(int level = 0, int projLevel = -1);
			WeaponI* getCoilgun(int level = 0, int projLevel = -1);
			WeaponI* getRailgun(int level = 0, int projLevel = -1);

			WeaponI* getThumper(int level = 0, int projLevel = -1);
			WeaponI* getLauncher(int level = 0, int projLevel = -1);

			WeaponI* getFun(int type);

			//Sets a weapon's level
			void setWeaponLevel(WeaponI* weapon, int level = 0, int projLevel = -1);

			//Increment a weapon's level
			void upgradeWeapon(WeaponI* weapon);


			//Projectiles
			ProjectileDef getShrapnel(int level = 0);

			ProjectileDef getPellet(int level = 0);
			ProjectileDef getNinMil(int level = 0);
			ProjectileDef getBullet(int level = 0);
			ProjectileDef getCannonball(int level = 0);

			ProjectileDef getGrenade(int level = 0);
			ProjectileDef getRocket(int level = 0);

			ProjectileDef getLaser(int level = 0);
			ProjectileDef getSlug(int level = 0);
			ProjectileDef getFlammen(int level = 0);

		private:
			std::function<void(std::vector<ProjectileDef>& defs, std::string id)> fireCallback_;

			//Upgrade trees for each weapon
			void upgradePistol(Weapon::SemiMag* weapon, int level = 0, int projLevel = -1);
			void upgradeRifle(Weapon::AutoMag* weapon, int level = 0, int projLevel = -1);
			void upgradeCannon(Weapon::SemiMag* weapon, int level = 0, int projLevel = -1);
			void upgradeShotgun(Weapon::SpreadMag* weapon, int level = 0, int projLevel = -1);
			void upgradeWerfer(Weapon::SpreadBat* weapon, int level = 0, int projLevel = -1);
			void upgradeCoilgun(Weapon::AutoBat* weapon, int level = 0, int projLevel = -1);
			void upgradeRailgun(Weapon::SemiBat* weapon, int level = 0, int projLevel = -1);
			void upgradeThumper(Weapon::SpreadMag* weapon, int level = 0, int projLevel = -1);
			void upgradeLauncher(Weapon::SemiMag* weapon, int level = 0, int projLevel = -1);

	};
}

#endif