#include "GameWorld.hpp"

//Constructor initialises Box2D World and boudaries
GameWorld::GameWorld() : b2World(GRAVITY), bounds_(addStaticBody(15, 10), 10), contactListener_(ContactListener())
{
	SetContactListener(&contactListener_);
}

//Returns true if the gameworld has a controlled entity !!!FIX
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
	bodyDef.bullet = false;
	bodyDef.position.Set(x, y);

	//Create body using world's factory
	b2Body* body = CreateBody(&bodyDef);

	return body;
}

//Adds a player to the world
void GameWorld::addPlayer(float x, float y, bool control)
{
	//Emplace the shape into shape list 
	//AND add body to world with function
	shapes_.emplace_back(addDynamicBody(x, y));

	if (control)
	{
		//Set our control to the one we just put in
		controlled_ = shapes_.begin();
	}

}

//Adds a basic enemy to the world
void GameWorld::addEnemy(float x, float y)
{
	//Push the shape into the shape vector
	//AND add body to world with function
	shapes_.emplace_back(addDynamicBody(x, y));
}

// Adds a projectile to the world
void GameWorld::addProjectile(float x, float y, float vx, float vy)
{
	//projectiles_.push_back(Projectile(addDynamicBody(x, y), b2Vec2(vx, vy)));
	projectiles_.emplace_back(addDynamicBody(x, y), b2Vec2(vx, vy));

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

	if (projectiles_.empty() == false)
	{
		for (std::list<Projectile>::iterator p = projectiles_.begin();
			p != projectiles_.end(); /*Don't increment here*/)
		{
			p->update(dt);

			//If we're not active, increment by deleting
			if (p->getActive() == false)
			{
				DestroyBody(p->getBody());
				projectiles_.erase(p++);
			}

			//Else just increment
			else ++p;
		}
	}

	////What a body has
	//void* s = (GetBodyList()->GetUserData());
	////Dynamic cast what body has
	//Shape* dcs = static_cast<Shape*>(s);
	////What body should have
	//Shape* as = &*shapes_.begin();
	//
	////What shape has
	//b2Body* b = shapes_.begin()->getBody();
	////What shape should have
	//b2Body* ab = GetBodyList();

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
