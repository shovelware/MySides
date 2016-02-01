#ifndef MS_PROJDEF_H
#define MS_PROJDEF_H

#include <stdafx.h>
#include <Box2D\Box2D.h>

class Entity;

class ProjectileDef {
public:
	ProjectileDef() :
		origin(b2Vec2_zero),
		heading(b2Vec2_zero),
		inVelocity(b2Vec2_zero),
		velScale(1),
		hpMAX(0),
		size(0),
		damageScale(1),
		lifeTime(0),
		colPrim(b2Color(255, 255, 000)),
		colSecn(b2Color(255, 000, 255)),
		colTert(b2Color(255, 255, 255)),
		owner(nullptr),
		target(nullptr)
	{}

	ProjectileDef(b2Vec2 origin, b2Vec2 heading, b2Vec2 inVelocity = b2Vec2_zero) :
		origin(origin),
		heading(heading),
		inVelocity(inVelocity),
		velScale(1),
		hpMAX(1),
		size(1),
		damageScale(1),
		lifeTime(1000),
		colPrim(b2Color(255, 255, 000)),
		colSecn(b2Color(255, 000, 255)),
		colTert(b2Color(255, 255, 255)),
		owner(nullptr),
		target(nullptr)
	{}

	ProjectileDef(const ProjectileDef& pd) :
		origin(b2Vec2_zero),
		heading(b2Vec2_zero),
		inVelocity(b2Vec2_zero),
		velScale(pd.velScale),
		hpMAX(pd.hpMAX),
		size(pd.size),
		damageScale(pd.damageScale),
		lifeTime(pd.lifeTime),
		colPrim(pd.colPrim),
		colSecn(pd.colSecn),
		colTert(pd.colTert),
		owner(nullptr),
		target(nullptr)
	{}

	//Some projDef fields should be manually set every time, 
	//to avoid confusion while copying

	//Not copied
	b2Vec2 origin;
	b2Vec2 heading;
	b2Vec2 inVelocity;

	//Velocity handled by projectile
	float velScale;

	unsigned int hpMAX;
	float size;

	float damageScale;

	int lifeTime;

	//Colours
	b2Color colPrim;
	b2Color colSecn;
	b2Color colTert;

	//Not copied
	Entity* owner;
	Entity* target;

	static enum ProjType {
		BULLET,
		ROCKET
	};

	static ProjectileDef bulletDef()
	{
		ProjectileDef bullet = ProjectileDef();		
		bullet.velScale = 1;
		bullet.hpMAX = 1;
		bullet.size = 1;
		bullet.damageScale = 1;
		bullet.lifeTime = 1000;

		return bullet;
	}

	bool isValid() { return lifeTime >= 0; }
};

#endif