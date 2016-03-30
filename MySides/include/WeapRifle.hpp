//Rifle
//Concrete weapon, fires a set number of rounds and reloads

#ifndef MS_WRIFLE_HPP
#define MS_WRIFLE_HPP

#include "Weapon.hpp"
#include "Magazine.hpp"

namespace Weapon {
	class Rifle : public WeaponI {
	public:
		Rifle(std::function<void(std::vector<ProjectileDef>& defs, std::string id)>& callback, ProjectileDef const &ammo);
		
		bool canFire() const;

		void reup();
		void update(int dt);

		void setRefireTime(int ms);
		void setReloadTime(int ms);
		void setMagSize(int size, bool reload = false);

		int getBar() const;
		int getBarMAX() const;
	private:
		void fire(b2Vec2 &heading);

		Magazine magazine_;

		int refireTime_;
		int refireTimeMAX_;

		int reloadTime_;
		int reloadTimeMAX_;
	};
}
#endif