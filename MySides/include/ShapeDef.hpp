#ifndef MS_SHAPEDEF_HPP
#define MS_SHAPEDEF_HPP

#include <stdafx.h>
#include <Box2D\Box2D.h>

class ShapeDef {
public:
	ShapeDef() :
		position(b2Vec2_zero),
		heading(b2Vec2_zero),
		size(1),
		upgrade(true),
		hpScale(1),
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
		size(1),
		upgrade(true),
		hpScale(10),
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
		size(sd.size),
		upgrade(sd.upgrade),
		hpScale(sd.hpScale),
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

	float size;
	bool upgrade;

	int hpScale;
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