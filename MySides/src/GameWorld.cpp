#include "GameWorld.hpp"

GameWorld::GameWorld() : b2World(GRAVITY)
{
}

bool GameWorld::hasPlayer()
{
	return !(player_ == nullptr);
}

b2Body * GameWorld::addBody(int x, int y)
{
	//Define body, the matter
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);

	//Create body using world's factory
	b2Body* bodyBox = CreateBody(&bodyDef);

	//Create a shape, the outline
	b2PolygonShape dynBox;
	dynBox.SetAsBox(1, 1);

	//Create a fixture, the link for body -> shape
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynBox;

	//Add material properties to the fixture
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.1f;

	//Create and add fixture using body's factory
	bodyBox->CreateFixture(&fixtureDef);

	//The body is now in the collision system
	return bodyBox;
}

b2Body * GameWorld::addPlayer(int x, int y)
{	
	//If there's no player, add one
	if (player_ == nullptr)
	{
		player_ = addBody(x, y);
		shape_ = Shape(player_);
	}

	return player_;
}

b2Body * GameWorld::addEnemy(int x, int y)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);

	b2Body* bodyTri = CreateBody(&bodyDef);

	b2PolygonShape dynTri;
	dynTri.Set(0,3);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynTri;

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.1f;

	return bodyTri;
}

void GameWorld::update(float dt)
{
	Step(dt, VELOCITY_ITERS, POSITION_ITERS);
}

Shape * GameWorld::player()
{
	if (&shape_ != nullptr)
		return &shape_;

	else return nullptr;
}
