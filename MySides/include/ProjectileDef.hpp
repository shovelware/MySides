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
		ghost(false),
		bounce(0),
		shrapnel(0),
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
		ghost(false),
		bounce(0),
		shrapnel(0),
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
		ghost(pd.ghost),
		bounce(pd.bounce),
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
	bool ghost;
	float bounce;
	int shrapnel;

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

	static ProjectileDef pelletDef()
	{
		ProjectileDef pellet = ProjectileDef();
		pellet.velScale = 1;
		pellet.hpMAX = 1;
		pellet.width = 0.25f;
		pellet.damage = 1;
		pellet.lifeTime = 150;

		return pellet;
	}

	static ProjectileDef ninmilDef()
	{
		ProjectileDef ninmil = ProjectileDef();
		ninmil.velScale = 1;
		ninmil.hpMAX = 1;
		ninmil.width = 0.5f;
		ninmil.damage = 1;
		ninmil.lifeTime = 500;
		
		return ninmil;
	}

	static ProjectileDef bulletDef()
	{
		ProjectileDef bullet = ProjectileDef();		
		bullet.velScale = 1;
		bullet.hpMAX = 1;
		bullet.width = 1;
		bullet.damage = 1.25f;
		bullet.lifeTime = 1000;

		return bullet;
	}

	static ProjectileDef dumdumDef()
	{
		ProjectileDef dumdum = ProjectileDef();
		dumdum.velScale = 2;
		dumdum.hpMAX = 2;
		dumdum.width = 2.f;
		dumdum.damage = 1.5f;
		dumdum.lifeTime = 1250;

		return dumdum;
	}

	static ProjectileDef cnnbllDef()
	{
		ProjectileDef cnnbll = ProjectileDef();
		cnnbll.velScale = 3;
		cnnbll.hpMAX = 4;
		cnnbll.width = 4.f;
		cnnbll.damage = 2.f;
		cnnbll.lifeTime = 1500;

		return cnnbll;
	}

	static ProjectileDef grenadeDef()
	{
		ProjectileDef grenade = ProjectileDef();
		grenade.velScale = 1.f;
		grenade.oneHit = true;
		grenade.hpMAX = 1;
		grenade.width = 1.75f;
		grenade.height = 1.75f;
		grenade.shrapnel = 12;
		grenade.damage = 1.f;
		grenade.lifeTime = 1000;

		return grenade;
	}

	static ProjectileDef rocketDef()
	{
		ProjectileDef rocket = ProjectileDef();
		rocket.velScale = 2.f;
		rocket.oneHit = true;
		rocket.hpMAX = 1;
		rocket.width = 2.f;
		rocket.height = 3.f;
		rocket.shrapnel = 8;
		rocket.damage = 1.f;
		rocket.lifeTime = 500;

		return rocket;
	}

	static ProjectileDef pewpewDef()
	{
		ProjectileDef pewpew = ProjectileDef();
		pewpew.velScale = 1.5f;
		pewpew.hpMAX = 2;
		pewpew.width = .5f;
		pewpew.height = 6.f;
		pewpew.ghost = true;
		pewpew.damage = 1.f;
		pewpew.lifeTime = 500;

		return pewpew;
	}

	bool isValid() const { return lifeTime >= 0; }
};

#endif