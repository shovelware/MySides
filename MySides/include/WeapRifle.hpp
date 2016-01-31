//Rifle
//Concrete weapon, fires a set number of rounds and reloads

#ifndef MS_WRIFLE_HPP
#define MS_WRIFLE_HPP

#include "Weapon.hpp"
#include "Magazine.hpp"

namespace Weapon {
	class Rifle : public WeaponI {
	public:
		Rifle(Shape* owner, std::function<void(ProjectileDef&)>& callback, ProjectileDef const &ammo);
		void update(int dt);
	private:
		void fire(b2Vec2 &heading);
		bool canFire();

		Magazine magazine_;

		int refireTime_;
		int refireTimeMAX_;

		int reloadTime_;
		int reloadTimeMAX_;
	};
}
#endif