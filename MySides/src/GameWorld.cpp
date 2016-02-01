#include "GameWorld.hpp"

//inline float randFloat(float MAX) { return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / MAX)); };
//inline float randFloat(float MIN, float MAX) { return MIN + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (MAX - MIN))); };

//Constructor initialises Box2D World and boudaries
GameWorld::GameWorld() : 
	b2World(GRAVITY), 
	contactListener_(ContactListener())
{
	SetContactListener(&contactListener_);

	bounds_ = new Bounds(addStaticBody(0, 0), 32),

	addProj_ = [this](ProjectileDef& def) { addProjectile(def); };
	//addSide_ = [this](SideDef& def) {addSide(def); };
	
	bgm_.openFromFile("../assets/spriterip.ogg");
	bgm_.setLoop(true);
	bgm_.setVolume(7.5f);

	fireBuffer.loadFromFile("../assets/fire.wav");
	fireSound.setBuffer(fireBuffer);
	fireSound.setMinDistance(100);
	fireSound.setAttenuation(1);

	spawnBuffer.loadFromFile("../assets/spawn.wav");
	spawnSound.setBuffer(spawnBuffer);
	spawnSound.setMinDistance(100);
	spawnSound.setAttenuation(1);

	dieBuffer.loadFromFile("../assets/die.wav");
	dieSound.setBuffer(dieBuffer);
	dieSound.setMinDistance(100);
	dieSound.setAttenuation(1);

	lossBuffer.loadFromFile("../assets/loss.wav");
	lossSound.setBuffer(lossBuffer);
	lossSound.setMinDistance(100);
	lossSound.setAttenuation(1);

	collectBuffer.loadFromFile("../assets/collect.wav");
	collectSound.setBuffer(collectBuffer);
	collectSound.setMinDistance(100);
	collectSound.setAttenuation(1);
}

GameWorld::~GameWorld()
{
	clearWorld();
	delete bounds_;
}

//Returns true if the gameworld has a controlled entity
bool GameWorld::hasControlled()
{
	//Return false if our pointer to the controlled shape is null
	return (controlled_ != nullptr);
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
	fric.bodyB = bounds_->getBody();

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

//Spawn a random enemy
void GameWorld::spawnEnemy()
{
	for (int i = 0; i < spawns_; ++i)
	{
		float x, y, rad = getBoundsRadius() * 0.7f;
		y = -(cos((2 * M_PI) * 32 / randFloat(0, 32)));
		x = -(sin((2 * M_PI) * 32 / randFloat(0, 32)));

		x *= rad;
		y *= rad;

		addEnemy(x, y);
	}
}

//Positions the passed sound relative to the player
void GameWorld::positionSound(sf::Sound & sound, b2Vec2 pos, bool scale = true)
{
	sf::Vector3f sndPos(pos.x * (scale ? _SCALE_ : 1.f), pos.y * (scale ? _SCALE_ : 1.f), 0);
	sound.setPosition(sndPos);
}

//Positions the listener at the player
void GameWorld::positionListener(b2Vec2 pos, bool scale = true)
{
	sf::Vector3f lisPos(pos.x * (scale ? _SCALE_ : 1.f), pos.y * (scale ? _SCALE_ : 1.f), 0);
	sf::Listener::setPosition(lisPos);
}

//Adds a player to the world 
void GameWorld::addPlayer(float x, float y, bool control)
{
	ShapeDef play = ShapeDef(b2Vec2(x, y), b2Vec2_zero, 5);

	player_ = new Shape(addDynamicBody(x, y), play);

	player_->setAI(false);
	
	if (control)
	{
		//Set our control to the one we just put in
		player_->setControlled(true);
		controlled_ = player_;

		ProjectileDef newDef = ProjectileDef::bulletDef();
		newDef.velScale = 1.f;
		newDef.damageScale = 1.f;
		newDef.size = 1;

		player_->arm(new Weapon::Shotgun(&*player_, addProj_, newDef));
	}

}

//Adds a basic enemy to the world
void GameWorld::addEnemy(float x, float y)
{
	//Push the shape into the shape vector
	//AND add body to world with function
	shapes_.push_back(new Shape(addDynamicBody(x, y), 4, .5f));
	Shape* added = *(--shapes_.end());

	ProjectileDef newDef = ProjectileDef::bulletDef();
	newDef.velScale = 0.5f;
	newDef.damageScale = 1.f;
	newDef.size = 1;

	Weapon::WeaponI* newWeap;
	
	//if (coinFlip())
	//{
	//	newWeap = new Weapon::Shotgun(&*added, addProj_, newDef);
	//}
	//
	//else
	//{
		newWeap = new Weapon::Rifle(&*added, addProj_, newDef);
	//}

	added->arm(newWeap);
	added->setAI(true);
	added->setControlled(false);
}

//Adds a projectile to the world
//void GameWorld::addProjectile(float x, float y, float vx, float vy)
//{
//	ProjectileDef p(b2Vec2(x, y), b2Vec2 (vx, vy));
//	p.owner = &*controlled_;
//
//	////
//	//p.inVelocity = controlled_->getBody()->GetLinearVelocity();
//	projectiles_.emplace_back(addBulletBody(x, y), p);
//}

//Adds a projectile to the world via definition
void GameWorld::addProjectile(ProjectileDef &def)
{
	projectiles_.push_back(new Projectile(addBulletBody(def.origin.x, def.origin.y), def));

	//Play fire sound at fired position
	positionSound(fireSound, def.origin);
	fireSound.play();
}

//Adds a side to the world
void GameWorld::addSide(float x, float y, float nx, float ny, float size)
{	
	sides_.push_back(new Side(addDynamicBody(x, y), b2Vec2(nx, ny), size));
}

void GameWorld::removePlayer()
{
	delete player_;
	player_ = nullptr;
}

//Removes enemy from the world and increments iterator, for use within loops
void GameWorld::removeEnemy(std::list<Shape*>::iterator& e)
{
	//Play death sound at position
	positionSound(dieSound, (*e)->getPosition());
	dieSound.play();

	//Delete enemy
	DestroyBody((*e)->getBody());
	delete (*e);

	e = shapes_.erase(e);
}

//Removes projectile from the world and increments iterator, for use within loops
void GameWorld::removeProjectile(std::list<Projectile*>::iterator& p)
{
	DestroyBody((*p)->getBody());
	delete (*p);

	p = projectiles_.erase(p);
}

//Removes side from the world and increments iterator, for use within loops
void GameWorld::removeSide(std::list<Side*>::iterator & s)
{
	positionSound(collectSound, (*s)->getPosition());
	collectSound.play();

	DestroyBody((*s)->getBody());
	s = sides_.erase(s);
}

void GameWorld::resetLevel()
{
	//Clear world
	clearWorld();
	timeInLevel_ = 0;
	lastSpawn_ = 0;
	spawnTime_ = 5000;

	//Add a new player
	addPlayer(0, 0, true);

	spawnSound.play();

	//Restart bgm
	//bgm_.play();

	//Regenerate level somehow
}

void GameWorld::clearWorld()
{
	player_ = nullptr;

	if (shapes_.empty() == false)
	{
		for (std::list<Shape*>::iterator s = shapes_.begin();
		s != shapes_.end(); /*Don't increment here*/)
		{
			removeEnemy(s);
		}
	}
	shapes_.clear();

	if (projectiles_.empty() == false)
	{
		for (std::list<Projectile*>::iterator p = projectiles_.begin();
		p != projectiles_.end(); /*Don't increment here*/)
		{
			removeProjectile(p);
		}
	}
	projectiles_.clear();

	if (sides_.empty() == false)
	{
		for (std::list<Side*>::iterator s = sides_.begin();
		s != sides_.end(); /*Don't increment here*/)
		{
			removeSide(s);
		}
	}
	sides_.clear();
}

//Returns the radius of the level bounds
float GameWorld::getBoundsRadius()
{
	return bounds_->getRadius();
}

//Resizes the bounds to the passed radius, [correcting for shapes outside](not yet)
void GameWorld::resizeBounds(float radius)
{
	if (radius < bounds_->getRadius())
	{
		//correct for bodies outside radius
	}

	//Recreate the bounds
	bounds_->resize(radius);
}

//Returns the length of one side of bounds
float GameWorld::getBoundsSide()
{
	return bounds_->getSideLength();
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
		controlled_->fire(direction);
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

Bounds*& GameWorld::getBounds() { return bounds_; }
std::list<Shape*>& GameWorld::getShapes() {	return shapes_; }
Shape*& GameWorld::getPlayer() { return player_; }
std::list<Projectile*>& GameWorld::getProjectiles() { return projectiles_; }
std::list<Side*>& GameWorld::getSides() { return sides_; }

//Update entity code and step the world
void GameWorld::update(int dt)
{
	//Players update first
	if (player_ != nullptr)
	{
		player_->update(dt);
		positionListener(player_->getPosition());

		////
		player_->setActive(true);//Debug invincible players
		
		//If we're not active
		if (player_->getActive() == false)
		{
			////Add a side
			//static float side = 1.f;
			//b2Vec2 pos = p->getPosition();
			//addSide(pos.x, pos.y, 0, 0, side++);
			//
			removePlayer();
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
		for (std::list<Shape*>::iterator shapeIt = shapes_.begin();
		shapeIt != shapes_.end(); /*Don't increment here*/)
		{
			//Pull pointer from iter for readability
			Shape* shp = (*shapeIt);

			shp->update(dt);

			//Demo AI
			if (shp->getAI() && player_ != nullptr)
			{
				b2Vec2 playerPos = controlled_->getPosition();
				b2Vec2 ePos = shp->getPosition();
				b2Vec2 between = playerPos - ePos;

				if (between.Length() > 40  || between.Length() < 2.5f)
				{

				}

				else if (between.Length() < 25 && (shp->getHP() >= shp->getHPMax() / 2))
				{
					shp->move(between);

					if (shp->getArmed())
					{
						shp->fire(between);
					}
				}

				else if (between.Length() < 10 * (shp->getHPMax() - shp->getHP())) 
				{
					shp->move(-between);
				}
			}

			//If we're not active, increment by deleting
			if (shp->getActive() == false)
			{
				//Add a side
				static float side = 1.f;
				b2Vec2 pos = shp->getPosition();
				addSide(pos.x, pos.y, 0, 0, side);
				removeEnemy(shapeIt);
			}

			//Else just increment
			else ++shapeIt;
		}
	}

	//Firebullets

	if (projectiles_.empty() == false)
	{
		for (std::list<Projectile*>::iterator projIt = projectiles_.begin();
			projIt != projectiles_.end(); /*Don't increment here*/)
		{
			//Pull pointer from it for readability
			Projectile* prj = (*projIt);

			prj->update(dt);

			//If we're not active, increment by deleting
			if (prj->getActive() == false)
			{
				removeProjectile(projIt);
			}

			//Else just increment
			else ++projIt;
		}
	}

	if (sides_.empty() == false)
	{
		for (std::list<Side*>::iterator sideIt = sides_.begin();
		sideIt != sides_.end(); /*Don't increment here*/)
		{
			//Pull pointer
			Side* sd = (*sideIt);

			//If we're not active, increment by deleting
			if (sd->getActive() == false)
			{
				removeSide(sideIt);
			}

			//Else just increment
			else ++sideIt;
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

	timeInLevel_ += dt;
	timeInLevel_ % UINT16_MAX;

	if (((timeInLevel_ - lastSpawn_) % UINT16_MAX) > spawnTime_)
	{
		spawnEnemy();
		lastSpawn_ = timeInLevel_;
		spawns_ = 1 % UINT16_MAX;
	}

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
	return player_;
}

//Handlers for game intent: Move, Fire, Select, Triggers

