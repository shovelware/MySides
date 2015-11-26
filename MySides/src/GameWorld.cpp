#include "GameWorld.hpp"

//Constructor initialises 
GameWorld::GameWorld() : b2World(GRAVITY), bounds_(addStaticBody(15, 10), 12)
{
}

//Returns true if the gameworld has a controlled entity
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

b2Body * GameWorld::addBulletBody(float x, float y)
{

}

//Adds a player to the world
Shape * GameWorld::addPlayer(float x, float y)
{	
	//Push the shape into shape vector 
	//AND add body to world with function
	shapes_.push_back(Shape(addDynamicBody(x, y)));

	//Set our control to the one we just put in
	controlled_ = --shapes_.end();

	return controlled_._Ptr;
}

//Adds a basic enemy to the world
Shape * GameWorld::addEnemy(float x, float y)
{
	//Push the shape into the shape vector
	//AND add body to world with function
	shapes_.push_back(Shape(addDynamicBody(x, y)));

	//Return a pointer to this enemy
	return (--shapes_.end())._Ptr;
}

//AddBullet

//Resizes the bounds to the passed radius, [correcting for shapes outside](not yet)
void GameWorld::resizeBounds(float radius)
{
	if (radius < bounds_.getRadius())
	{
		//correct for bodies outside radius
	}

	//Recreate the bounds
	bounds_ = Bounds(bounds_.getBody(), radius);
}

//Moves controlled pointer to next shape in list, loops at end
void GameWorld::controlNext()
{
		//Move forward
		controlled_++;

		//If we've hit the end, loop around
		if (controlled_ == shapes_.end())
		{
			controlled_ = shapes_.begin();
		}
}

//Moves controlled pointer to previous shape in list, loops at start
void GameWorld::controlPrev()
{
	//If we hit the end, loop around
	if (controlled_ == shapes_.begin())
	{
		controlled_ = --shapes_.end();
	}
	
	//Move backwards
	else controlled_--;
}

//Update entity code and step the world
void GameWorld::update(float dt)
{
	//Update Shapes
	//Firebullets
	Step(dt, VELOCITY_ITERS, POSITION_ITERS);
}

//Gets a pointer to the controlled shape
Shape * GameWorld::player()
{
	//If we have a controlled character
	if (controlled_._Ptr != nullptr)
		return &(*controlled_);

	//Otherwise
	else return nullptr;
}
