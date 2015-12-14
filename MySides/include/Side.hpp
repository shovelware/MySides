#ifndef MS_SIDE_HPP
#define MS_SIDE_HPP

#include "Entity.hpp"

class Side : public Entity
{
public:
	Side(b2Body* body, b2Vec2 normal, float size);

	void collect();
	float getValue();

	//Do we need an update function?
	bool collide(Entity* other, b2Contact& contact);

private:
	float size_;
};

#endif
