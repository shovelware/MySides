#ifndef MS_PROJECTILE_HPP
#define MS_PROJECTILE_HPP

#include "Entity.hpp"

class Projectile : public Entity{
private:
	float speed_;
	float damage_;

public:
	Projectile(b2Body* body, b2Vec2 heading);
	void update(int milliseconds);
	void move(b2Vec2);
	void rotate(float);
};

#endif