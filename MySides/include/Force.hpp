#ifndef MS_FORCE_HPP
#define MS_FORCE_HPP

#include "ForceDef.hpp"
#include "Entity.hpp"

class Force : public Entity{
public:
	Force(b2Body* body, float force, float radius, int lifeTime, int faction = 0);
	Force(b2Body* body, const ForceDef& def);
	void update(int milliseconds);

	float getForce() const;
	float getRadius() const;
	float getLifeTimePercent() const;

	bool collide(Entity* other, b2Contact& contact, std::string tag);
private:
	void createBody(float radius);

	bool sides_;
	bool shapes_;
	bool pickups_;
	bool projectiles_;

	float force_;
	float radius_;

	int lifeTimeMAX_;
	int lifeTime_;
};

#endif