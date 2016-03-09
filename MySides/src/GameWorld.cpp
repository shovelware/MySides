#include "GameWorld.hpp"

//Constructor initialises Box2D World and boudaries
GameWorld::GameWorld() :
	b2World(GRAVITY),
	contactListener_(ContactListener()),
	player_(nullptr),
	controlled_(nullptr),
	pause_(false)
{
	SetContactListener(&contactListener_);

	bounds_ = new Bounds(addStaticBody(b2Vec2_zero), 32);

	addProj_ = [this](ProjectileDef& def) { addProjectile(def); };
	addSide_ = [this](SideDef& def) { addSide(def); };

	//audio_.addBGM("spriterip", "../assets/spriterip.ogg");
	audio_.addAFX("spriterip", "../assets/spriterip.ogg", 1, 0.2, 400, 1024);
	audio_.addAFX("wind", "../assets/wind.ogg", 0, 1, 750, 1000);

	audio_.addSFX("fire", "../assets/fire.wav", 64);
	audio_.addSFX("spawn", "../assets/spawn.wav", 16);
	audio_.addSFX("die", "../assets/die.wav", 32);
	audio_.addSFX("loss", "../assets/loss.wav", 1);
	audio_.addSFX("drop", "../assets/drop.wav", 64);
	audio_.addSFX("collect", "../assets/collect.wav", 32);
	
	getControlled_ = std::bind(&GameWorld::getControlled, this);
}

GameWorld::~GameWorld()
{
	clearWorld();
	delete player_;
	delete bounds_;
}

sf::Vector2f GameWorld::B2toSF(const b2Vec2& vec, bool scale) const
{
	return sf::Vector2f(vec.x * (scale ? _SCALE_ : 1.f), vec.y * (scale ? _SCALE_ : 1.f));
}

//Returns true if the gameworld has a controlled entity
bool GameWorld::hasControlled()
{
	//Return false if the pointer to our controlled shape is null
	return (controlled_ != nullptr);
}

//Adds a dynamic body to the world, returns a pointer to created body
b2Body * GameWorld::addDynamicBody(const b2Vec2& pos)
{
	//Define the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = pos;

	//Create body using world's factory
	b2Body* body = CreateBody(&bodyDef);

	//Dynamic bodies have friction with bounds
	b2FrictionJointDef fric;
	fric.localAnchorA = (b2Vec2(0, 0));
	fric.localAnchorB = (b2Vec2(0, 0));

	fric.bodyA = body;
	fric.bodyB = bounds_->getBody();

	fric.maxForce	= 0.0001f;
	fric.maxTorque = 0.0005f;

	fric.collideConnected = false;

	CreateJoint(&fric);

	return body;
}

//Adds a static body to the world, returns a pointer to created body
b2Body * GameWorld::addStaticBody(const b2Vec2& pos)
{
	//Define the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position = pos;

	//Create body using world's factory
	b2Body* body = CreateBody(&bodyDef);
	
	return body;
}

//Adds a bullet body to the world, returns a pointer to created body
b2Body * GameWorld::addBulletBody(const b2Vec2& pos)
{
	//Define the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.bullet = true;
	bodyDef.position = pos;
	//Create body using world's factory
	b2Body* body = CreateBody(&bodyDef);

	return body;
}

//Pops an entity back inside the bounds
void GameWorld::popInside(Entity * ent)
{
	//Bounds centre on origin so between is just pos
	float rad = bounds_->getRadius();

	b2Vec2 position = ent->getPosition();
	b2Vec2 boundsEdge = position;
	boundsEdge.Normalize();
	boundsEdge *= rad;

	b2Vec2 between = position - boundsEdge;

	float dist = position.Length();

	bool inside = bounds_->getPoly()->TestPoint(bounds_->getBody()->GetTransform(), position);

	if (dist > rad || dist < 0)
	{
		//Pop inside
		//between.Normalize();
		//between *= (rad * .9f);
		//ent->setPosition(between);

		//Push inside
		between *= 0.005f;
		ent->getBody()->ApplyForceToCenter(-between, true);
	}
}

//Spawn a random enemy
void GameWorld::spawnEnemy()
{
	for (int i = 0; i < spawns_; ++i)
	{
		b2Vec2 pos;
		float rad = getBoundsRadius() * 0.7f;
		pos.y = -(cos((2 * M_PI) * 32 / randFloat(0, 32)));
		pos.x = -(sin((2 * M_PI) * 32 / randFloat(0, 32)));

		pos *= rad;

		addEnemy(pos);
	}
}

//Adds a player to the world 
void GameWorld::addPlayer(const b2Vec2& pos, bool control)
{
	ShapeDef play = ShapeDef(pos, b2Vec2_zero, 5);
	play.colPrim = b2Color(0.6f, 0.3f, 0.9f);
	play.colSecn = b2Color(0.f, 1.f, 1.f);
	play.colTert = b2Color(1.f, 0.f, 0.f);

	if (player_ != nullptr)
	{
		delete player_;
	}

	player_ = new Player(addDynamicBody(pos), play, addSide_);
	
	if (control)
	{
		//Set our control to the one we just put in
		controlled_ = player_;

		ProjectileDef newDef = ProjectileDef::bulletDef();
		newDef.velScale = 1.f;
		newDef.lifeTime = 3000;
		newDef.damageScale = 4.f;
		newDef.hpMAX = 1.f;
		newDef.size = 0.1f;
		newDef.bounce = 1.f;

		player_->arm(new Weapon::Shotgun(&*player_, addProj_, newDef));
	}

}

//Adds a basic enemy to the world
void GameWorld::addEnemy(const b2Vec2& pos)
{
	//Push the shape into the shape vector
	//AND add body to world with function
	ShapeDef enem = ShapeDef(pos, b2Vec2_zero, static_cast<int>(randFloat(3, 8) + 1));
	//ShapeDef enem = ShapeDef(b2Vec2(x, y), b2Vec2_zero, -1);
	enem.size = .5f;
	enem.speedScale = .5f;

	enem.colPrim = b2Color(randFloat(0.9f, 1.f), randFloat(0.f, 1.f), 0.f);
	enem.colSecn = b2Color(randFloat(0.6f, 1.f), randFloat(0.6f, 1.f), 0.f);
	enem.colTert = b2Color(randFloat(0.5f, 1.f), randFloat(0.1f, 0.3f), randFloat(0.1f, 0.3f));

	shapes_.push_back(new Enemy(addDynamicBody(enem.position), enem, addSide_, getControlled_));

	Shape* added = *(--shapes_.end());

	ProjectileDef newDef = ProjectileDef::bulletDef();
	newDef.velScale = 0.5f;
	newDef.damageScale = 1.f;
	newDef.size = 1;

	Weapon::WeaponI* newWeap;
	
	if (coinFlip())
	{
		newWeap = new Weapon::Shotgun(&*added, addProj_, newDef);
	}
	
	else
	{
		newWeap = new Weapon::Rifle(&*added, addProj_, newDef);
	}

	added->arm(newWeap);

	audio_.playSFX("spawn", B2toSF(pos, true));

	enemies++;
}

//Adds a projectile to the world via definition
void GameWorld::addProjectile(const ProjectileDef& def)
{
	projectiles_.push_back(new Projectile(addBulletBody(def.origin), def));

	//Play fire sound at fired position
	audio_.playSFX("fire", B2toSF(def.origin, true));
}

//Adds a side to game world via definition
void GameWorld::addSide(const SideDef& def)
{
	sides_.push_back(new Side(addDynamicBody(def.position), def));
	
	audio_.playSFX("drop", B2toSF(def.position, true));

	freesides++;
}

//Removes the player from the game world
void GameWorld::removePlayer()
{
	if (controlled_ == player_)
	{
		controlled_ = nullptr;
	}

	if (player_ != nullptr)
	{
		//Play loss sound at position
		audio_.playSFX("loss", B2toSF(player_->getPosition(), true));

		DestroyBody(player_->getBody());

		delete player_;
		player_ = nullptr;
	}
}

//Removes enemy from the world and increments iterator, for use within loops
void GameWorld::removeEnemy(std::list<Enemy*>::iterator& e)
{
	//Play death sound at position
	audio_.playSFX("die", B2toSF((*e)->getPosition(), true));

	//Delete enemy
	DestroyBody((*e)->getBody());
	delete (*e);
	e = shapes_.erase(e);

	enemies--;
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
	audio_.playSFX("collect", B2toSF((*s)->getPosition(), true));

	DestroyBody((*s)->getBody());
	s = sides_.erase(s);

	freesides--;
}

//Resets the level
void GameWorld::resetLevel()
{
	//Clear world
	clearWorld();
	timeInLevel_ = 0;
	lastSpawn_ = 0;
	spawnTime_ = 5000;

	//Add a new player
	addPlayer(b2Vec2_zero, true);

	audio_.playSFX("spawn", B2toSF(b2Vec2_zero, true));
	spawns_ = 1;

	//Restart bgm
	audio_.playAFX("spriterip");
	audio_.playAFX("wind");
	//Regenerate level somehow
	hiSides = 0;
	enemies = 0;
	freesides = 0;
}

void GameWorld::clearWorld()
{
	removePlayer();

	if (shapes_.empty() == false)
	{
		for (std::list<Enemy*>::iterator e = shapes_.begin();
		e != shapes_.end(); /*Don't increment here*/)
		{
			removeEnemy(e);
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

void GameWorld::bomb()
{
	//Explode shapes
	if (shapes_.empty() == false)
	{
		for (std::list<Enemy*>::iterator shapeIt = shapes_.begin();
		shapeIt != shapes_.end(); ++shapeIt)
		{
			(*shapeIt)->explode();
		}
	}

	//Baleet projectiles

	if (projectiles_.empty() == false)
	{
		for (std::list<Projectile*>::iterator projIt = projectiles_.begin();
		projIt != projectiles_.end(); ++projIt)
		{
			(*projIt)->setActive(false);
		}
	}

}

void GameWorld::testBed()
{
	randomiseCol(bounds_);
	//audio_.pauseBGM();
}

//Returns the radius of the level bounds
float GameWorld::getBoundsRadius()
{
	return bounds_->getRadius();
}

//Resizes the bounds to the passed radius
void GameWorld::resizeBounds(float radius)
{
	if (radius < bounds_->getRadius())
	{
		//Gameworld's pop inside should
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

void GameWorld::randomiseCol(Entity * e)
{
	e->setPrimary(b2Color(randFloat(0.f, 1.f), randFloat(0.f, 1.f), randFloat(0.f, 1.f)));
	e->setSecondary(b2Color(randFloat(0.f, 1.f), randFloat(0.f, 1.f), randFloat(0.f, 1.f)));
	e->setTertiary(b2Color(randFloat(0.f, 1.f), randFloat(0.f, 1.f), randFloat(0.f, 1.f)));
}

Bounds* GameWorld::getBounds() { return bounds_; }
Shape* GameWorld::getPlayer() { return player_; }
std::list<Enemy*>& GameWorld::getShapes() {	return shapes_; }
std::list<Projectile*>& GameWorld::getProjectiles() { return projectiles_; }
std::list<Side*>& GameWorld::getSides() { return sides_; }

//Update entity code and step the world
void GameWorld::update(int dt)
{
	if (!pause_)
	{
		//Players update first
		if (player_ != nullptr)
		{
			player_->update(dt);
			popInside(player_);

			audio_.setListener(B2toSF(player_->getPosition(), true));

			////player_->setActive(true);//Debug invincible players

			//If we're not active
			if (player_->getActive() == false)
			{
				removePlayer();
			}
		}

		//Update Shapes
		if (shapes_.empty() == false)
		{
			for (std::list<Enemy*>::iterator shapeIt = shapes_.begin();
			shapeIt != shapes_.end(); /*Don't increment here*/)
			{
				//Pull pointer from iter for readability
				Enemy* shp = (*shapeIt);

				shp->update(dt);
				popInside(shp);

				//If we're not active, increment by deleting
				if (shp->getActive() == false)
				{
					removeEnemy(shapeIt);
				}

				//Else just increment
				else ++shapeIt;
			}
		}

		//Update projectiles
		if (projectiles_.empty() == false)
		{
			for (std::list<Projectile*>::iterator projIt = projectiles_.begin();
			projIt != projectiles_.end(); /*Don't increment here*/)
			{
				//Pull pointer from it for readability
				Projectile* prj = (*projIt);

				prj->update(dt);
				popInside(prj);

				//If we're not active, increment by deleting
				if (prj->getActive() == false)
				{
					removeProjectile(projIt);
				}

				//Else just increment
				else ++projIt;
			}
		}

		//Update Sides
		if (sides_.empty() == false)
		{
			for (std::list<Side*>::iterator sideIt = sides_.begin();
			sideIt != sides_.end(); /*Don't increment here*/)
			{
				//Pull pointer
				Side* sd = (*sideIt);

				if (player_ != nullptr)
				{
					b2Vec2 between = player_->getPosition() - sd->getPosition();

					if (between.Length() < 7.5f)
						sd->attract(between);
					
				}

				popInside(sd);

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

		//Basic difficulty ramp
		timeInLevel_ += dt;
		timeInLevel_ % UINT16_MAX;

		if (((timeInLevel_ - lastSpawn_) % UINT16_MAX) > spawnTime_ / 5)
		{
			//VIDOE
			static int i = 0;
			int num = 32;
			
			if (i < num)
			{
				b2Vec2 pos(
					-(cos((2 * M_PI) / num * i)),
					-(sin((2 * M_PI) / num * i)));

				pos *= 20.0f;
				//addEnemy(pos);
			}

			i++;
			//VIDEO

			//lastSpawn_ = timeInLevel_;
			//
			////if we want to do less than double the enemy number
			//if (enemies < spawns_ * 2)
			//{
			//	spawnEnemy();
			//	spawns_ = (spawns_ + 1 % UINT16_MAX > 10 ? 10 : spawns_ + 1 % UINT16_MAX);
			//}

		}


		Step(dt, VELOCITY_ITERS, POSITION_ITERS);

		if (hasControlled())
		{
			hiSides = player_->getSidesCollected();
			hiTime = timeInLevel_;

			//2 minute time limit
			if (timeInLevel_ >= maxTime * 1000)
			{
				removePlayer();
			}
		}
	}
}

bool GameWorld::getPaused() const { return pause_; }

void GameWorld::pause()
{
	pause_ = true;
}

void GameWorld::resume() 
{ 
	pause_ = false; 
	audio_.resumeBGM();
}

//Gets a pointer to the controlled shape, or null if there isn't one
Shape * GameWorld::getControlled()
{
	return controlled_;
}

//Handlers for game intent: Move, Fire, Select, Triggers
void GameWorld::move(b2Vec2 direction)
{
	if (controlled_ != nullptr && (direction.x != 0 || direction.y != 0))
	{
		controlled_->move(direction);
	}
}
void GameWorld::fire(b2Vec2 direction)
{
	//If there's a direction to fire in
	if (controlled_ != nullptr && (direction.x != 0 || direction.y != 0))
	{
		//Look in a direction
		if (direction.Length() <= 0.75f)
		{
			controlled_->orient(direction);
		}

		else controlled_->fire(direction);
	}
}

