// GameWorld.hpp
// Inherits from Box2d World, added functionality for game control

#ifndef MS_GAMEWORLD_HPP
#define MS_GAMEWORLD_HPP

#include "stdafx.h"

#include <Box2D\Box2D.h>
#include <SFMLDebugDraw.h>

#include <vector>

#include "Entity.hpp"
#include "Shape.hpp"
#include "Bounds.hpp"


class GameWorld : public b2World {
private:
	const b2Vec2 GRAVITY = b2Vec2(0, 0);
	const int VELOCITY_ITERS = 6;
	const int POSITION_ITERS = 6;

	std::vector<Shape> shapes_;
	std::vector<Shape>::iterator controlled_;
	
	Bounds bounds_;

	//CHANGE THESE COORDINATES TO SCREENSPACE: SFML IN, B2D OUT
	//MAYBE DO CONVERSION OUTSIDE WHO KNOWS WHY NOT DECIDE WHETHER OR NOT THEY'RE SCREENSPACE THERE?
	b2Body* addDynamicBody(float x, float y);
	b2Body* addStaticBody(float x, float y);
	b2Body* addBulletBody(float x, float y);
public:
	Shape * player();

	GameWorld();
	bool hasControlled();

	Shape* addPlayer(float x, float y);
	Shape * addEnemy(float x, float y);
	b2Body* addProjectile(float x, float y);
	
	void resizeBounds(float radius);

	void controlNext();
	void controlPrev();

	void update(float dt);
};

#endif