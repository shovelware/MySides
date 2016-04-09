#ifndef MS_SHAPEDEF_HPP
#define MS_SHAPEDEF_HPP

#include <stdafx.h>
#include <Box2D\Common\b2Math.h>

class ShapeDef {
public:
	ShapeDef() :
		position(b2Vec2_zero),
		heading(b2Vec2_zero),
		hpScale(1),
		upgrade(true),
		size(1),
		speedScale(1),
		vertices(0),
		faction(0),
		ai(0),
		colPrim(b2Color(.5f, .5f, .5f)),
		colSecn(b2Color(.75f, .75f, .75f)),
		colTert(b2Color(0.f, 0.f, 0.f))
	{}

	ShapeDef(b2Vec2 position, b2Vec2 heading, int vertices) :
		position(position),
		heading(heading),
		hpScale(10),
		upgrade(true),
		size(1),
		speedScale(1),
		vertices(vertices),
		faction(0),
		ai(0),
		colPrim(b2Color(.5f, .5f, .5f)),
		colSecn(b2Color(.75f, .75f, .75f)),
		colTert(b2Color(0.f, 0.f, 0.f))
	{}

	ShapeDef(const ShapeDef& sd) :
		position(sd.position),
		heading(sd.heading),
		hpScale(sd.hpScale),
		upgrade(sd.upgrade),
		size(sd.size),
		speedScale(sd.speedScale),
		vertices(sd.vertices),
		faction(sd.faction),
		ai(sd.ai),
		colPrim(sd.colPrim),
		colSecn(sd.colSecn),
		colTert(sd.colTert)
	{}

	b2Vec2 position;
	b2Vec2 heading;

	int hpScale;
	bool upgrade;

	float size;

	float speedScale;

	int vertices;

	int faction;
	int ai;

	//Colours
	b2Color colPrim;
	b2Color colSecn;
	b2Color colTert;

	bool isValid() const { return (2 < vertices && vertices < 9); }
};
#endif