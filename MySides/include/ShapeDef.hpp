#ifndef MS_SHAPEDEF_HPP
#define MS_SHAPEDEF_HPP

#include <stdafx.h>
#include <Box2D\Common\b2Math.h>

class ShapeDef {
public:
	ShapeDef() :
		position(b2Vec2_zero),
		heading(b2Vec2_zero),
		hpMAX(1),
		size(1),
		speedScale(1),
		vertices(0),
		colPrim(b2Color(.5f, .5f, .5f)),
		colSecn(b2Color(.75f, .75f, .75f)),
		colTert(b2Color(0.f, 0.f, 0.f))
	{}

	ShapeDef(b2Vec2 position, b2Vec2 heading, int vertices) :
		position(position),
		heading(heading),
		hpMAX(vertices > 0 ? vertices : 1),
		size(1),
		speedScale(1),
		vertices(vertices),
		colPrim(b2Color(.5f, .5f, .5f)),
		colSecn(b2Color(.75f, .75f, .75f)),
		colTert(b2Color(0.f, 0.f, 0.f))
	{}

	ShapeDef(const ShapeDef& sd) :
		position(sd.position),
		heading(sd.heading),
		hpMAX(sd.hpMAX),
		size(sd.size),
		speedScale(sd.speedScale),
		vertices(sd.vertices),
		colPrim(sd.colPrim),
		colSecn(sd.colSecn),
		colTert(sd.colTert)
	{}

	b2Vec2 position;
	b2Vec2 heading;

	unsigned int hpMAX;
	float size;

	float speedScale;

	int vertices;

	//Colours
	b2Color colPrim;
	b2Color colSecn;
	b2Color colTert;

	//Entity* parent;//???	

	bool isValid() { return (2 < vertices && vertices < 9); }
};
#endif