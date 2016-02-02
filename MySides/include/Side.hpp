#ifndef MS_SIDE_HPP
#define MS_SIDE_HPP

#include "Entity.hpp"
#include "SideDef.hpp"

class Side : public Entity
{
public:
	Side(b2Body* body, b2Vec2 normal, float length);
	Side(b2Body* body, SideDef def);

	void collect();
	float getValue();

	//Do we need an update function?
	bool collide(Entity* other, b2Contact& contact);

private:

	void setShape(float size);
	float length_;
};

#endif
