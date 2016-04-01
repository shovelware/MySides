#ifndef MS_PROJDEF_H
#define MS_PROJDEF_H

#include <stdafx.h>
#include <Box2D\Common\b2Math.h>
#include <Box2D\Common\b2Draw.h>

class Entity;

class ProjectileDef {
public:
	ProjectileDef() :
		origin(b2Vec2_zero),
		heading(b2Vec2_zero),
		inVelocity(b2Vec2_zero),
		velScale(1),
		oneHit(false),
		bounce(0),
		penetration(0),
		force(std::make_pair(0.f, 0.f)),
		shrapnel(std::make_pair(0, 0)),
		hpMAX(0),
		width(0),
		height(0),
		damage(1),
		lifeTime(0),
		colPrim(b2Color(.75f, .75f, 0.f)),
		colSecn(b2Color(.75f, .75f, 0.f)),
		colTert(b2Color(1.f, 1.f, 1.f)),
		owner(nullptr),
		target(nullptr)
	{}

	ProjectileDef(b2Vec2 origin, b2Vec2 heading, b2Vec2 inVelocity = b2Vec2_zero) :
		origin(origin),
		heading(heading),
		inVelocity(inVelocity),
		velScale(1),
		oneHit(false),
		bounce(0),
		penetration(0),
		force(std::make_pair(0.f, 0.f)),
		shrapnel(std::make_pair(0, 0)),
		hpMAX(1),
		width(1),
		height(0),
		damage(1),
		lifeTime(1000),
		colPrim(b2Color(.75f, .75f, 0.f)),
		colSecn(b2Color(.75f, .75f, 0.f)),
		colTert(b2Color(1.f, 1.f, 1.f)),
		owner(nullptr),
		target(nullptr)
	{}

	ProjectileDef(const ProjectileDef& pd) :
		origin(b2Vec2_zero),
		heading(b2Vec2_zero),
		inVelocity(b2Vec2_zero),
		oneHit(pd.oneHit),
		velScale(pd.velScale),
		bounce(pd.bounce),
		penetration(pd.penetration),
		force(pd.force),
		shrapnel(pd.shrapnel),
		hpMAX(pd.hpMAX),
		width(pd.width),
		height(pd.height),
		damage(pd.damage),
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

	bool oneHit;
	float bounce;
	int penetration;
	std::pair<float, float> force; //! < force, radius 
	std::pair<int, int> shrapnel;	//! < number, level

	unsigned int hpMAX;
	float width;
	float height;

	int damage;
	int lifeTime;

	//Colours
	b2Color colPrim;
	b2Color colSecn;
	b2Color colTert;

	//Not copied
	Entity* owner;
	Entity* target;

	bool isValid() const { return lifeTime >= 0; }
};

#endif