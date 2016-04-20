#include "GameWorld.hpp"

//Constructor initialises Box2D World, boundaries, armory
GameWorld::GameWorld() :
	b2World(GRAVITY),
	contactListener_(ContactListener()),
	player_(nullptr),
	controlled_(nullptr),
	pause_(false),
	worldLevel_(nullptr),
	menuLevel_(nullptr),
	menu_(true),
	transitionTime_(0),
	transitionTimeMAX_(1000),
	dstr("X"),
	di(-1)
{
	SetContactListener(&contactListener_);

	bounds_ = new Bounds(addStaticBody(b2Vec2_zero), 32);

	addProj_ = [this](ProjectileDef& def) { addProjectile(def); };
	addSide_ = [this](SideDef& def) { addSide(def); };
	fireWeap_ = [this](std::vector<ProjectileDef>& defs, std::string id) { fireWeapon(defs, id); };
	addForce_ = [this](b2Vec2 pos, float force, float radius, int time, int faction) { addForce(pos, force, radius, time, faction); };
	
	boundsFriction_.localAnchorA = (b2Vec2(0, 0));
	boundsFriction_.localAnchorB = (b2Vec2(0, 0));
	boundsFriction_.bodyB = bounds_->getBody();
	boundsFriction_.maxForce = 0.003f;
	boundsFriction_.maxTorque = 0.05f;
	boundsFriction_.collideConnected = true;

	//armory
	armory_ = new Weapon::Armory(fireWeap_);


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

	//Other noises

	//Bomb
	audio_.addSFX("bomb", "../assets/nsnd/boom.wav", 2);

	//Oldies
	audio_.addSFX("spawn", "../assets/spawn.wav", 8);
	audio_.addSFX("die", "../assets/die.wav", 8);
	audio_.addSFX("loss", "../assets/loss.wav", 1);
	audio_.addSFX("drop", "../assets/drop.wav", 8);
	audio_.addSFX("collect", "../assets/collect.wav", 8);
	
	getControlled_ = std::bind(&GameWorld::getControlled, this);

	//Level
	populateLevelList();
	//selectedLevel_ = levels_.end();
	//loadMenu();

	menu_ = false;
	selectedLevel_ = --levels_.end();
	loadLevel(*selectedLevel_);
}

GameWorld::~GameWorld()
{
	clearWorld();
	unloadLevel();
	clearLevelList();
	delete menuLevel_;
	delete bounds_;
	delete armory_;
	delete player_;
}

sf::Vector2f GameWorld::B2toSF(const b2Vec2& vec, bool scale) const
{
	return sf::Vector2f(vec.x * (scale ? _SCALE_ : 1.f), vec.y * (scale ? _SCALE_ : 1.f));
}

#pragma region Box2D & Entity Control
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

//Adds a player to the world 
void GameWorld::addPlayer(const PlayerDef& def)
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
		
		audio_.playSFX("spawn", B2toSF(b2Vec2_zero, true));

		Weapon::WeaponI* weapon = armory_->requisition(def.weapon, def.weaponLevel);
		if (weapon != nullptr)
			armShape(player_, weapon);
	}

	controlled_ = player_;
}

void GameWorld::addEnemy(const EnemyDef& def)
{
	if (def.isValid())
	{
		b2Body* body = addDynamicBody(def.position);

		//Shapes have friction with bounds
		boundsFriction_.bodyA = body;
		CreateJoint(&boundsFriction_);
		boundsFriction_.bodyA = nullptr;

		Enemy* enem = new Enemy(body, def, addSide_, getControlled_);

		Weapon::WeaponI* weapon = armory_->requisition(def.weapon, def.weaponLevel);
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
	//No sides in menu
	if (menu_) return;

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

void GameWorld::addForce(b2Vec2 pos, float force, float radius, int lifetime, int faction)
{
	if (radius > 0 && lifetime > 0)
	{
		Force * f =	new Force(addStaticBody(pos), force, radius, lifetime, faction);
		float col = randFloat(0.f, 0.25f);
		float col2 = col * 2;
		float col3 = col * 3;
		float col4 = col * 4;

		f->setPrimary(b2Color(col2, col2, col2));
		f->setSecondary(b2Color(col3, col3, col3));
		if (force < 0)
			f->setTertiary(b2Color(col4, col4, col4));
		else
			f->setTertiary(b2Color(col, col, col));

		forces_.push_back(f);
	}
}

void GameWorld::addForce(const ForceDef & def)
{
	if (def.isValid())
	{
		forces_.push_back(new Force(addStaticBody(def.pos), def));
	}
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

	if (menu_)
	{
		menuLevel_->updateSelection(e->getFaction());
	}

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
#pragma endregion

void GameWorld::clearEnemies()
{
	if (shapes_.empty() == false)
	{
		for (std::list<Enemy*>::iterator e = shapes_.begin();
		e != shapes_.end(); e = shapes_.erase(e))
		{
			removeEnemy((*e));
		}
	}
	shapes_.clear();
}

void GameWorld::clearWorld(bool clearPlayer)
{
	if (clearPlayer)
	{
		removePlayer();
	}

	clearEnemies();

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

void GameWorld::populateLevelList()
{
	levels_.push_back(Level::Atlas::testQueue());
	levels_.push_back(Level::Atlas::testWeapon());
	levels_.push_back(Level::Atlas::testSize());
	levels_.push_back(Level::Atlas::testLayer());
	levels_.push_back(Level::Atlas::testSurv());
	levels_.push_back(Level::Atlas::testComplete());
	levels_.push_back(Level::Atlas::testAI());
	levels_.push_back(Level::Atlas::testSteer());

	//Menu Level
	{
		PlayerDef play(Level::Atlas::basePlayer());
		play.weapon = "fungun";
		play.weaponLevel = 999;
		play.bombTime = 1000;
		play.bombRadius = 64.f;

		menuLevel_ = new Level::Menu("testmenu", play, levels_.size());
		float radius = 32;

		menuLevel_->addAFX("../assets/wind.ogg", 0, 1, 1500, 2000);
		menuLevel_->setPrimary(b2Color(0.f, 0.f, 0.f));
		menuLevel_->setSecondary(b2Color(1.f, 1.f, 1.f));
		menuLevel_->setTertiary(b2Color(0.4f, 0.f, 0.4f));
		menuLevel_->setBoundsRadius(radius);
		menuLevel_->setBoundsPoints(32);

		if (!levels_.empty())
		{
			for (std::list<Level::LevelI*>::iterator iter = levels_.begin(), end = levels_.end(); iter != end; ++iter)
			{
				menuLevel_->addLevel(*(*iter));
			}
		}
	}
}

void GameWorld::clearLevelList()
{
	for (std::list<Level::LevelI*>::iterator iter = levels_.begin(), end = levels_.end(); iter != end; ++iter)
	{
		delete (*iter);
	}

	levels_.clear();
}

void GameWorld::loadMenu()
{
	if (worldLevel_ != nullptr && worldLevel_ != menuLevel_)
		unloadLevel();
	
	clearWorld();

	menuLevel_->reset();
	worldLevel_ = menuLevel_;
	
	//AFX
	std::queue<Level::AFXDef> afx(worldLevel_->getAFX());
	for (int i = 0, max = afx.size(); i < max; ++i)
	{
		Level::AFXDef& a = afx.front();
		audio_.addAFX(std::string("levelAFX" + std::to_string(i)), a.path, a.nearFactor, a.farFactor, a.nearDistance, a.farDistance);
		audio_.playAFX(std::string("levelAFX" + std::to_string(i)));
		afx.pop();
	}

	//Bounds
	bounds_->setPrimary(worldLevel_->getPrimary());
	bounds_->setSecondary(worldLevel_->getSecondary());
	bounds_->setTertiary(worldLevel_->getTertiary());
	bounds_->resize(worldLevel_->getBoundsRadius(), worldLevel_->getBoundsPoints());

	//Player
	PlayerDef p = worldLevel_->getPlayer();
	addPlayer(p);
	addPickup(Pickup::Type::SIGHT, p.position, -1, 32.f);

	hiSides = 0;
	enemies = 0;
	freesides = 0;

	//Start fade in	
	transitionDir_ = 1;
	transitionTime_ = transitionTimeMAX_;

	//Deselect
	selectedLevel_ = levels_.end();
}

void GameWorld::loadLevel(Level::LevelI* level)
{
	//Don't load menu through normal means
	if (level == menuLevel_) return;

	//unload current level
	if (worldLevel_ != nullptr)
		unloadLevel();

	clearWorld();

	worldLevel_ = level->clone();

	//AFX
	std::queue<Level::AFXDef> afx(worldLevel_->getAFX());
	for (int i = 0, max = afx.size(); i < max; ++i)
	{
		Level::AFXDef& a = afx.front();
		audio_.addAFX(std::string("levelAFX" + std::to_string(i)), a.path, a.nearFactor, a.farFactor, a.nearDistance, a.farDistance);
		audio_.playAFX(std::string("levelAFX" + std::to_string(i)));
		afx.pop();
	}

	//Bounds
	bounds_->setPrimary(worldLevel_->getPrimary());
	bounds_->setSecondary(worldLevel_->getSecondary());
	bounds_->setTertiary(worldLevel_->getTertiary());
	bounds_->resize(worldLevel_->getBoundsRadius(), worldLevel_->getBoundsPoints());

	//Player
	PlayerDef p = worldLevel_->getPlayer();
	addPlayer(p);

	hiSides = 0;
	enemies = 0;
	freesides = 0;
	
	//Start fade in	
	transitionDir_ = 1;
	transitionTime_ = transitionTimeMAX_;
}

void GameWorld::startLevel()
{
	if (!worldLevel_->getStarted())	worldLevel_->start();
	else worldLevel_->forceWave(); ////DEBUG-ISH
	
}

void GameWorld::resetLevel()
{
	//Reload current level
	if (!menu_)
		loadLevel(*selectedLevel_);
}

//Cleans up after our level afx
void GameWorld::unloadLevel()
{
	std::queue<Level::AFXDef> afx(worldLevel_->getAFX());
	for (int i = 0, max = afx.size(); i < max; ++i)
	{
		Level::AFXDef& a = afx.front();
		audio_.removeAFX(std::string("levelAFX" + std::to_string(i)));
		afx.pop();
	}

	if (worldLevel_ != menuLevel_)
		delete worldLevel_;
}

void GameWorld::selectLevel(std::string name)
{
	std::list<Level::LevelI*>::iterator find = std::find_if(levels_.begin(), levels_.end(), [name](Level::LevelI* lvl) { return lvl->getID() == name; });
	
	if (find != levels_.end())
	{
		selectedLevel_ = find;
	}
}

void GameWorld::selectLevel(int index)
{
	if (0 <= index && index < levels_.size())
	{
		selectedLevel_ = levels_.begin();

		for (int i = 0; i < index; ++i)
		{
			selectedLevel_++;
		}
	}
}

void GameWorld::menuToLevel()
{
	if (worldLevel_ == menuLevel_ && menu_)
	{
		worldLevel_->stop();
		menu_ = false;
		transitionDir_ = -1;
		selectLevel(menuLevel_->getSelection());
		if (transitionTime_ == 0) transitionTime_ = transitionTimeMAX_;
	}
}

void GameWorld::returnToMenu()
{
	if (worldLevel_ != menuLevel_ && !menu_)
	{
		worldLevel_->stop();
		menu_ = true;
		transitionDir_ = -1;
		transitionTime_ = transitionTimeMAX_;
	}
}

//Arms a shape with the weapon requisitioned, if the armory has it
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

int GameWorld::getHapticL() const {	return leftHaptic_; }
int GameWorld::getHapticR() const {	return rightHaptic_; }

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
		between *= 0.25f;

		if (ent == player_)
			between *= 0.5f;

		ent->getBody()->ApplyForceToCenter(-between, true);
	}
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

bool GameWorld::hasControlled() const { return (controlled_ != nullptr); }
Shape * GameWorld::getControlled() { return controlled_; }
bool GameWorld::inMenu() const { return worldLevel_ == menuLevel_; }

//Rendering helpers
Shape* GameWorld::getPlayer() { return player_; }
const Bounds& GameWorld::getBounds() { return *bounds_; }
const std::list<Enemy*>& GameWorld::getShapes() {	return shapes_; }
const std::list<Projectile*>& GameWorld::getProjectiles() { return projectiles_; }
const std::list<Side*>& GameWorld::getSides() { return sides_; }
const std::list<Pickup::PickupI*>& GameWorld::getPickups() { return pickups_; }
const std::list<Force*>& GameWorld::getForces() { return forces_; }
const Level::LevelI& GameWorld::getWorldLevel() {	return *worldLevel_; }

const Level::LevelI& GameWorld::getSelectedLevel() 
{ 
	if (selectedLevel_ != levels_.end()) return *(*selectedLevel_);
	else return *menuLevel_;
}

float GameWorld::getTransitionProgress()
{
	return (float)transitionDir_ * ((float)transitionTime_ / (float)transitionTimeMAX_);
}

void GameWorld::step(int dt)
{
	Step(dt, VELOCITY_ITERS, POSITION_ITERS);
}

//Update entity code and step the world
void GameWorld::update(int dt)
{
	if (!pause_)
	{
		if (transitionTime_ > 0)
		{
			//If there's time to take
			if (transitionTime_ - dt > 0)
			{
				transitionTime_ = transitionTime_ - dt;
			}

			//Else we're done, do loading
			else
			{
				transitionTime_ = 0;
				transitionDir_ = 0;

				if (!menu_ && worldLevel_ == menuLevel_)
				{
					loadLevel(*selectedLevel_);
				}

				else if (menu_ && worldLevel_ != menuLevel_)
				{
					loadMenu();
				}

				else worldLevel_->start();
			}
		}

		//step(dt);
		updatePlayer(dt);
		updateEnemy(dt);
		updateProjectile(dt);
		updateSide(dt);
		updatePickup(dt);
		updateForce(dt);

		if (worldLevel_ == menuLevel_) updateMenu(dt);
		else updateLevel(dt);
		
		//Sound
		if (player_ != nullptr && player_->getSidesCollected() > lastSides_)
		{
			lastSides_ = player_->getSidesCollected();
			audio_.playSFX("collect", B2toSF(player_->getPosition(), true));
		}

		//Cleanup
		cullWeapons();

		//Spr3
		if (hasControlled())
		{
			hiSides = controlled_->getSidesCollected();

			//Should a level decide to kill player on fail? PD:Z?
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

			bool inside = prj->getPosition().Length() < bounds_->getRadius() * 1.1f;

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

void GameWorld::updateMenu(int dt)
{
	bool player = (player_ != nullptr && player_->getAlive());
	int sides = player ? player_->getSidesCollected() : 0;

	menuLevel_->updateStatus(sides, enemies, player);
	menuLevel_->update(dt);

	//Start transition if we complete
	if (menuLevel_->getComplete() && transitionTime_ == 0)
	{
		menu_ = false;
		transitionDir_ = -1;
		transitionTime_ = transitionTimeMAX_;
	}

	//If there's a player and DEBUG and we're ready
	if (player && menuLevel_->getStarted() && menuLevel_->getWaveReady())
	{
		//Selection is detected in removeEnemy() if menu_ is true
		//CleareEnemies() calls removeEnemy(), so we can't have it overriding our selection
		menu_ = false;
		clearEnemies();
		menu_ = true;
		
		if (menuLevel_->getSelection() >= 0)
			selectLevel(menuLevel_->getSelection());

		//Respawn the menu
		Wave w = menuLevel_->getWave();
		std::vector<std::pair<EnemyDef, int>> wv = w.getWave();

		for (auto iter = wv.begin(), end = wv.end(); iter != end; ++iter)
		{
			for (int i = 0, max = iter->second; i < max; ++i)
			{
				addEnemy(iter->first);
			}
		}
	}
}

void GameWorld::updateLevel(int dt)
{
	bool player = (player_ != nullptr && player_->getAlive());
	int sides = player ? player_->getSidesCollected() : 0;

	worldLevel_->updateStatus(sides, enemies, player);
	worldLevel_->update(dt);
	
	//Start transition if we complete
	if (worldLevel_->getComplete() && transitionTime_ == 0)
	{
		returnToMenu();
	}

	//If there's a player and level is started and we're ready
	if (player && worldLevel_->getStarted() && worldLevel_->getWaveReady())
	{
		//Spawn a wave
		Wave w = worldLevel_->getWave();
		std::vector<std::pair<EnemyDef, int>> wv = w.getWave();

		for (auto iter = wv.begin(), end = wv.end(); iter != end; ++iter)
		{
			b2Vec2 pos = iter->first.position;

			for (int i = 0, max = iter->second; i < max; ++i)
			{
				if (iter->second > 1)
				{
					//angle = atan2(y, x);
					//length = sqrt(x * x + y * y);
					//angle += 1;
					//new_x = length * cos(angle);
					//new_y = length * sin(angle);

					b2Vec2 adjust(randFloat(-iter->first.size * iter->second, iter->first.size * iter->second), 
									randFloat(-iter->first.size * iter->second, iter->first.size * iter->second));
					adjust.Normalize();
					//float angle = atan2f(pos.y, pos.x);
					//float increm = ((M_PI * 2 / max) * i);
					//
					//pos.y = dist * ( cos(increm));
					//pos.x = dist * (-sin(increm));

					iter->first.position = pos + adjust;
				}

				addEnemy(iter->first);
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

//Pause control
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

//Handlers for game intent: Move, Fire, Trigger, release, reload
void GameWorld::move(b2Vec2& direction)
{
	if (controlled_ != nullptr)
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
	if (menu_ && selectedLevel_ == levels_.end());

	else if (!nuke && player_ != nullptr)
	{
		if (player_->getBombReady())
		{
			if (menu_)
			{
				menuLevel_->complete();
			}

			float range = player_->getBombRadius();
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

			ForceDef bombDef = ForceDef();
			bombDef.pos = player_->getPosition();
			bombDef.force = 0.2f;
			bombDef.radius = range;
			bombDef.faction = player_->getFaction();
			bombDef.primary = player_->getTertiary();
			bombDef.secondary = player_->getPrimary();
			bombDef.tertiary = player_->getSecondary();
			bombDef.lifeTime = 200;
			bombDef.shapes = true;
			bombDef.projectiles = true;
			bombDef.sides = false;
			bombDef.pickups = false;

			addForce(bombDef);

			audio_.playSFX("bomb", B2toSF(player_->getPosition(), true));
			player_->bomb();
		}
	}

	else if (nuke == true)
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

		if (player_ != nullptr)
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
		EnemyDef enem = ShapeDef(b2Vec2(-20 + 5 * i, 15), b2Vec2(0, -1), std::max(i, 3));
		

		float e = (nom / 255.f) - i * 3 * (nom / 255);
		float s = 1 - i * (nom / 255.f);
		float t = (nom / 255.f) - i * 0.5f * (nom / 255);

		enem.colPrim = b2Color(e, 0.5, e);
		enem.colSecn = b2Color(s, s, 0);
		enem.colTert = b2Color(1, 0, t);
		enem.ai = 1;
		enem.weapon = dstr;
		enem.weaponLevel = i;
		enem.faction = 2;

		addEnemy(enem);

	}
}

void GameWorld::f2()
{
	EnemyDef e = EnemyDef::triDef();
	b2Vec2 pos(0, 0);

	switch (di)
	{
	case 4:
		e = EnemyDef::squDef();
		break;
	case 5:
		e = EnemyDef::penDef();
		break;
	case 6:
		e = EnemyDef::hexDef();
		break;
	case 7:
		e = EnemyDef::hepDef();
		break;
	case 8:
		e = EnemyDef::octDef();
		break;
	}

	if (player_ != nullptr)
	{
		pos = player_->getOrientation();
		pos *= player_->getSize() * 1.1f;
		pos += player_->getPosition();
	}

	e.verticesMin = 3;
	e.ai = 2;
	e.speedScale = 0.5f;
	e.damageScale = 0.f;
	e.weapon = dstr;
	e.faction = 2;

	addEnemy(e);
}

void GameWorld::f3()
{
	EnemyDef e = EnemyDef::triDef();
	b2Vec2 pos(0, 0);

	switch (di)
	{
	case 4:
		e = EnemyDef::squDef();
		break;
	case 5:
		e = EnemyDef::penDef();
		break;
	case 6:
		e = EnemyDef::hexDef();
		break;
	case 7:
		e = EnemyDef::hepDef();
		break;
	case 8:
		e = EnemyDef::octDef();
		break;
	}

	if (player_ != nullptr)
	{
		pos = player_->getOrientation();
		pos *= player_->getSize() * 1.1f;
		pos += player_->getPosition();
	}

	e.verticesMin = 3;
	e.ai = 3;
	e.speedScale = 0.5f;
	e.damageScale = 0.f;
	e.weapon = dstr;
	e.faction = 2;

	addEnemy(e);
}

void GameWorld::f4()
{
	EnemyDef e = EnemyDef::triDef();
	b2Vec2 pos(0, 0);

	switch (di)
	{
	case 4:
		e = EnemyDef::squDef();
		break;
	case 5:
		e = EnemyDef::penDef();
		break;
	case 6:
		e = EnemyDef::hexDef();
		break;
	case 7:
		e = EnemyDef::hepDef();
		break;
	case 8:
		e = EnemyDef::octDef();
		break;
	}

	if (player_ != nullptr)
	{
		pos = player_->getOrientation();
		pos *= player_->getSize() * 1.1f;
		pos += player_->getPosition();
	}

	e.verticesMin = 3;
	e.ai = 4;
	e.speedScale = 0.5f;
	e.damageScale = 0.f;
	e.weapon = dstr;
	e.faction = 2;

	addEnemy(e);
}

void GameWorld::f5()
{
	EnemyDef e = EnemyDef::triDef();
	b2Vec2 pos(0, 0);

	switch (di)
	{
	case 4:
		e = EnemyDef::squDef();
		break;
	case 5:
		e = EnemyDef::penDef();
		break;
	case 6:
		e = EnemyDef::hexDef();
		break;
	case 7:
		e = EnemyDef::hepDef();
		break;
	case 8:
		e = EnemyDef::octDef();
		break;
	}

	if (player_ != nullptr)
	{
		pos = player_->getOrientation();
		pos *= player_->getSize() * 1.1f;
		pos += player_->getPosition();
	}

	e.verticesMin = 3;
	e.brain = AIDef::testDef();
	e.speedScale = 0.5f;
	e.damageScale = 0.f;
	e.weapon = dstr;
	e.faction = 2;

	addEnemy(e);
}

void GameWorld::f6()
{
	player_->kill();
}

void GameWorld::f7()
{
	addPickup(Pickup::Type::SHIELD, b2Vec2(-10, 0), 20000);
	addPickup(Pickup::Type::SIGHT, b2Vec2(10, 0), 20000);
}

void GameWorld::f8()
{
	if (player_ != nullptr)
		randomiseCol(player_);
}

void GameWorld::f9()
{
	if (bounds_ != nullptr)
		randomiseCol(bounds_);
}

void GameWorld::f0()
{
	float x, y, rad = bounds_->getRadius() * 0.7f;
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

	EnemyDef enem(ShapeDef(pos, b2Vec2_zero, static_cast<int>(randFloat(3, 8) + 1)));
	//ShapeDef enem = ShapeDef(b2Vec2(x, y), b2Vec2_zero, -1);
	
	enem.ai = 3;
	enem.size = .5f;
	enem.speedScale = .5f;
	enem.hpScale = 5;
	enem.size = randFloat(0.5f, 1.5f);
	//enem.colPrim = b2Color(randFloat(0.9f, 1.f), randFloat(0.f, 1.f), 0.f);
	//enem.colSecn = b2Color(randFloat(0.6f, 1.f), randFloat(0.6f, 1.f), 0.f);
	//enem.colTert = b2Color(randFloat(0.5f, 1.f), randFloat(0.1f, 0.3f), randFloat(0.1f, 0.3f));

	float vert = enem.vertices;

	enem.colPrim = b2Color(0.1f * vert,			0.5f - vert * 0.05f,		1.f - vert * 0.1f);
	enem.colSecn = b2Color(0.2f * vert,			0.1f * vert,				0.2f * vert);
	enem.colTert = b2Color(0.9f - vert * 0.1f,	1.f / vert,					1.f - vert * 0.1f);

	addEnemy(enem);
}
