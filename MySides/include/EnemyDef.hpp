#ifndef MS_ENEMYDEF_HPP
#define MS_ENEMYDEF_HPP

#include "ShapeDef.hpp"

class EnemyDef : public ShapeDef {
public:
	EnemyDef() :
		ShapeDef(ShapeDef()),
		weapon(""),
		weaponLevel(0),
		ai(0)
	{}

	EnemyDef(const ShapeDef& sd) :
		ShapeDef(sd),
		weapon(""),
		weaponLevel(0),
		ai(0)
	{}

	EnemyDef(const EnemyDef& ed) :
		ShapeDef(ed),
		weapon(ed.weapon),
		weaponLevel(ed.weaponLevel),
		ai(ed.ai)
	{}

	int ai;
	//AIDef brain AIDef();

	std::string weapon = "";
	int weaponLevel = 0;

	static EnemyDef baseDef()
	{
		EnemyDef base = EnemyDef();
		base.hpScale = 1;
		base.speedScale = 1;
		base.damageScale = 1;

		return base;
	}

	static EnemyDef modDef()
	{
		EnemyDef mod = EnemyDef();
		mod.verticesMin = 0;
		mod.verticesMax = 0;
		mod.vertices = 0;

		mod.size = 0;

		mod.hpScale = 0;
		mod.speedScale = 0;
		mod.damageScale = 0;
		
		return mod;
	}

	static EnemyDef maxDef()
	{
		EnemyDef modmax = EnemyDef();
		modmax.verticesMin = 0;
		modmax.verticesMax = 0;
		modmax.vertices = 0;

		return modmax;
	}

	static EnemyDef triDef()
	{
		//Pink triangle
		EnemyDef tri = EnemyDef();
		tri.verticesMin = 3;
		tri.vertices = 3;
		tri.verticesMax = 3;
		tri.colPrim = b2Color(0.4f, 0.3f, 0.3f);
		tri.colSecn = b2Color(0.7f, 0.5f, 0.5f);
		tri.colTert = b2Color(0.9f, 1, 0);
		tri.faction = 1;
		tri.hpScale = 3;
		tri.upgrade = false;
		tri.size = 0.5f;

		return tri;
	}

	static EnemyDef squDef()
	{
		//Blue square
		EnemyDef squ = EnemyDef();
		squ.verticesMin = 4;
		squ.vertices = 4;
		squ.verticesMax = 4;
		squ.colPrim = b2Color(0, 0, 0.4f);
		squ.colSecn = b2Color(0, 0, 0.7f);
		squ.colTert = b2Color(0, 0.9f, 0);
		squ.faction = 1;
		squ.hpScale = 4;
		squ.upgrade = false;
		squ.size = 0.75f;

		return squ;
	}

	static EnemyDef penDef()
	{
		//Yellow Pentagon
		EnemyDef pen = EnemyDef();
		pen.verticesMin = 5;
		pen.vertices = 5;
		pen.verticesMax = 5;
		pen.colPrim = b2Color(0.3f, 0.3f, 0);
		pen.colSecn = b2Color(0.6f, 0.6f, 0);
		pen.colTert = b2Color(0, 0, 0.9f);
		pen.faction = 1;
		pen.hpScale = 5;
		pen.upgrade = false;
		pen.size = 1.f;

		return pen;
	}

	static EnemyDef hexDef()
	{
		//Red Hexagon
		EnemyDef hex = EnemyDef();
		hex.verticesMin = 6;
		hex.vertices = 6;
		hex.verticesMax = 6;
		hex.colPrim = b2Color(0.4f, 0, 0);
		hex.colSecn = b2Color(0.9f, 0, 0);
		hex.colTert = b2Color(0, 0, 0);
		hex.faction = 1;
		hex.hpScale = 6;
		hex.upgrade = false;
		hex.size = 1.25f;

		return hex;
	}

	static EnemyDef hepDef()
	{
		//Cyan Heptagon
		EnemyDef hep = EnemyDef();
		hep.verticesMin = 7;
		hep.vertices = 7;
		hep.verticesMax = 7;
		hep.colPrim = b2Color(0, 0.4f, 0.4f);
		hep.colSecn = b2Color(0, 0.9f, 0.9f);
		hep.colTert = b2Color(1, 1, 1);
		hep.faction = 1;
		hep.hpScale = 7;
		hep.upgrade = false;
		hep.size = 1.5f;

		return hep;
	}

	static EnemyDef octDef()
	{
		//Black Octagon
		EnemyDef oct = EnemyDef();
		oct.verticesMin = 8;
		oct.vertices = 8;
		oct.verticesMax = 8;
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