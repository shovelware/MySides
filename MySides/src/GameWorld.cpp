#include "GameWorld.hpp"

GameWorld::GameWorld() : b2World(GRAVITY), bounds_(addStaticBody(0, 0), 50)
{
}

bool GameWorld::hasControlled()
{
	//Return false if our pointer to the controlled shape is null
	return !(controlled_._Ptr == nullptr);
}

//Adds a dynamic body to the world, returns a pointer to created body
b2Body * GameWorld::addDynamicBody(float x, float y)
{
	//Define body, the matter
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);

	//Create body using world's factory
	b2Body* body = CreateBody(&bodyDef);

	return body;
}

//Adds a static body to the world, returns a pointer to created body
b2Body * GameWorld::addStaticBody(float x, float y)
{
	//Define body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);

	//Create body using world's factory
	b2Body* body = CreateBody(&bodyDef);
	
	return body;
}

Shape * GameWorld::addPlayer(float x, float y)
{	
	//Add the shape with a body
	shapes_.push_back(Shape(addDynamicBody(x, y)));

	//Set our control to the one we just put in
	controlled_ = --shapes_.end();

	return controlled_._Ptr;
}

/*Shape * GameWorld::addEnemy(float x, float y)
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

//AddBullet

void GameWorld::rebound(float radius)
{
	if (radius < bounds_.getRadius())
	{
		//correct for bodies outside radius
	}

	//Recreate the bounds
	bounds_ = Bounds(bounds_.getBody(), radius);
}

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
