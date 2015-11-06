// GameWorld.hpp
// Inherits from Box2d World, added functionality for game control

#ifndef GAMEWORLD_HPP
#define GAMEWORLD_HPP

#include "stdafx.h"

#include <Box2D\Box2D.h>
#include <SFMLDebugDraw.h>

#include "Entity.hpp"
#include "Shape.hpp"

class GameWorld : public b2World {
private:
	const b2Vec2 GRAVITY = b2Vec2(0, 0);
	const int VELOCITY_ITERS = 10;
	const int POSITION_ITERS = 10;

	b2Body * player_;

	Shape shape_;

public:
	GameWorld();
	bool hasPlayer();

	b2Body * addBody(int x, int y);

	b2Body * addPlayer(int x, int y);
	b2Body * addEnemy(int x, int y);
	b2Body * addBullet(int x, int y);

	void update(float dt);

	Shape * player();

};

#endif