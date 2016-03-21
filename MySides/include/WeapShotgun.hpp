#ifndef MS_WSHOTGUN_HPP
#define MS_WSHOTGUN_HPP

#include "Weapon.hpp"
namespace Weapon {
	class Shotgun : public WeaponI
	{
	public:
		Shotgun(Shape* owner, std::function<void(std::vector<ProjectileDef>& defs, std::string id)>& callback, ProjectileDef const &ammo);
		bool canFire();
		void update(int dt);

		void setRefireTime(int ms);
		void setReloadTime(int ms);

	private:
		void fire(b2Vec2 &heading);

		int refireTime_;
		int refireTimeMax_;
		int reloadTime_;
		int reloadTimeMAX_;

		int pellets_;
	};
}
#endif