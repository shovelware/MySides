#include "GameWorld.hpp"

//Constructor initialises Box2D World, boundaries, armory
GameWorld::GameWorld() :
	b2World(GRAVITY),
	contactListener_(ContactListener()),
	player_(nullptr),
	controlled_(nullptr),
	pause_(false),
	dstr("X"),
	di(-1)
{
	SetContactListener(&contactListener_);

	bounds_ = new Bounds(addStaticBody(b2Vec2_zero), 32);

	addProj_ = [this](ProjectileDef& def) { addProjectile(def); };
	addSide_ = [this](SideDef& def) { addSide(def); };
	fireWeap_ = [this](std::vector<ProjectileDef>& defs, std::string id) { fireWeapon(defs, id); };
	addForce_ = [this](b2Vec2 pos, float force, float radius, int time) { addForce(pos, force, radius, time); };
	
	boundsFriction_.localAnchorA = (b2Vec2(0, 0));
	boundsFriction_.localAnchorB = (b2Vec2(0, 0));
	boundsFriction_.bodyB = bounds_->getBody();
	boundsFriction_.maxForce = 0.0025f;
	boundsFriction_.maxTorque = 0.05f;
	boundsFriction_.collideConnected = false;

	//armory
	armory_ = new Weapon::Armory(fireWeap_);

	//audio_.addBGM("spriterip", "../assets/spriterip.ogg");
	audio_.addAFX("spriterip", "../assets/spriterip.ogg", 1, 0.2, 400, 1024);
	audio_.addAFX("wind", "../assets/wind.ogg", 0, 1, 750, 1000);

	//Weapon noises
	audio_.addSFX("shotgun", "../assets/nsnd/shotty.wav", 8);
	audio_.addSFX("pistol", "../assets/nsnd/sbullet.wav", 16);
	audio_.addSFX("rifle", "../assets/nsnd/bullet.wav", 16);
	audio_.addSFX("cannon", "../assets/nsnd/cannon.wav", 8);

	audio_.addSFX("coilgun", "../assets/nsnd/pew.wav", 16);
	audio_.addSFX("railgun", "../assets/nsnd/bwump.wav", 8);
	audio_.addSFX("werfer", "../assets/nsnd/brn.wav", 32);

	audio_.addSFX("launcher", "../assets/nsnd/rock.wav", 8);
	audio_.addSFX("thumper", "../assets/nsnd/gren.wav", 8);
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
	delete bounds_;
	delete armory_;
	delete player_;
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
		between *= 0.5f;
		
		ent->getBody()->ApplyForceToCenter(-between, true);
	}
}

//Adds a player to the world 
void GameWorld::addPlayer(const ShapeDef& def, Weapon::WeaponI* weapon)
{
	if (player_ != nullptr) 
		removePlayer();
	
	if (def.isValid())
	{
		b2Body* body = addDynamicBody(def.position);
		//Shapes have friction with bounds
		boundsFriction_.bodyA = body;
		CreateJoint(&boundsFriction_);
		boundsFriction_.bodyA = nullptr;

		player_ = new Player(body, def, addSide_);
		pickups_.push_back(new Pickup::Attractor(addDynamicBody(def.position), -1, 5.f, 3.f));
		pickups_.push_back(new Pickup::Attractor(addDynamicBody(def.position), -1, 15.f, 1.f));
		addPickup(Pickup::Type::SIGHT, def.position, -1, 32.f);
		
		audio_.playSFX("spawn", B2toSF(b2Vec2_zero, true));

		if (weapon != nullptr)
			armShape(player_, weapon);
	}

	controlled_ = player_;
}

void GameWorld::addEnemy(const ShapeDef& def, Weapon::WeaponI* weapon)
{
	if (def.isValid())
	{
		b2Body* body = addDynamicBody(def.position);

		//Shapes have friction with bounds
		boundsFriction_.bodyA = body;
		CreateJoint(&boundsFriction_);
		boundsFriction_.bodyA = nullptr;

		Enemy* enem = new Enemy(body, def, addSide_, getControlled_);

		//p. sure this is broke
		if (weapon != nullptr)
			armShape(enem, weapon);

		shapes_.push_back(enem);

		enemies++;
	}
}

//Adds a projectile to the world via definition
void GameWorld::addProjectile(const ProjectileDef& def)
{
	projectiles_.push_back(new Projectile(addBulletBody(def.origin), def, addForce_));
}

//Adds a side to game world via definition
void GameWorld::addSide(const SideDef& def)
{
	b2Body* body = addDynamicBody(def.position);

	sides_.push_back(new Side(body, def));
	
	audio_.playSFX("drop", B2toSF(def.position, true));

	freesides++;
}

void GameWorld::addPickup(Pickup::Type type, b2Vec2 position, int time, float strength)
{
	switch (type)
	{
	case Pickup::Type::SIGHT:
		pickups_.push_back(new Pickup::Sight(addDynamicBody(position), time, strength));
		break;

	case Pickup::Type::SHIELD:
		pickups_.push_back(new Pickup::Shield(addDynamicBody(position), time, strength));
		break;

	case Pickup::Type::ATTRACT:
		pickups_.push_back(new Pickup::Attractor(addDynamicBody(position), time, strength));
		break;

	case Pickup::Type::WEAPON:
		break;
	}

	//audio_.playSFX(of some sort)
}

void GameWorld::addShrapnel(Projectile* src)
{
	if (src->getShrapnel().shards > 0)
	{
		b2Vec2 pos = src->getPosition();
		b2Vec2 dir(0, 0);
		Entity* owner = src->getOwner();

		ProjectileDef shrapDef = armory_->getShrapnel(src->getShrapnel().level);
		shrapDef.colPrim = src->getSecondary();
		shrapDef.colSecn = src->getSecondary();
		shrapDef.colTert = src->getPrimary();
		shrapDef.damage = src->getDamage();
		shrapDef.faction = src->getFaction();
		
		for (int i = src->getShrapnel().shards; i > 0; --i)
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

		audio_.playSFX("explosion", B2toSF(pos, true));
	}

}

void GameWorld::addForce(b2Vec2 pos, float force, float radius, int lifetime)
{
		forces_.push_back(new Force(addStaticBody(pos), force, radius, lifetime));
}

//Spawn a random enemy
void GameWorld::spawnEnemy()
{
	std::string c[9];
	c[0] = "pistol";
	c[1] = "rifle";
	c[2] = "shotgun";
	c[3] = "coilgun";
	c[4] = "cannon";
	c[5] = "werfer";
	c[6] = "railgun";
	c[7] = "thumper";
	c[8] = "launcher";

	for (int i = 0; i < spawns_; ++i)
	{
		b2Vec2 pos;
		float minRad = getBoundsRadius() * 0.5f;
		float rad = getBoundsRadius() * 0.9f;
		pos.y = -(cos((2 * M_PI) * 32 / randFloat(0, 32)));
		pos.x = -(sin((2 * M_PI) * 32 / randFloat(0, 32)));

		pos *= randFloat(minRad, rad);

		ShapeDef enem = ShapeDef(pos, pos, randFloat(3, 6));
		enem.hpScale = randFloat(2, 5);
		enem.speedScale = 0.5f;
		enem.size = randFloat(0.5f, 1.5f);
		enem.upgrade = false;
		enem.ai = 2;
		enem.colPrim = b2Color(randFloat(0.f, 1.f), randFloat(0.f, 1.f), randFloat(0.f, 1.f));
		enem.colSecn = b2Color(randFloat(0.f, 1.f), randFloat(0.f, 1.f), randFloat(0.f, 1.f));
		enem.colTert = b2Color(randFloat(0.f, 1.f), randFloat(0.f, 1.f), randFloat(0.f, 1.f));
		enem.faction = 2;

		Weapon::WeaponI* weap = armory_->requisition(c[(int)std::floor(randFloat(0, 4))], (int)std::floor(randFloat(0, 4)));

		addEnemy(enem, weap);
	}
}

bool GameWorld::requisition(Shape * shape, std::string weaponName, int weaponCode)
{
	if (shape != nullptr)
	{
		Weapon::WeaponI* newWeap = armory_->requisition(weaponName, weaponCode);
		
		if (newWeap != nullptr)
		{
			armShape(shape, newWeap);
			return true;
		}
	}

	return false;
}

void GameWorld::armShape(Shape* shape, Weapon::WeaponI * weapon)
{
	if (weapon != nullptr)
	{
		weapons_.push_back(weapon);
		shape->arm(weapon);
	}
}

void GameWorld::disarmShape(Shape* shape)
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
		//Play loss sound at position if we're dead
		if (player_->getAlive() == false)
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
	if (p->getShrapnel().shards != 0)
	{
		addShrapnel(p);
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
	ShapeDef play = ShapeDef(b2Vec2_zero, b2Vec2(0, -1), 6);
	play.colPrim = b2Color(0.6f, 0.3f, 0.9f);
	play.colSecn = b2Color(0.f, 1.f, 1.f);
	play.colTert = b2Color(1.f, 0.f, 0.f);
	play.faction = 1;
	play.hpScale = 10;
	addPlayer(play);

	spawns_ = 0;

	//Restart bgm
	//audio_.playAFX("spriterip");
	audio_.playAFX("wind");

	//Regenerate level somehow
	hiSides = 0;
	enemies = 0;
	freesides = 0;
}

void GameWorld::clearWorld(bool clearPlayer)
{
	if (clearPlayer)
	{
		removePlayer();
	}

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

	cullWeapons();
	weapons_.clear();

	if (pickups_.empty() == false)
	{
		for (std::list<Pickup::PickupI*>::iterator p = pickups_.begin();
		p != pickups_.end(); )
		{
			if (!clearPlayer && (*p)->getOwner() == player_)
			{
				++p;
				continue;
			}

			removePickup((*p)); 
			p = pickups_.erase(p);
		}
	}
	if (clearPlayer) pickups_.clear();

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

void GameWorld::step(int dt)
{
	Step(dt, VELOCITY_ITERS, POSITION_ITERS);
}

//Update entity code and step the world
void GameWorld::update(int dt)
{
	if (!pause_)
	{
		//step(dt);
		updatePlayer(dt);
		updateEnemy(dt);
		updateProjectile(dt);
		updateSide(dt);
		updatePickup(dt);
		updateForce(dt);
		updateLevel(dt);

		
		//Sound
		if (player_ != nullptr && player_->getCollected())
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

		audio_.setListener(B2toSF(player_->getPosition(), true));

		////player_->setActive(true);//Debug invincible players

		//If we're not active
		if (player_->getActive() == false)
		{
			removePlayer();
		}

		//Take physics effects
		else popInside(player_);
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

			//If we're not active, increment by deleting
			if (shp->getActive() == false)
			{
				removeEnemy(shp);
				shapeIt = shapes_.erase(shapeIt);
			}

			//Else take physics effects, increment
			else {
				popInside(shp);
				++shapeIt;
			}
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

			bool inside = prj->getPosition().Length() < bounds_->getRadius();

			//If we're not active, or outside the bounds increment by deleting
			if (prj->getActive() == false || !inside)
			{
				removeProjectile(prj);

				projIt = projectiles_.erase(projIt);
			}

			//Else take physics effects, increment
			else {
				//popInside(prj); 
				++projIt;
			}
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

			sd->update(dt);

			//If we're not active, increment by deleting
			if (sd->getActive() == false)
			{
				removeSide(sd);

				sideIt = sides_.erase(sideIt);
			}

			//Else take physics effects, increment
			else {
				popInside(sd); ++sideIt;
			}
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
	if (spawns_ > 0)
	{
		if (((timeInLevel_ - lastSpawn_) % UINT16_MAX) > spawnTime_)
		{
			lastSpawn_ = timeInLevel_;

			//if we want to do less than double the enemy number
			if (enemies < spawns_ * 2)
			{
				spawnEnemy();
				spawns_ = (spawns_ + 1 % UINT16_MAX > 10 ? 10 : spawns_ + 1 % UINT16_MAX);
			}
		}
	}
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

void GameWorld::bomb(bool nuke)
{
	if (!nuke && player_ != nullptr)
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

	else if (nuke = true)
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

		audio_.playSFX("bomb", B2toSF(player_->getPosition(), true));
	}
}

void GameWorld::f1()
{
	//Testing Weapon levels
	int nom = 0;
	for (char c : dstr)
	{
		char x = c;
		nom += x;
	}

	nom = nom % 255;
	//std::cout << nom / 255.f << std::endl;
	for (int i = 0; i < 8; ++i)
	{
		ShapeDef enem = ShapeDef(b2Vec2(-20 + 5 * i, 15), b2Vec2(0, -1), std::max(i, 3));
		

		float e = (nom / 255.f) - i * 3 * (nom / 255);
		float s = 1 - i * (nom / 255.f);
		float t = (nom / 255.f) - i * 0.5f * (nom / 255);

		enem.colPrim = b2Color(e, 0.5, e);
		enem.colSecn = b2Color(s, s, 0);
		enem.colTert = b2Color(1, 0, t);
		enem.ai = 1;
		
		std::string wn = dstr;

		Weapon::WeaponI* newWeap = armory_->requisition(wn, i);

		addEnemy(enem, newWeap);

	}
}

void GameWorld::f2()
{
	for (int i = 1; i < 7; ++i)
	{
		for (int j = 1; j < 7; ++j)
		{
			b2Vec2 pos(-17.5 + j * 5,  2 - i * pow(1.25, i));
			ShapeDef e(pos, b2Vec2_zero + pos, i + 2);
			e.size = 0.5 * i;
			e.hpScale = 5 * (j -1);
			e.colPrim = b2Color(0.1 * i, 0.05 * j, 0.4);
			e.colSecn = b2Color(0.7, 0.4 + (0.05 * i), 0.9 - (0.1 * j));
			e.colTert = b2Color(1 - (0.05 * (i + j)), 0.7, 0.025 * (i * j));
			e.ai = 2;

			addEnemy(e);
		}
	}
}

void GameWorld::f3()
{
	//if (player_ != nullptr)
	//	addPickup(Pickup::Type::ATTRACT, player_->getPosition(), 5000, 64.f);


	ProjectileDef boom = ProjectileDef();

	boom.damage = 80.f;
	boom.hpMAX = 4;
	boom.lifeTime = 5000;
	boom.height = 1.f;
	boom.width = 1.f;
	boom.penetration = 8;
	boom.target = player_;

	Weapon::WeaponI* newWeap = new Weapon::SemiBat(fireWeap_, boom, "coilgun", 1280, 32, 64);

	if (player_ != nullptr)
	{
		armShape(player_, newWeap);
	}
}

void GameWorld::f4()
{
	if (spawns_ <= 0)
	{
		spawns_ = 1;
		spawnEnemy();
	}

	else spawns_ = 0;
}

void GameWorld::f5()
{
	if (bounds_ != nullptr)
		randomiseCol(bounds_);
}

void GameWorld::f6()
{
	if (player_ != nullptr)
		randomiseCol(player_);
}

void GameWorld::f7()
{
	//EW
	for (int max = 9, i = 0; i < max; ++i)
	{
		ShapeDef enem = ShapeDef(b2Vec2(-20 + 5 * i, 15), b2Vec2_zero, static_cast<int>(randFloat(3, 8) + 1));
		//ShapeDef enem = ShapeDef(b2Vec2(x, y), b2Vec2_zero, -1);

		ProjectileDef newDef = ProjectileDef();
		Weapon::WeaponI* newWeap = nullptr;

		switch (i)
		{
		case 0:
			newWeap = armory_->getShotgun();
			newDef = armory_->getPellet();

			enem.colPrim = b2Color(.75f, .75f, .75f);
			enem.colSecn = b2Color(.5f, .5f, .5f);
			enem.colTert = b2Color(.75f, .75f, .75f);
			break;

		case 1:
			newWeap = armory_->getPistol();
			newDef = armory_->getNinMil();

			enem.colPrim = b2Color(1, .5f, 0);
			enem.colSecn = b2Color(.75f, .25f, 0);
			enem.colTert = b2Color(1, .5f, 0);
			break;

		case 2:
			newWeap = armory_->getRifle();
			newDef = armory_->getBullet();

			enem.colPrim = b2Color(1, 1, 0);
			enem.colSecn = b2Color(.75f, .75f, 0);
			enem.colTert = b2Color(1, 1, 0);
			break;

		case 3:
			newWeap = armory_->getCannon();
			newDef = armory_->getCannonball();

			enem.colPrim = b2Color(0.2f, 0.2f, 0.2f);
			enem.colSecn = b2Color(.1f, .1f, .1f);
			enem.colTert = b2Color(0.2f, 0.2f, 0.2f);
			break;

		case 4:
			newWeap = armory_->getThumper();
			newDef = armory_->getGrenade();

			enem.colPrim = b2Color(0, 0.75f, 0);
			enem.colSecn = b2Color(0, 0.5f, 0);
			enem.colTert = b2Color(0, 0.75f, 0);
			break;

		case 5:
			newWeap = armory_->getLauncher();
			newDef = armory_->getRocket();

			enem.colPrim = b2Color(0.7f, 0, 0);
			enem.colSecn = b2Color(0.5f, 0, 0);
			enem.colTert = b2Color(0.7f, 0, 0);
			break;

		case 6:
			newWeap = armory_->getCoilgun();
			newDef = armory_->getLaser();

			enem.colPrim = b2Color(0, 0, 1);
			enem.colSecn = b2Color(0, 0, .7f);
			enem.colTert = b2Color(0, 0, 1);
			break;

		case 7:
			newWeap = armory_->getRailgun();
			newDef = armory_->getSlug();

			enem.colPrim = b2Color(0, 1.f, 1.f);
			enem.colSecn = b2Color(0.7f, 0.7f, 0.7f);
			enem.colTert = b2Color(0.7f, 0.8f, 0.1f);
			break;

		case 8:
			newWeap = armory_->getWerfer();
			newDef = armory_->getFlammen();

			enem.colPrim = b2Color(0.8f, 0.5f, 0.1f);
			enem.colSecn = b2Color(0.7f, 0.8f, 0.1f);
			enem.colTert = b2Color(0.2f, 0, 0);
		}


		float x, y;

		y = -(cos((2 * M_PI) * i / max)) * 5;
		x = -(sin((2 * M_PI) * i / max)) * 5;

		enem.position = (b2Vec2(x, y));
		enem.heading = (enem.position);


		newDef.damage = 0;

		Enemy* enemy = new Enemy(addDynamicBody(enem.position), enem, addSide_, getControlled_);


		if (newWeap != nullptr)
		{
			newWeap->setProjectile(newDef);
		}
		armShape(enemy, newWeap);

		shapes_.push_back(enemy);
	}

}

void GameWorld::f8()
{
	addPickup(Pickup::Type::SHIELD, b2Vec2(-10, 0), 20000);
	addPickup(Pickup::Type::SIGHT, b2Vec2(10, 0), 20000);
}

void GameWorld::f9()
{
	b2Vec2 centre (0, 0);
	b2Vec2 pos(0, 0);
	float radius = 20.f;
	ShapeDef def;
	int groups = 8;
	int shapes = 10;

	//for 4 sizes
	for (float i = 1; i <= groups; ++i)
	{
		centre.y = radius * (cos((M_PI * 2) / groups * i));
		centre.x = radius * (-sin((M_PI * 2) / groups * i));

		//for 5 shape sizes
		for (float j = 3, max = shapes + 2; j <= max; ++j)
		{
			float ang = atan2f(centre.y, centre.x);
			pos.y = (radius / 4.f) * (cos((M_PI * 2)/ max * j));
			pos.x = (radius / 4.f) * (-sin((M_PI * 2)/ max * j));
			
			def.position = centre + pos;
			def.heading =  pos + centre;
			def.size = i * 0.25f;

			def.vertices = (j < shapes ? j : 3 + (int)j % shapes);
			def.hpScale = 5 * i;
			def.colPrim = b2Color(0.2f * i, 0.1f * j, 1.f - j / 10, 1 - (0.1 * i));
			def.colSecn = b2Color(0.2f * j, 0.1f * (i / j), 0.05f * (i + j), 1 - (0.1 * i));
			def.colTert = b2Color(0.1f * i, 0.3f * i, 1.f * (j - i), 1 - (0.1 * i));
			def.ai = 1;
			

			addEnemy(def);
		}
	}
}

void GameWorld::f0()
{
	float x, y, rad = getBoundsRadius() * 0.7f;
	y = -(cos((2 * M_PI) * 64 / randFloat(0, 64)));
	x = -(sin((2 * M_PI) * 64 / randFloat(0, 64)));

	b2Vec2 pos(x, y);
	pos.Normalize();
	pos *= randFloat(10, rad);

	if (player_ != nullptr)
	{
		pos = player_->getOrientation();
		pos *= player_->getSize() * 1.1f;
		pos += player_->getPosition();
	}

	ShapeDef enem = ShapeDef(pos, b2Vec2_zero, static_cast<int>(randFloat(3, 8) + 1));
	//ShapeDef enem = ShapeDef(b2Vec2(x, y), b2Vec2_zero, -1);
	enem.size = .5f;
	enem.speedScale = .5f;
	enem.hpScale = 5;
	enem.colPrim = b2Color(randFloat(0.9f, 1.f), randFloat(0.f, 1.f), 0.f);
	enem.colSecn = b2Color(randFloat(0.6f, 1.f), randFloat(0.6f, 1.f), 0.f);
	enem.colTert = b2Color(randFloat(0.5f, 1.f), randFloat(0.1f, 0.3f), randFloat(0.1f, 0.3f));

	addEnemy(enem);
}

void GameWorld::testBed()
{
}
