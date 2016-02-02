#ifndef MS_ENEMY_HPP
#define MS_ENEMY_HPP

#include "Shape.hpp"

class Enemy : public Shape
{
public:
	Enemy(b2Body* body, ShapeDef def) : Shape(body, def)
	{}
};

#endif