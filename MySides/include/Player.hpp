#ifndef MS_PLAYER_HPP
#define MS_PLAYER_HPP

#include "Shape.hpp"
#include "PlayerDef.hpp"

class Player : public Shape 
{
public:
	Player(b2Body* body, const PlayerDef& def, std::function<void(SideDef&)>& callback);

	~Player();

	bool collide(Entity* other, b2Contact& contact, std::string tag);
	
	bool getBombReady() const;
	float getBombRadius() const;
	void setBombRadius(float range);
	int getBombTime() const;
	int getBombTimeMax() const;
	void setBombTimeMax(int time);

	void bomb();

	void update(int milliseconds);
private:
	float bombRadius_;
	int bombTime_;
	int bombTimeMax_;
};

#endif