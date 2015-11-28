#include "GameWorld.hpp"

//Constructor initialises Box2D World and boudaries
GameWorld::GameWorld() : b2World(GRAVITY), bounds_(addStaticBody(15, 10), 10), contactListener_(ContactListener())
{
	SetContactListener(&contactListener_);
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
	//Define the body
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
	//Define the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);

	//Create body using world's factory
	b2Body* body = CreateBody(&bodyDef);
	
	return body;
}

//Adds a bullet body to the world, returns a pointer to created body
b2Body * GameWorld::addBulletBody(float x, float y)
{
	//Define the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.bullet = true;
	bodyDef.position.Set(x, y);

	//Create body using world's factory
	b2Body* body = CreateBody(&bodyDef);

	return body;
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

// Adds a projectile to the world
Projectile* GameWorld::addProjectile(float x, float y, float vx, float vy)
{
	projectiles_.push_back(Projectile(addDynamicBody(x, y), b2Vec2(vx, vy)));

	return (--projectiles_.end())._Ptr;
}

//Resizes the bounds to the passed radius, [correcting for shapes outside](not yet)
void GameWorld::resizeBounds(float radius)
{
	if (radius < bounds_.getRadius())
	{
		//correct for bodies outside radius
	}

	//Recreate the bounds
	bounds_.resize(radius);
}

void GameWorld::move(b2Vec2 direction)
{
	controlled_->move(direction);
}

void GameWorld::fire(b2Vec2 direction)
{
	if (direction.x != 0 || direction.y != 0)
	{
		b2Vec2 fp = controlled_->getFirePoint(direction.x, direction.y); //This will be abstracted to shape class
		addProjectile(fp.x, fp.y, direction.x, direction.y);
	}
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

	for(Projectile p : projectiles_)
	{
		p.update(dt);

		if (p.getActive() == false)
		{
			//DestroyBody(p.getBody());
		}
	}

	//std::remove_if(projectiles_.begin(), projectiles_.end(), isAlive);

	Step(dt, VELOCITY_ITERS, POSITION_ITERS);
}

/*
UpdateShape(Shape* s, int dt)
{
	s->update(dt)
	if (s->getActive == false)
	{
		removeBody(s->getBody);
	}
}
*/

//Gets a pointer to the controlled shape
Shape * GameWorld::player()
{
	//If we have a controlled character
	if (controlled_._Ptr != nullptr)
		return &(*controlled_);

	//Otherwise
	else return nullptr;
}
