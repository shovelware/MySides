#include "GameWorld.hpp"

GameWorld::GameWorld() : b2World(GRAVITY)
{
}

bool GameWorld::hasControlled()
{
	//Return false if our pointer to the controlled shape is null
	return !(controlled_._Ptr == nullptr);
}

b2Body * GameWorld::addBody(int x, int y)
{
	//Define body, the matter
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);

	//Create body using world's factory
	b2Body* body = CreateBody(&bodyDef);

	return body;
}

Shape * GameWorld::addPlayer(int x, int y)
{	
	//Add the shape with a body
	shapes_.push_back(Shape(addBody(x, y)));

	//Set our control to the one we just put in
	controlled_ = --shapes_.end();

	return controlled_._Ptr;
}

/*Shape * GameWorld::addEnemy(int x, int y)
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
}*/

void GameWorld::update(float dt)
{
	Step(dt, VELOCITY_ITERS, POSITION_ITERS);
}

Shape * GameWorld::player()
{
	//If we have a controlled character
	if (controlled_._Ptr != nullptr)
		return &(*controlled_);

	//Otherwise
	else return nullptr;
}
