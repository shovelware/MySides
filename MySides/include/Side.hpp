#ifndef MS_SIDE_HPP
#define MS_SIDE_HPP

#include "Entity.hpp"
#include "SideDef.hpp"

class Side : public Entity
{
public:
	Side(b2Body* body, const SideDef& def);

	void collect();
	float getValue();

	b2Vec2 getHeading();
	float getTimer() const;
	
	void update(int milliseconds);
	bool collide(Entity* other, b2Contact& contact, std::string tag);

private:
	b2Vec2 heading_;
	float length_;
	int lifeTime_;
	int lifeTimeMAX_;

	void setShape(float size);

	
};

#endif
