#include "GameWorld.hpp"

//Constructor initialises Box2D World and boudaries
GameWorld::GameWorld() : b2World(GRAVITY), bounds_(addStaticBody(0, 0), 32), contactListener_(ContactListener())
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

	//Dynamic bodies have friction with bounds
	b2FrictionJointDef fric;
	fric.localAnchorA = (b2Vec2(0, 0));
	fric.localAnchorB = (b2Vec2(0, 0));

	fric.bodyA = body;
	fric.bodyB = bounds_.getBody();

	fric.maxForce	= 0.0001f;
	fric.maxTorque	= 0.005f;

	fric.collideConnected = true;

	CreateJoint(&fric);

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
void GameWorld::addPlayer(float x, float y, bool control)
{
	//Emplace the shape into shape list 
	//AND add body to world with function
	players_.emplace_back(addDynamicBody(x, y), 4, .5f);

	std::list<Shape>::iterator added = --players_.end();
	added->setAI(false);
	
	if (control)
	{
		//Set our control to the one we just put in
		added->setControlled(true);
		controlled_ = added;
	}

}

//Adds a basic enemy to the world
void GameWorld::addEnemy(float x, float y)
{
	//Push the shape into the shape vector
	//AND add body to world with function
	shapes_.emplace_back(addDynamicBody(x, y), 4, .5f);
	std::list<Shape>::iterator added = --shapes_.end();
	added->setAI(true);
	added->setControlled(false);
}

//Adds a projectile to the world
void GameWorld::addProjectile(float x, float y, float vx, float vy)
{
	ProjectileDef p = ProjectileDef(b2Vec2(x, y), b2Vec2 (vx, vy));
	p.body = addBulletBody(x, y);
	p.owner = &*controlled_;

	////
	p.inVelocity = controlled_->getBody()->GetLinearVelocity();

	if (p.isValid())
	{
		projectiles_.emplace_back(p);
	}
}

//Adds a side to the world
void GameWorld::addSide(float x, float y, float nx, float ny, float size)
{
	sides_.emplace_back(addDynamicBody(x, y), b2Vec2(nx, ny), size);
}

void GameWorld::removePlayer(std::list<Shape>::iterator& p)
{
	DestroyBody(p->getBody());
	shapes_.erase(p++);
}

//Removes enemy from the world and increments iterator, for use within loops
void GameWorld::removeEnemy(std::list<Shape>::iterator& e)
{
	DestroyBody(e->getBody());
	shapes_.erase(e++);
}

//Removes projectile from the world and increments iterator, for use within loops
void GameWorld::removeProjectile(std::list<Projectile>::iterator& p)
{
	DestroyBody(p->getBody());
	projectiles_.erase(p++);
}

//Removes side from the world and increments iterator, for use within loops
void GameWorld::removeSide(std::list<Side>::iterator & s)
{
	DestroyBody(s->getBody());
	sides_.erase(s++);
}

//Returns the radius of the level bounds
float GameWorld::getBoundsRadius()
{
	return bounds_.getRadius();
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

//Returns the length of one side of bounds
float GameWorld::getBoundsSide()
{
	return bounds_.getSideLength();
}

void GameWorld::move(b2Vec2 direction)
{
	if (direction.x != 0 || direction.y != 0)
	{
		controlled_->move(direction);
	}
}

void GameWorld::fire(b2Vec2 direction)
{
	//If there's a direction to fire in
	if (direction.x != 0 || direction.y != 0)
	{
		//And we can fire
		if (controlled_->getArmed())
		{

			b2Vec2 fp = controlled_->getFirePoint(direction.x, direction.y); //This will be abstracted to shape class
			b2Transform bxf = bounds_.getBody()->GetTransform();

			//Test if we're shooting on the same side of the bounds
			bool projIn = bounds_.getShape()->TestPoint(bxf, fp);
			bool shapeIn = bounds_.getShape()->TestPoint(bxf, controlled_->getPosition());

			if (projIn == shapeIn)
			{
				addProjectile(fp.x, fp.y, direction.x, direction.y);
			}
		}
	}
}

//Moves controlled pointer to next shape in list, loops at end NEEDS REWRITE
void GameWorld::controlNextEnemy()
{
		////Move forward
		//controlled_++;
		//
		////If we've hit the end, loop around
		//if (controlled_ == shapes_.end())
		//{
		//	controlled_ = shapes_.begin();
		//}
}

//Moves controlled pointer to previous shape in list, loops at start NEEDS REWRITE
void GameWorld::controlPrevEnemy()
{
	////If we hit the end, loop around
	//if (controlled_ == shapes_.begin())
	//{
	//	controlled_ = --shapes_.end();
	//}
	//
	////Move backwards
	//else controlled_--;
}

//Sets debugDraw of world to passed, inherited function
void GameWorld::SetDebugDraw(b2Draw* debugDraw)
{
	b2World::SetDebugDraw(debugDraw);
}

//Draw's world's debug data, inherited function
void GameWorld::DrawDebugData()
{
	b2World::DrawDebugData();
}

std::list<Shape>& GameWorld::getShapes()
{
	return shapes_;
}

Bounds & GameWorld::getBounds()
{
	return bounds_;
}

std::list<Shape>& GameWorld::getPlayers()
{
	return players_;
}

std::list<Projectile>& GameWorld::getProjectiles()
{
	return projectiles_;
}

std::list<Side>& GameWorld::getSides()
{
	return sides_;
}

//Update entity code and step the world
void GameWorld::update(int dt)
{
	//Players update first
	if (players_.empty() == false)
	{
		for (std::list<Shape>::iterator p = players_.begin();
		p != players_.end(); /*Don't increment here*/)
		{
			p->setActive(true);
			p->update(dt);
			
			//If we're not active, increment by deleting
			if (p->getActive() == false)
			{
				////Add a side
				//static float side = 1.f;
				//b2Vec2 pos = p->getPosition();
				//addSide(pos.x, pos.y, 0, 0, side++);
				//
				p++;//removePlayer(p);
			}

			//Else just increment
			else ++p;
		}

	}

	////New Shape Update
	//if (shapes_.empty() == false)
	//{
	//	for (std::list<Shape>::iterator s = shapes_.begin();
	//	s != shapes_.end(); /*Don't increment here*/)
	//	{
	//		//Update the shape
	//		s->update(dt);
	//
	//		//Spawn sides if we have any
	//		if (s->hasSides())
	//		{
	//			addSides(s);
	//		}
	//		
	//		//If we're alive
	//		if (s->getAlive() == true)
	//		{ 
	//			Add any projectiles (Move alive check to shape?)
	//			if (s->hasProjectiles())
	//			{
	//				addProjectiles(s);
	//			}
	//		}
	//
	//		//If we're not active, increment by deleting
	//		if (s->getActive() == false)
	//		{
	//			//Add a side
	//			static float side = 1.f;
	//			b2Vec2 pos = s->getPosition();
	//			addSide(pos.x, pos.y, 0, 0, side++);
	//
	//			removeEnemy(s);
	//		}
	//
	//		//Else just increment
	//		else ++s;
	//	}
	//}

	//Update Shapes
	if (shapes_.empty() == false)
	{
		for (std::list<Shape>::iterator s = shapes_.begin();
		s != shapes_.end(); /*Don't increment here*/)
		{
			s->update(dt);

			//Demo AI
			if (s->getAI())
			{
				b2Vec2 playerPos = controlled_->getPosition();
				b2Vec2 ePos = s->getPosition();
				b2Vec2 between = playerPos - ePos;

				if (between.Length() > 40  || between.Length() < 5)
				{

				}

				else if (between.Length() < 25 && (s->getHP() >= s->getMaxHP() / 2))
				{
					s->move(between);

					if (s->getArmed())
					{
						b2Vec2 fp = s->getFirePoint(between.x, between.y);

						//addProjectile(fp.x, fp.y, between.x, between.y);
					}
				}

				else if (between.Length() < 10 * (s->getMaxHP() - s->getHP())) 
				{
					s->move(-between);
				}
			}

			//If we're not active, increment by deleting
			if (s->getActive() == false)
			{
				//Add a side
				static float side = 1.f;
				b2Vec2 pos = s->getPosition();
				addSide(pos.x, pos.y, 0, 0, side);
				
				removeEnemy(s);
			}

			//Else just increment
			else ++s;
		}
	}

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
				removeProjectile(p);
			}

			//Else just increment
			else ++p;
		}
	}

	if (sides_.empty() == false)
	{
		for (std::list<Side>::iterator s = sides_.begin();
		s != sides_.end(); /*Don't increment here*/)
		{
			//If we're not active, increment by deleting
			if (s->getActive() == false)
			{
				removeSide(s);
			}

			//Else just increment
			else ++s;
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
Shape * GameWorld::controlled()
{
	//If we have a controlled character
	if (controlled_._Ptr != nullptr)
		return &(*controlled_);

	//Otherwise
	else return nullptr;
}

//Handlers for game intent: Move, Fire, Select, Triggers

