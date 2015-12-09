#ifndef MS_PROJECTILE_HPP
#define MS_PROJECTILE_HPP

#include "Entity.hpp"

class Projectile : public Entity{
public:
	Projectile(b2Body* body, b2Vec2 heading);
	void hit();
	int getDamage() const;
	void update(int milliseconds);
private:
	float speed_;
	float damage_;

	Entity* owner_;
	b2Vec2 origin_;
	b2Vec2 heading_;

	int lifeTime_;
	bool fired_;
	bool impact_;
	
};

#endif