#ifndef MS_PROJDEF_H
#define MS_PROJDEF_H

#include <stdafx.h>
#include <Box2D\Common\b2Math.h>
#include <Box2D\Common\b2Draw.h>

class Entity;

class ProjectileDef {
public:
	struct ProjDet {
		float force = 0;
		float radius = 0;
		int lifeTime = 0;
	};

	struct ProjShrapnel {
		int shards = 0;
		int level = 0;
	};

	struct ProjTracking {
		float radius = 0;
		float speed = 0;
	};

	ProjectileDef() :
		origin(b2Vec2_zero),
		heading(b2Vec2_zero),
		inVelocity(b2Vec2_zero),
		velScale(1),
		oneHit(false),
		bounce(0),
		penetration(0),
		detonation(ProjDet()),
		shrapnel(ProjShrapnel()),
		tracking(ProjTracking()),
		hpMAX(0),
		width(0),
		height(0),
		damage(1),
		lifeTime(0),
		faction(0),
		colPrim(b2Color(.75f, .75f, 0.f)),
		colSecn(b2Color(.75f, .75f, 0.f)),
		colTert(b2Color(1.f, 1.f, 1.f)),
		owner(nullptr)
	{}

	ProjectileDef(b2Vec2 origin, b2Vec2 heading, b2Vec2 inVelocity = b2Vec2_zero) :
		origin(origin),
		heading(heading),
		inVelocity(inVelocity),
		velScale(1),
		oneHit(false),
		bounce(0),
		penetration(0), 
		detonation(ProjDet()),
		shrapnel(ProjShrapnel()),
		tracking(ProjTracking()),
		hpMAX(1),
		width(1),
		height(0),
		damage(1),
		lifeTime(1000),
		faction(0),
		colPrim(b2Color(.75f, .75f, 0.f)),
		colSecn(b2Color(.75f, .75f, 0.f)),
		colTert(b2Color(1.f, 1.f, 1.f)),
		owner(nullptr)
	{}

	ProjectileDef(const ProjectileDef& pd) :
		origin(b2Vec2_zero),
		heading(b2Vec2_zero),
		inVelocity(b2Vec2_zero),
		oneHit(pd.oneHit),
		velScale(pd.velScale),
		bounce(pd.bounce),
		penetration(pd.penetration),
		detonation(pd.detonation),
		shrapnel(pd.shrapnel),
		tracking(pd.tracking),
		hpMAX(pd.hpMAX),
		width(pd.width),
		height(pd.height),
		damage(pd.damage),
		lifeTime(pd.lifeTime),
		faction(pd.faction),
		colPrim(pd.colPrim),
		colSecn(pd.colSecn),
		colTert(pd.colTert),
		owner(nullptr)
	{
	}

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
	ProjDet detonation; //! < force, radius, lifetime
	ProjShrapnel shrapnel;	//! < number, level
	ProjTracking tracking;

	unsigned int hpMAX;
	float width;
	float height;

	int damage;
	int lifeTime;
	int faction;

	//Colours
	b2Color colPrim;
	b2Color colSecn;
	b2Color colTert;

	//Not copied
	Entity* owner;

	bool isValid() const { return lifeTime >= 0; }

	float getRange() const {
		float dist = 0;
		float speed = 0;

		if (height != 0)
		{
			speed = 0.05f * (width * height);
			speed *= velScale;

			dist = lifeTime * speed;
		}

		else {
			speed = 0.05f * (b2_pi * (width * width));
			speed *= velScale;

			dist = lifeTime * speed;
		}

		return dist;
	}
};

#endif