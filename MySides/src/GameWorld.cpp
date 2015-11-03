#include "GameWorld.hpp"

GameWorld::GameWorld() : b2World(GRAVITY)
{
	//SetAllowSleeping(true);
}

void GameWorld::addBody(int x, int y)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);

	b2Body* bodyBox = CreateBody(&bodyDef);

	b2PolygonShape dynBox;
	dynBox.SetAsBox(3, 3);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynBox;

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.1f;

	bodyBox->CreateFixture(&fixtureDef);
}

void GameWorld::update(float dt)
{
	Step(dt, VELOCITY_ITERS, POSITION_ITERS);
}
