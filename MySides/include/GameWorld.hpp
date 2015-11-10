// GameWorld.hpp
// Inherits from Box2d World, added functionality for game control

#ifndef GAMEWORLD_HPP
#define GAMEWORLD_HPP

#include "stdafx.h"

#include <Box2D\Box2D.h>
#include <SFMLDebugDraw.h>

#include <vector>

#include "Entity.hpp"
#include "Shape.hpp"

class GameWorld : public b2World {
private:
	const b2Vec2 GRAVITY = b2Vec2(0, 0);
	const int VELOCITY_ITERS = 10;
	const int POSITION_ITERS = 10;


	std::vector<Shape> shapes_;
	std::vector<Shape>::iterator controlled_;

public:
	GameWorld();
	bool hasControlled();

	b2Body * addBody(int x, int y);

	Shape * addPlayer(int x, int y);
	//Shape * addEnemy(int x, int y);
	b2Body * addBullet(int x, int y);

	void update(float dt);

	Shape * player();

};

#endif