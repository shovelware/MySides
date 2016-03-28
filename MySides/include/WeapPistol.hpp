#ifndef MS_WPISTOL_HPP
#define MS_WPISTOL_HPP

#include "Weapon.hpp"
#include "Magazine.hpp"

namespace Weapon {
	class Pistol : public WeaponI {
	public:
		Pistol(std::function<void(std::vector<ProjectileDef>& defs, std::string id)>& callback, ProjectileDef const &ammo);
		
		bool canFire();

		void update(int dt);

		void resizeMag(int size);
	private:
		void fire(b2Vec2 &heading);

		Magazine magazine_;

		int idleTime_;
		int idleTimeMAX_;

		int reloadTime_;
		int reloadTimeMAX_;

		bool cocked_;
	};
}

#endif