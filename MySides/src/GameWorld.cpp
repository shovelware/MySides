#include "GameWorld.hpp"

//Constructor initialises Box2D World, boundaries, armory
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
	fireWeap_ = [this](std::vector<ProjectileDef>& defs, std::string id) { fireWeapon(defs, id); };
	addForce_ = [this](b2Vec2 pos, float force, float radius, int time) { addForce(pos, force, radius, time); };

	//armory
	armory_ = new Weapon::Armory(fireWeap_);

	//audio_.addBGM("spriterip", "../assets/spriterip.ogg");
	audio_.addAFX("spriterip", "../assets/spriterip.ogg", 1, 0.2, 400, 1024);
	audio_.addAFX("wind", "../assets/wind.ogg", 0, 1, 750, 1000);

	//Weapon noises
	audio_.addSFX("shotgun", "../assets/nsnd/shotty.wav", 8);
	audio_.addSFX("pistol", "../assets/nsnd/sbullet.wav", 16);
	audio_.addSFX("magnum", "../assets/nsnd/mbullet.wav", 16);
	audio_.addSFX("rifle", "../assets/nsnd/bullet.wav", 16);
	audio_.addSFX("cannon", "../assets/nsnd/cannon.wav", 8);

	audio_.addSFX("coilgun", "../assets/nsnd/pew.wav", 16);
	audio_.addSFX("railgun", "../assets/nsnd/bwump.wav", 8);

	audio_.addSFX("rocket", "../assets/nsnd/rock.wav", 8);
	audio_.addSFX("grenade", "../assets/nsnd/gren.wav", 8);
	audio_.addSFX("explosion", "../assets/nsnd/bom.wav", 8);

	//Bomb
	audio_.addSFX("bomb", "../assets/nsnd/boom.wav", 2);

	//Oldies
	audio_.addSFX("spawn", "../assets/spawn.wav", 8);
	audio_.addSFX("die", "../assets/die.wav", 8);
	audio_.addSFX("loss", "../assets/loss.wav", 1);
	audio_.addSFX("drop", "../assets/drop.wav", 8);
	audio_.addSFX("collect", "../assets/collect.wav", 8);
	
	getControlled_ = std::bind(&GameWorld::getControlled, this);
}

GameWorld::~GameWorld()
{
	clearWorld();
	delete armory_;
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
		float rad = getBoundsRadius() * 0.9f;
		pos.y = -(cos((2 * M_PI) * 32 / randFloat(0, 32)));
		pos.x = -(sin((2 * M_PI) * 32 / randFloat(0, 32)));

		pos *= rad;

		addEnemy(pos);
	}
}

//Adds a player to the world 
void GameWorld::addPlayer(const b2Vec2& pos, bool control)
{
	ShapeDef play = ShapeDef(pos, b2Vec2(0, -1), 5);
	
	play.colPrim = b2Color(0.6f, 0.3f, 0.9f);
	play.colSecn = b2Color(0.f, 1.f, 1.f);
	play.colTert = b2Color(1.f, 0.f, 0.f);

	if (player_ != nullptr)
	{
		delete player_;
	}

	player_ = new Player(addDynamicBody(pos), play, addSide_);	
	addPickup(Pickup::Type::ATTRACT, pos, -1);

	if (control)
	{
		//Set our control to the one we just put in
		controlled_ = player_;

		ProjectileDef newDef = armory_->getSlug();
	

		weapons_.push_back(new Weapon::SemiBat(fireWeap_, newDef));// , 500, 32, 64));
		Weapon::WeaponI* newWeap = (*--weapons_.end());


		player_->arm(newWeap);
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
	enem.hpScale = 5;

	enem.colPrim = b2Color(randFloat(0.9f, 1.f), randFloat(0.f, 1.f), 0.f);
	enem.colSecn = b2Color(randFloat(0.6f, 1.f), randFloat(0.6f, 1.f), 0.f);
	enem.colTert = b2Color(randFloat(0.5f, 1.f), randFloat(0.1f, 0.3f), randFloat(0.1f, 0.3f));

	shapes_.push_back(new Enemy(addDynamicBody(enem.position), enem, addSide_, getControlled_));

	Shape* added = *(--shapes_.end());

	ProjectileDef newDef = armory_->getBullet();
	newDef.velScale = 0.5f;
	newDef.damage = 1;
	newDef.width = 1.f;
	newDef.hpMAX = 1;
	//newDef.lifeTime = 100;

	//Weapon::WeaponI* newWeap;
	//
	//if (coinFlip())
	//{
	//	weapons_.push_back(new Weapon::Shotgun(fireWeap_, newDef));
	//}
	//
	//else
	//{
	//	weapons_.push_back(new Weapon::Rifle(fireWeap_, newDef));
	//}
	//
	//newWeap = (*--weapons_.end());
	//added->arm(newWeap);

	audio_.playSFX("spawn", B2toSF(pos, true));

	enemies++;
}

void GameWorld::addEnemy(const ShapeDef & def, Weapon::WeaponI * weapon)
{
	shapes_.push_back(new Enemy(addDynamicBody(def.position), def, addSide_, getControlled_));
	
	//p. sure this is broke
	if (weapon != nullptr)
	{
		armShape((*--shapes_.end()), weapon);
	}
}

//Adds a projectile to the world via definition
void GameWorld::addProjectile(const ProjectileDef& def)
{
	projectiles_.push_back(new Projectile(addBulletBody(def.origin), def));
}

//Adds a side to game world via definition
void GameWorld::addSide(const SideDef& def)
{
	sides_.push_back(new Side(addDynamicBody(def.position), def));
	
	audio_.playSFX("drop", B2toSF(def.position, true));

	freesides++;
}

void GameWorld::addPickup(Pickup::Type type, b2Vec2 position, int time)
{
	switch (type)
	{
	case Pickup::Type::SIGHT:
		pickups_.push_back(new Pickup::Sight(addDynamicBody(position), time));
		break;

	case Pickup::Type::SHIELD:
		pickups_.push_back(new Pickup::Shield(addDynamicBody(position), time));
		break;

	case Pickup::Type::ATTRACT:
		pickups_.push_back(new Pickup::Attractor(addDynamicBody(position), time));
		break;

	case Pickup::Type::WEAPON:
		break;
	}

	//audio_.playSFX(of some sort)
}

void GameWorld::addShrapnel(Projectile* src)
{
	std::pair<int, int> shrapnel = src->getShrapnel();
	std::pair<float, float> explosion = src->getForce();
	b2Vec2 pos = src->getPosition();


	if (shrapnel.first > 0)
	{
		b2Vec2 pos = src->getPosition();
		b2Vec2 dir(0, 0);
		Entity* owner = src->getOwner();

		ProjectileDef shrapDef = armory_->getShrapnel(shrapnel.second);
		shrapDef.colPrim = src->getSecondary();
		shrapDef.colSecn = src->getSecondary();
		shrapDef.colTert = src->getPrimary();
		shrapDef.damage = src->getDamage();
		
		for (int i = shrapnel.first; i > 0; --i)
		{
			dir.x = randFloat(-1, 1);
			dir.y = randFloat(-1, 1);
			dir.Normalize();

			shrapDef.origin = pos;
			shrapDef.heading = dir;
			shrapDef.owner = owner;
			//shrapDef.lifeTime = 50;

			addProjectile(shrapDef);
		}
	}

	audio_.playSFX("explosion", B2toSF(pos, true));
}

void GameWorld::addForce(b2Vec2 pos, float force, float radius, int lifetime)
{
	forces_.push_back(new Force(addStaticBody(pos), force, radius, lifetime));
}

void GameWorld::armShape(Shape * shape, Weapon::WeaponI * weapon)
{
	weapons_.push_back(weapon);
	shape->arm(weapon);
}

void GameWorld::disarmShape(Shape * shape)
{
	shape->disarm();
}

//Fire possibly many projectiles
void GameWorld::fireWeapon(std::vector<ProjectileDef>& defs, std::string id)
{
	for (std::vector<ProjectileDef>::iterator iter = defs.begin(), end = defs.end(); iter != end; ++iter)
	{
		addProjectile((*iter));
	}

	//Play sound at fired position
	audio_.playSFX(id, B2toSF(defs.begin()->origin, true));
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

		cleanPickups(player_);
		DestroyBody(player_->getBody());

		delete player_;
		player_ = nullptr;
	}
}

//Removes enemy from the world and increments iterator, for use within loops
void GameWorld::removeEnemy(Enemy* e)
{
	//Play death sound at position
	audio_.playSFX("die", B2toSF(e->getPosition(), true));

	//Delete enemy
	DestroyBody(e->getBody());
	cleanPickups(e);

	delete e;

	enemies--;
}

//Removes projectile from the world and increments iterator, for use within loops
void GameWorld::removeProjectile(Projectile* p)
{
	//Shrapnel if we need to
	if (p->getShrapnel().first != 0)
	{
		addShrapnel(p);
	}

	//If we need to explode
	std::pair<float, float> force = p->getForce();
	if (force.first != 0 && force.second != 0)
	{
		addForce(p->getPosition(), force.first, force.second, 50);
	}

	DestroyBody(p->getBody());
	delete p;
}

//Removes side from the world and increments iterator, for use within loops
void GameWorld::removeSide(Side* s)
{
	DestroyBody(s->getBody());
	delete s;

	freesides--;
}

//Removes weapon from list and increments iterator, for use in loops
void GameWorld::removeWeapon(Weapon::WeaponI* w)
{
	delete w;
}

void GameWorld::removePickup(Pickup::PickupI* p)
{
	DestroyBody(p->getBody());
	delete p;
}

void GameWorld::removeForce(Force* f)
{
	DestroyBody(f->getBody());
	delete f;
}

//Resets the level
void GameWorld::resetLevel()
{
	//Clear world
	clearWorld();
	timeInLevel_ = 0;
	lastSpawn_ = 0;
	spawnTime_ = 15000;

	bounds_->setSecondary(b2Color(0, 0, 0, 1));
	//Add a new player
	addPlayer(b2Vec2_zero, true);

	audio_.playSFX("spawn", B2toSF(b2Vec2_zero, true));
	spawns_ = 1;

	//Restart bgm
	//audio_.playAFX("spriterip");
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
		e != shapes_.end(); e = shapes_.erase(e))
		{
			removeEnemy((*e));
		}
	}
	shapes_.clear();

	if (projectiles_.empty() == false)
	{
		for (std::list<Projectile*>::iterator p = projectiles_.begin();
		p != projectiles_.end(); p = projectiles_.erase(p))
		{
			removeProjectile((*p));
		}
	}
	projectiles_.clear();

	if (sides_.empty() == false)
	{
		for (std::list<Side*>::iterator s = sides_.begin();
		s != sides_.end(); s = sides_.erase(s))
		{
			removeSide((*s));
			
		}
	}
	sides_.clear();

	if (weapons_.empty() == false)
	{
		for (std::list<Weapon::WeaponI*>::iterator w = weapons_.begin();
		w != weapons_.end(); w = weapons_.erase(w))
		{
			removeWeapon((*w));
		}
	}
	weapons_.clear();

	if (pickups_.empty() == false)
	{
		for (std::list<Pickup::PickupI*>::iterator p = pickups_.begin();
		p != pickups_.end(); p = pickups_.erase(p))
		{
			removePickup((*p));
		}
	}
	pickups_.clear();

	if (forces_.empty() == false)
	{
		for (std::list<Force*>::iterator f = forces_.begin();
		f != forces_.end(); f = forces_.erase(f))
		{
			removeForce((*f));
		}
	}
	forces_.clear();
}

int GameWorld::getHapticL() const
{
	return leftHaptic_;
}

int GameWorld::getHapticR() const
{
	return rightHaptic_;
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
std::list<Pickup::PickupI*>& GameWorld::getPickups() { return pickups_; }

//Update entity code and step the world
void GameWorld::update(int dt)
{
	if (!pause_)
	{
		updatePlayer(dt);
		updateEnemy(dt);
		updateProjectile(dt);
		updateSide(dt);
		updatePickup(dt);
		updateForce(dt);
		updateLevel(dt);

		Step(dt, VELOCITY_ITERS, POSITION_ITERS);
		
		//Sound
		if (player_->getCollected())
		{
			audio_.playSFX("collect", B2toSF(player_->getPosition(), true));
		}

		//Cleanup
		cullWeapons();

		//Spr3
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

		leftHaptic_ = 0;
		rightHaptic_ = rightHaptic_ - 5 >= 0 ? rightHaptic_ - 5 : 0;
	}
}

void GameWorld::updatePlayer(int dt)
{
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
}

void GameWorld::updateEnemy(int dt)
{
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
				removeEnemy(shp);
				shapeIt = shapes_.erase(shapeIt);
			}

			//Else just increment
			else ++shapeIt;
		}
	}
}

void GameWorld::updateProjectile(int dt)
{
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
				removeProjectile(prj);

				projIt = projectiles_.erase(projIt);
			}

			//Else just increment
			else ++projIt;
		}
	}
}

void GameWorld::updateSide(int dt)
{
	if (sides_.empty() == false)
	{
		for (std::list<Side*>::iterator sideIt = sides_.begin();
		sideIt != sides_.end(); /*Don't increment here*/)
		{
			//Pull pointer
			Side* sd = (*sideIt);

			popInside(sd);

			//If we're not active, increment by deleting
			if (sd->getActive() == false)
			{
				removeSide(sd);

				sideIt = sides_.erase(sideIt);
			}

			//Else just increment
			else ++sideIt;
		}
	}
}

void GameWorld::updatePickup(int dt)
{
	if (pickups_.empty() == false)
	{
		for (std::list<Pickup::PickupI*>::iterator pickIt = pickups_.begin();
		pickIt != pickups_.end(); /*Don't increment here*/)
		{
			//Pull pointer from it for readability
			Pickup::PickupI* pick = (*pickIt);

			pick->update(dt);
			
			//If we're a free pickup, keep us inside
			if (pick->getCollected() == false)
			{
				popInside(pick);
			}

			//If we're not owned and have been collected, we're done, increment by deleting
			if (pick->getCollected() == true && pick->getOwner() == nullptr)
			{
				removePickup(pick);

				pickIt = pickups_.erase(pickIt);
			}

			//Else just increment
			else ++pickIt;
		}
	}
}

void GameWorld::updateForce(int dt)
{
	if (forces_.empty() == false)
	{
		for (std::list<Force*>::iterator forceIt = forces_.begin();
		forceIt != forces_.end(); /*Don't increment here*/)
		{
			Force* force = (*forceIt);

			force->update(dt);

			if (force->getActive() == false)
			{
				removeForce(force);

				forceIt = forces_.erase(forceIt);
			}

			else ++forceIt;
		}

	}
}

void GameWorld::updateLevel(int dt)
{
	//Basic difficulty ramp
	timeInLevel_ += dt;
	timeInLevel_ % UINT16_MAX;

	//if (((timeInLevel_ - lastSpawn_) % UINT16_MAX) > spawnTime_ / 5)
	//{
	//	lastSpawn_ = timeInLevel_;
	//
	//	//if we want to do less than double the enemy number
	//	if (enemies < spawns_ * 2)
	//	{
	//		spawnEnemy();
	//		spawns_ = (spawns_ + 1 % UINT16_MAX > 10 ? 10 : spawns_ + 1 % UINT16_MAX);
	//	}
	//}
}

void GameWorld::cullWeapons()
{
	if (weapons_.empty() == false)
	{
		for (std::list<Weapon::WeaponI*>::iterator weapIt = weapons_.begin();
		weapIt != weapons_.end(); /*Don't increment here*/)
		{
			//Pull pointer
			Weapon::WeaponI* weap = (*weapIt);
			
			//If we're not active, increment by deleting
			if (weap->getOwner() == nullptr)
			{
				removeWeapon(weap);

				weapIt = weapons_.erase(weapIt);
			}

			//Else just increment
			else ++weapIt;
		}
	}
}

void GameWorld::cleanPickups(Shape* shape)
{
	if (pickups_.empty() == false)
	{
		for (std::list<Pickup::PickupI*>::iterator pickIt = pickups_.begin();
		pickIt != pickups_.end(); /*Don't increment here*/)
		{
			Pickup::PickupI* pick = (*pickIt);

			if (pick->getOwner() == shape)
			{
				pick->setOwner(nullptr);
			}

			++pickIt;
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

//Handlers for game intent: Move, Fire, Trigger, release, reload
void GameWorld::move(b2Vec2& direction)
{
	if (controlled_ != nullptr && (direction.x != 0 || direction.y != 0))
	{
		controlled_->move(direction);
		leftHaptic_ = direction.Length() * 5;
	}
}

void GameWorld::look(b2Vec2& direction)
{
	if (controlled_ != nullptr && (direction.x != 0 || direction.y != 0))
	{
		controlled_->orient(direction);
	}
}

void GameWorld::trigger(b2Vec2& direction)
{
	//If there's a direction to fire in
	if (controlled_ != nullptr)
	{
		bool canfire = controlled_->getWeaponReady();

		controlled_->trigger(direction);

		bool hasfire = controlled_->getWeaponReady();
		
		if (canfire && !hasfire)
		{
			rightHaptic_ = 50;
		}
	}
}

void GameWorld::release()
{
	if (controlled_ != nullptr)
	{
		controlled_->release();
	}
}

void GameWorld::reup()
{
	if (controlled_ != nullptr)
	{
		controlled_->reup();
	}
}

void GameWorld::bomb()
{
	if (player_ != nullptr)
	{
		if (player_->getBombReady())
		{
			float range = player_->getBombRange();
			//Explode shapes
			if (shapes_.empty() == false)
			{
				for (std::list<Enemy*>::iterator shapeIt = shapes_.begin();
				shapeIt != shapes_.end(); ++shapeIt)
				{
					if (b2Distance((*shapeIt)->getPosition(), player_->getPosition()) < range)
						(*shapeIt)->explode();
				}
			}

			//Baleet projectiles

			if (projectiles_.empty() == false)
			{
				for (std::list<Projectile*>::iterator projIt = projectiles_.begin();
				projIt != projectiles_.end(); ++projIt)
				{
					if (b2Distance((*projIt)->getPosition(), player_->getPosition()) < range)
						(*projIt)->setActive(false);
				}
			}

			audio_.playSFX("bomb", B2toSF(player_->getPosition(), true));
			player_->bomb();
		}
	}
}

void GameWorld::f1()
{
	//Testing projectile levels
	for (int i = 0; i < 8; ++i)
	{
		ShapeDef enem = ShapeDef(b2Vec2(-20 + 5 * i, 15), b2Vec2(0, -1), i);
		
		float e =  1 - i * (1 / 8.f);
		float s =  1 - i * (1 / 16.f);
		float t =  1 - i * (1 / 32.f);

		enem.colPrim = b2Color(e, e, e);
		enem.colSecn = b2Color(s, s, s);
		enem.colTert = b2Color(t, t, t);

		ProjectileDef newDef = armory_->getPellet(i);
		newDef.damage = 0;

		Enemy* added = new Enemy(addDynamicBody(enem.position), enem, addSide_, getControlled_);

		Weapon::WeaponI* newWeap;
		newWeap = armory_->getShotgun(i);
		Weapon::SpreadMag* shotz = static_cast<Weapon::SpreadMag*>(newWeap);
		shotz->setMagSize(10000);
		weapons_.push_back(newWeap);

		armShape(added, newWeap);
		shapes_.push_back(added);
	}
}

void GameWorld::f2()
{}

void GameWorld::f3()
{}

void GameWorld::f4()
{}

void GameWorld::f5()
{}

void GameWorld::f6()
{
	
}

void GameWorld::f7()
{
	static int x = -1;
	player_->disarm();
	x = ++x % 9;
	std::cout << x << std::endl;
	armShape(player_, armory_->getShotgun(x, x));

}

void GameWorld::f8()
{
	addPickup(Pickup::Type::SHIELD, b2Vec2(-10, 0), 20000);
	addPickup(Pickup::Type::SIGHT, b2Vec2(10, 0), 20000);
}

void GameWorld::f9()
{
	if (player_ != nullptr)
		randomiseCol(player_);
}

void GameWorld::f0()
{
	if (player_ != nullptr)
		player_->collect(999);
}

void GameWorld::testBed()
{
	//EW
	for (int i = 0; i < 8; ++i)
	{
		ShapeDef enem = ShapeDef(b2Vec2(-20 + 5 * i, 15), b2Vec2_zero, static_cast<int>(randFloat(3, 8) + 1));
		//ShapeDef enem = ShapeDef(b2Vec2(x, y), b2Vec2_zero, -1);

		ProjectileDef newDef = armory_->getLaser();
		Weapon::WeaponI* newWeap = new Weapon::AutoMag(fireWeap_, newDef);

		Weapon::AutoMag* riffle;
		switch (i)
		{
		case 0:
			newWeap = armory_->getShotgun(0);
			
			enem.colPrim = b2Color(.75f, .75f, .75f);
			enem.colSecn = b2Color(.5f, .5f, .5f);
			enem.colTert = b2Color(.75f, .75f, .75f);
			break;

		case 1:
			newDef = armory_->getNinMil();

			riffle = static_cast<Weapon::AutoMag*>(newWeap);
			riffle->setRefireTime(200);
			riffle->setReloadTime(2500);
			riffle->setMagSize(18);
			riffle->setID("pistol");

			enem.colPrim = b2Color(1, .5f, 0);
			enem.colSecn = b2Color(.75f, .25f, 0);
			enem.colTert = b2Color(1, .5f, 0);
			break;

		case 2:
			newDef = armory_->getBullet();
			enem.colPrim = b2Color(1, 1, 0);
			enem.colSecn = b2Color(.75f, .75f, 0);
			enem.colTert = b2Color(1, 1, 0);
			break;

		case 3:
			newDef = armory_->getDumDum();

			riffle = static_cast<Weapon::AutoMag*>(newWeap);
			riffle->setRefireTime(200);
			riffle->setReloadTime(2500);
			riffle->setMagSize(15);
			riffle->setID("magnum");

			enem.colPrim = b2Color(.5f, 0, .5f);
			enem.colSecn = b2Color(.25f, 0, .25f);
			enem.colTert = b2Color(.5f, 0, .5f);
			break;

		case 4:
			newDef = armory_->getCannonball();

			riffle = static_cast<Weapon::AutoMag*>(newWeap);
			riffle->setRefireTime(1000);
			riffle->setReloadTime(4000);
			riffle->setMagSize(8);
			riffle->setID("cannon");

			enem.colPrim = b2Color(0.2f, 0.2f, 0.2f);
			enem.colSecn = b2Color(.1f, .1f, .1f);
			enem.colTert = b2Color(0.2f, 0.2f, 0.2f);
			break;

		case 5:
			newDef = armory_->getGrenade();

			riffle = static_cast<Weapon::AutoMag*>(newWeap);
			riffle->setRefireTime(750);
			riffle->setReloadTime(4000);
			riffle->setMagSize(8);
			riffle->setID("grenade");

			enem.colPrim = b2Color(0, 0.75f, 0);
			enem.colSecn = b2Color(0, 0.5f, 0);
			enem.colTert = b2Color(0, 0.75f, 0);
			break;

		case 6:
			newDef = armory_->getRocket();

			riffle = static_cast<Weapon::AutoMag*>(newWeap);
			riffle->setRefireTime(1000);
			riffle->setReloadTime(4000);
			riffle->setMagSize(4);
			riffle->setID("rocket");

			enem.colPrim = b2Color(0.7f, 0, 0);
			enem.colSecn = b2Color(0.5f, 0, 0);
			enem.colTert = b2Color(0.7f, 0, 0);
			break;

		case 7:
			newDef = armory_->getLaser();

			riffle = static_cast<Weapon::AutoMag*>(newWeap);
			riffle->setID("coilgun");

			enem.colPrim = b2Color(0, 0, 1);
			enem.colSecn = b2Color(0, 0, .7f);
			enem.colTert = b2Color(0, 0, 1);
			break;
		}
		if (i <= 4)
		{
			enem.position = b2Vec2(-10 + 5 * i, -7.5);
			enem.heading = b2Vec2(0, -1);
		}

		else
		{
			enem.position = b2Vec2(-10+ 5 * (i - 4), 7.5);
			enem.heading = b2Vec2(0, 1);
		}


		newDef.damage = 0;

		shapes_.push_back(new Enemy(addDynamicBody(enem.position), enem, addSide_, getControlled_));
		Shape* added = *(--shapes_.end());

		newWeap->setProjectile(newDef);
		armShape(added, newWeap);
	}
	//randomiseCol(bounds_);
}
