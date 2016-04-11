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
		vertices(0),
		verticesMin(3),
		verticesMax(8),
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
		verticesMin(3),
		verticesMax(8),
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
		verticesMin(sd.verticesMin),
		verticesMax(sd.verticesMax),
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

	unsigned int vertices;
	unsigned int verticesMin;
	unsigned int verticesMax;

	int faction;
	int ai;

	//Colours
	b2Color colPrim;
	b2Color colSecn;
	b2Color colTert;

	bool isValid() const { 
		return (verticesMin <= vertices && vertices <= verticesMax &&
				2 < verticesMin && verticesMax < 9); }

	static ShapeDef triDef()
	{
		//Pink triangle
		ShapeDef tri = ShapeDef(b2Vec2_zero, b2Vec2(0, -1), 3);
		tri.verticesMin = 3;
		tri.vertices = 3;
		tri.verticesMax = 3;
		tri.ai = 2;
		tri.colPrim = b2Color(0.7f, 0.5f, 0.5f);
		tri.colSecn = b2Color(0.4f, 0.3f, 0.3f);
		tri.colTert = b2Color(0.9f, 1, 0);
		tri.faction = 1;
		tri.hpScale = 3;
		tri.upgrade = false;
		tri.size = 0.5f;

		return tri;
	}

	static ShapeDef squDef() 
	{
		//Blue square
		ShapeDef squ = ShapeDef(b2Vec2_zero, b2Vec2(0, -1), 3);
		squ.verticesMin = 4;
		squ.vertices = 4;
		squ.verticesMax = 4;
		squ.ai = 2;
		squ.colPrim = b2Color(0, 0, 0.7f);
		squ.colSecn = b2Color(0, 0, 0.4f);
		squ.colTert = b2Color(0, 0.9f, 0);
		squ.faction = 1;
		squ.hpScale = 4;
		squ.upgrade = false;
		squ.size = 0.75f;

		return squ;
	}

	static ShapeDef penDef() 
	{
		//Yellow Pentagon
		ShapeDef pen = ShapeDef(b2Vec2_zero, b2Vec2(0, -1), 3);
		pen.verticesMin = 5;
		pen.vertices = 5;
		pen.verticesMax = 5;
		pen.ai = 2;
		pen.colPrim = b2Color(0.6f, 0.6f, 0);
		pen.colSecn = b2Color(0.3f, 0.3f, 0);
		pen.colTert = b2Color(0, 0, 0.9f);
		pen.faction = 1;
		pen.hpScale = 5;
		pen.upgrade = false;
		pen.size = 1.f;

		return pen;
	}

	static ShapeDef hexDef() 
	{
		//Red Hexagon
		ShapeDef hex = ShapeDef(b2Vec2_zero, b2Vec2(0, -1), 3);
		hex.verticesMin = 6;
		hex.vertices = 6;
		hex.verticesMax = 6;
		hex.ai = 2;
		hex.colPrim = b2Color(0.9f, 0, 0);
		hex.colSecn = b2Color(0.4f, 0, 0);
		hex.colTert = b2Color(0, 0, 0);
		hex.faction = 1;
		hex.hpScale = 6;
		hex.upgrade = false;
		hex.size = 1.25f;

		return hex;
	}

	static ShapeDef hepDef() 
	{
		//Cyan Heptagon
		ShapeDef hep = ShapeDef(b2Vec2_zero, b2Vec2(0, -1), 3);
		hep.verticesMin = 7;
		hep.vertices = 7;
		hep.verticesMax = 7;
		hep.ai = 2;
		hep.colPrim = b2Color(0, 0.9f, 0.9f);
		hep.colSecn = b2Color(0, 0.9f, 0.9f);
		hep.colTert = b2Color(1, 1, 1);
		hep.faction = 1;
		hep.hpScale = 7;
		hep.upgrade = false;
		hep.size = 1.5f;

		return hep;
	}

	static ShapeDef octDef() 
	{
		//Black Heptagon
		ShapeDef oct = ShapeDef(b2Vec2_zero, b2Vec2(0, -1), 3);
		oct.verticesMin = 8;
		oct.vertices = 8;
		oct.verticesMax = 8;
		oct.ai = 2;
		oct.colPrim = b2Color(0, 0, 0);
		oct.colSecn = b2Color(0.9f, 0.9f, 0.9f);
		oct.colTert = b2Color(0, 1, 0);
		oct.faction = 1;
		oct.hpScale = 8;
		oct.upgrade = false;
		oct.size = 1.75f;

		return oct;
	}
};
#endif