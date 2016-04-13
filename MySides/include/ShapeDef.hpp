#ifndef MS_SHAPEDEF_HPP
#define MS_SHAPEDEF_HPP

#include <stdafx.h>
#include <Box2D\Box2D.h>

class ShapeDef {
public:
	ShapeDef() :
		position(b2Vec2_zero),
		heading(b2Vec2_zero),
		hpScale(1),
		upgrade(true),
		size(1),
		speedScale(1),
		damageScale(1),
		vertices(0),
		verticesMin(3),
		verticesMax(8),
		faction(0),
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
		damageScale(1),
		vertices(vertices),
		verticesMin(3),
		verticesMax(8),
		faction(0),
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
		damageScale(sd.damageScale),
		vertices(sd.vertices),
		verticesMin(sd.verticesMin),
		verticesMax(sd.verticesMax),
		faction(sd.faction),
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
	float damageScale;

	unsigned int vertices;
	unsigned int verticesMin;
	unsigned int verticesMax;

	int faction;

	//Colours
	b2Color colPrim;
	b2Color colSecn;
	b2Color colTert;

	bool isValid() const { 
		return (verticesMin <= vertices && vertices <= verticesMax &&
				2 < verticesMin && verticesMax < 9); }
};
#endif