#ifndef MS_FORCE_HPP
#define MS_FORCE_HPP

#include "Entity.hpp"

class Force : public Entity{
public:
	Force(b2Body* body, float force, float radius, int lifeTime, int faction = 0);
	void update(int milliseconds);

	float getForce() const;
	float getRadius() const;
	float getLifeTimePercent() const;

	bool collide(Entity* other, b2Contact& contact, std::string tag);
private:
	void createBody(float radius);

	float force_;
	float radius_;
	int lifeTimeMAX_;
	int lifeTime_;
};

#endif