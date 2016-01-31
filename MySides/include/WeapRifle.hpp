//Rifle
//Concrete weapon, fires a set number of rounds and reloads

#ifndef MS_WRIFLE_HPP
#define MS_WRIFLE_HPP

#include "Weapon.hpp"

	class Magazine {
	private:
		int count_;
		int countMAX_;
	public:
		Magazine(int maxRounds) :
			countMAX_(maxRounds),
			count_(maxRounds)
		{

		}

		void remove()
		{
			if (count_ - 1 >= 0)
			{
				count_ -= 1;
			}

			else
			{
				count_ = 0;
			}
		}

		void reload()
		{
			count_ = countMAX_;
		}

		bool checkEmpty()
		{
			return (count_ <= 0);
		}
	};

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