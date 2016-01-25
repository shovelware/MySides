#ifndef MS_SIDE_HPP
#define MS_SIDE_HPP

#include "Entity.hpp"

struct SideDef {
	SideDef(b2Vec2 position, b2Vec2 direction, float size) :
		position(position),
		direction(direction),
		size(size)
	{}

	b2Vec2 position;
	b2Vec2 direction;
	float size;
};

class Side : public Entity
{
public:
	Side(b2Body* body, b2Vec2 normal, float size);
	//Side(b2Body* body, SideDef def);

	void collect();
	float getValue();

	//Do we need an update function?
	bool collide(Entity* other, b2Contact& contact);

private:

	void setShape(float size);
	float size_;
};

#endif
