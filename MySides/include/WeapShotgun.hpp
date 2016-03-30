#ifndef MS_WSHOTGUN_HPP
#define MS_WSHOTGUN_HPP

#include "Weapon.hpp"
#include "Magazine.hpp"

namespace Weapon {
	class Shotgun : public WeaponI
	{
	public:
		Shotgun(std::function<void(std::vector<ProjectileDef>& defs, std::string id)>& callback, ProjectileDef const &ammo);
		bool canFire() const;

		void reup();
		void update(int dt);

		void setRefireTime(int ms);
		void setReloadTime(int ms);
		void setMagSize(int size, bool reload = false);
		void setPellets(int pellets);
		void setSpread(float spread);

		int getBar() const;
		int getBarMAX() const;
	private:
		void fire(b2Vec2 &heading);

		Magazine magazine_;

		int refireTime_;
		int refireTimeMax_;

		int reloadTime_;
		int reloadTimeMAX_;

		int pellets_;
		float spread_;
	};
}
#endif