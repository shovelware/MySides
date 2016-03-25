#ifndef MS_PLAYER_HPP
#define MS_PLAYER_HPP

#include "Shape.hpp"

class Player : public Shape 
{
public:
	Player(b2Body* body, ShapeDef def, std::function<void(SideDef&)>& callback);

	bool collide(Entity* other, b2Contact& contact);
	
	float getBombRange() const;
	bool getBombReady() const;

	void bomb();
	void update(int milliseconds);

private:
	float bombRange_;
	int bombTime_;
	int bombTimeMax_;
};

#endif