#ifndef MS_PLAYER_HPP
#define MS_PLAYER_HPP

#include "Shape.hpp"

class Player : public Shape 
{
public:
	Player(b2Body* body, ShapeDef def) : Shape(body, def)
	{}
};

#endif