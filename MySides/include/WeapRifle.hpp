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
		bool canFire();
		void update(int dt);

		int refireTimeMAX_;
		int reloadTimeMAX_;
	private:
		void fire(b2Vec2 &heading);

		Magazine magazine_;

		int refireTime_;

		int reloadTime_;
	};
}
#endif