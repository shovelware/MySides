#ifndef MS_SHAPEDEF_H
#define MS_SHAPEDEF_H

#include <stdafx.h>
#include <Box2D\Box2D.h>

class ShapeDef {
public:
	ShapeDef() :
		position(b2Vec2_zero),
		heading(b2Vec2_zero),
		maxHP(1),
		size(1),
		vertices(0)
	{}

	ShapeDef(b2Vec2 position, b2Vec2 heading, int vertices) :
		position(position),
		heading(heading),
		maxHP(1),
		size(1),
		vertices(vertices)
	{}

	ShapeDef(const ShapeDef& sd) :
		position(sd.position),
		heading(sd.heading),
		maxHP(sd.maxHP),
		size(sd.size),
		vertices(sd.vertices),
		colPrim(sd.colPrim),
		colSecn(sd.colSecn),
		colTert(sd.colTert)
	{}

	b2Vec2 position;
	b2Vec2 heading;

	unsigned int maxHP;
	float size;

	int vertices;

	//Colours
	b2Color colPrim;
	b2Color colSecn;
	b2Color colTert;

	//Entity* parent;//???	

	bool isValid() { return (2 < vertices && vertices < 9); }
};
#endif