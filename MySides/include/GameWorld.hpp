#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <Box2D\Box2D.h>
#include <SFMLDebugDraw.h>

class GameWorld : public b2World {
private:
	const b2Vec2 GRAVITY = b2Vec2(0, 0);
	const int VELOCITY_ITERS = 10;
	const int POSITION_ITERS = 10;
public:
	GameWorld();
	void addBody(int x, int y);
	void update(float dt);
};

#endif