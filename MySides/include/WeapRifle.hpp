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
		count_(countMAX_)
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

class Rifle : public Weapon {
public:
	Rifle(Shape* owner, std::function<void(ProjectileDef&)>& callback);
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
#endif