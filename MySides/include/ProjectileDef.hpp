#ifndef MS_PROJDEF_H
#define MS_PROJDEF_H

#include <stdafx.h>
#include <Box2D\Common\b2Math.h>

class Entity;

class ProjectileDef {
public:
	ProjectileDef() :
		origin(b2Vec2_zero),
		heading(b2Vec2_zero),
		inVelocity(b2Vec2_zero),
		velScale(1),
		rect(false),
		ghost(false),
		bounce(0),
		hpMAX(0),
		size(0),
		damageScale(1),
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
		rect(false),
		ghost(false),
		bounce(0),
		hpMAX(1),
		size(1),
		damageScale(1),
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
		velScale(pd.velScale),
		bounce(pd.bounce),
		rect(pd.rect),
		ghost(pd.ghost),
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

	bool rect;
	bool ghost;
	float bounce;

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

	static ProjectileDef pelletDef()
	{
		ProjectileDef pellet = ProjectileDef();
		pellet.velScale = 1;
		pellet.hpMAX = 1;
		pellet.size = 0.25f;
		pellet.damageScale = 1;
		pellet.lifeTime = 150;

		return pellet;
	}

	static ProjectileDef ninmilDef()
	{
		ProjectileDef ninmil = ProjectileDef();
		ninmil.velScale = 1;
		ninmil.hpMAX = 1;
		ninmil.size = 0.5f;
		ninmil.damageScale = 1;
		ninmil.lifeTime = 500;
		
		return ninmil;
	}

	static ProjectileDef bulletDef()
	{
		ProjectileDef bullet = ProjectileDef();		
		bullet.velScale = 1;
		bullet.hpMAX = 1;
		bullet.size = 1;
		bullet.damageScale = 1.25f;
		bullet.lifeTime = 1000;

		return bullet;
	}

	static ProjectileDef dumdumDef()
	{
		ProjectileDef dumdum = ProjectileDef();
		dumdum.velScale = 2;
		dumdum.hpMAX = 2;
		dumdum.size = 2.f;
		dumdum.damageScale = 1.5f;
		dumdum.lifeTime = 1250;

		return dumdum;
	}

	static ProjectileDef cnnbllDef()
	{
		ProjectileDef cnnbll = ProjectileDef();
		cnnbll.velScale = 3;
		cnnbll.hpMAX = 4;
		cnnbll.size = 3.f;
		cnnbll.damageScale = 2.f;
		cnnbll.lifeTime = 1500;

		return cnnbll;
	}

	static ProjectileDef grenadeDef()
	{
		ProjectileDef grenade = ProjectileDef();
		grenade.velScale = 1;
		grenade.hpMAX = 1;
		grenade.size = 1.f;
		grenade.rect = true;
		grenade.damageScale = 2.f;
		grenade.lifeTime = 1250;

		return grenade;
	}

	static ProjectileDef rocketDef()
	{
		ProjectileDef rocket = ProjectileDef();
		rocket.velScale = 3;
		rocket.hpMAX = 1;
		rocket.size = 1.f;
		rocket.rect = true;
		rocket.damageScale = 1.f;
		rocket.lifeTime = 500;

		return rocket;
	}

	static ProjectileDef pewpewDef()
	{
		ProjectileDef pewpew = ProjectileDef();
		pewpew.velScale = 2;
		pewpew.hpMAX = 2;
		pewpew.size = .25f;
		pewpew.rect = true;
		pewpew.ghost = true;
		pewpew.damageScale = 1.f;
		pewpew.lifeTime = 500;

		return pewpew;
	}

	bool isValid() { return lifeTime >= 0; }
};

#endif