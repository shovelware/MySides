#ifndef MS_WSPREADMAG_HPP
#define MS_WSPREADMAG_HPP

#include "Weapon.hpp"
#include "Magazine.hpp"

namespace Weapon {
	class SpreadMag : public WeaponI {
	public:
		SpreadMag(fireFunc& callback, ProjectileDef const &ammo, std::string id);
		SpreadMag(fireFunc& callback, ProjectileDef const &ammo, std::string id, int magSize, int resetTime, int reloadTime, int pellets, float spread);
		
		bool isUpping() const;
		bool canFire() const;
		bool canTrigger() const;

		void reup(bool instant = false);
		void update(int ms);

		void setMagSize(int size, bool reload = false);

		void setResetTime(int ms);
		void setReloadTime(int ms);
		void setPellets(int pellets);
		void setSpread(float spread);

		int getBar() const;
		int getBarMAX() const;
	private:
		void fire(b2Vec2 &heading);

		Magazine magazine_;

		int resetTime_;
		int resetTimeMAX_;

		int reloadTime_;
		int reloadTimeMAX_;

		bool pumped_;

		int pellets_;
		float spread_;
	};
}
#endif