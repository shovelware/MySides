// GameWorld.hpp
// Inherits from Box2d World, added functionality for game control

#ifndef MS_GAMEWORLD_HPP
#define MS_GAMEWORLD_HPP

#include "stdafx.h"

#include <Box2D\Box2D.h>
#include <SFMLDebugDraw.h>

#include <vector>
#include <list> //< Replacing vectors
#include <algorithm>

#include "ContactListener.hpp"

#include "Entity.hpp"
#include "Shape.hpp"
#include "Bounds.hpp"
#include "Projectile.hpp"

//Checks if entity is alive, predicate for remove_if
static bool isAlive(Entity* e)
{
	return e->getAlive();
}

//Checks if entity is active, predicate for remove_if
static bool isActive(Entity e)
{
	return e.getActive();
}

class GameWorld : public b2World {
private:
	const b2Vec2 GRAVITY = b2Vec2(0, 0);
	const int VELOCITY_ITERS = 6;
	const int POSITION_ITERS = 2;

	std::list<Shape> shapes_; //Put players in their own container?
	std::list<Projectile> projectiles_;

	std::list<Shape>::iterator controlled_;
	
	ContactListener contactListener_;
	Bounds bounds_;

	b2Body* addDynamicBody(float x, float y);
	b2Body* addStaticBody (float x, float y);
	b2Body* addBulletBody (float x, float y);
public:
	Shape * player();

	GameWorld();
	bool hasControlled();

	void addPlayer(float x, float y, bool control);
	void addEnemy(float x, float y);
	void addProjectile(float x, float y, float vx, float vy);
	
	void removePlayer();
	void removeEnemy(std::list<Shape>::iterator e);
	void removeProjectile(std::list<Projectile>::iterator p);


	//temporary function until lifetimes implemented
	void clearProj()
	{
		for (std::list<Projectile>::iterator p = projectiles_.begin();
		p != projectiles_.end();)
		{
			DestroyBody(p->getBody());
			projectiles_.erase(p++);
		}
	}
	//void clear(bool clearPlayer);
	//void loadLevel();
	//b2Vec2 randomPos(); //On a circle, in an arc, from centre, whatever, reuse code test stuff
	//

	float getBoundsRadius();
	void resizeBounds(float radius);

	void move(b2Vec2 direction);
	void fire(b2Vec2 direction);

	void controlNext();
	void controlPrev();

	//Return a reference to
	//shapes, bounds, projectiles for drawing

	void update(float dt);
};

#endif