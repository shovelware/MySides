#ifndef MS_ARMORY_HPP
#define MS_ARMORY_HPP

#include "ProjectileDef.hpp"

#include "Weapon.hpp"

#include "WeapRifle.hpp"
#include "WeapCoil.hpp"
#include "WeapShotgun.hpp"

namespace Weapon {
	class Armory {
		public:
			WeaponI* getPistol(int level);
			WeaponI* getRifle(int level);
			WeaponI* getMagnum(int level);
			WeaponI* getCannon(int level);
			WeaponI* getShotgun(int level);

			WeaponI* getShotLaser(int level);
			WeaponI* getCharger(int level);
			WeaponI* getCoilgun(int level);
			WeaponI* getRailgun(int level);
		private:

	};
}

#endif