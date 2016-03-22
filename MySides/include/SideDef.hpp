#ifndef MS_SIDEDEF_HPP
#define MS_SIDEDEF_HPP

#include <stdafx.h>
#include <Box2D\Common\b2Math.h>

class SideDef {
public:
	SideDef() :
		position(b2Vec2_zero),
		normal(b2Vec2_zero),
		length(0),
		colPrim(b2Color(1.f, 0.f, 0.f)),
		colSecn(b2Color(.75f, .75f, .75f)),
		colTert(b2Color(1.f, 1.f, 1.f))
	{}


	SideDef(b2Vec2 position, b2Vec2 normal, float length) :
		position(position),
		normal(normal),
		length(length),
		colPrim(b2Color(1.f, 0.f, 0.f)),
		colSecn(b2Color(.75f, .75f, .75f)),
		colTert(b2Color(1.f, 1.f, 1.f))
	{}

	SideDef(const SideDef &sd) :
		position(sd.position),
		normal(sd.normal),
		length(sd.length),
		colPrim(sd.colPrim),
		colSecn(sd.colSecn),
		colTert(sd.colTert)
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