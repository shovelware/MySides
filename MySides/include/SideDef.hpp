#ifndef MS_SIDEDEF_HPP
#define MS_SIDEDEF_HPP

#include <stdafx.h>
#include <Box2D\Box2D.h>

class SideDef {
public:
	SideDef() :
		position(b2Vec2_zero),
		normal(b2Vec2_zero),
		length(0)
	{}


	SideDef(b2Vec2 position, b2Vec2 normal, float length) :
		position(position),
		normal(normal),
		length(length)
	{}

	SideDef(const SideDef &sd) :
		position(sd.position),
		normal(sd.normal),
		length(sd.length)
	{}

	b2Vec2 position;
	b2Vec2 normal;

	float length;

	//Colours
	b2Color colPrim;
	b2Color colSecn;
	b2Color colTert;

	bool isValid() { return (length > 0); }
};

#endif