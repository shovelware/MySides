#include "GameWorld.hpp"

//Constructor initialises Box2D World, boundaries, armory
GameWorld::GameWorld() :
	b2World(GRAVITY),
	contactListener_(ContactListener()),
	player_(nullptr),
	controlled_(nullptr),
	pause_(false),
	dstr("X"),
	di(-1),
	worldLevel_(new Level::WaveQueue("default", PlayerDef()))
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

	//Level
	populateLevelList();

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

	currentLevel_ = levels_.begin();
	loadLevel(*currentLevel_);
}

GameWorld::~GameWorld()
{
	clearWorld();
	clearLevelList();
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
#pragma endregion

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

void GameWorld::populateLevelList()
{
	//Player
	PlayerDef bplay = PlayerDef();
	bplay.vertices = 6;
	bplay.colPrim = b2Color(0.6f, 0.3f, 0.9f);
	bplay.colSecn = b2Color(0.f, 1.f, 1.f);
	bplay.colTert = b2Color(1.f, 0.f, 0.f);
	bplay.upgrade = true;
	bplay.faction = 1;
	bplay.hpScale = 10;
	
	//Test Queue level
	{
		PlayerDef play(bplay);
		play.weapon = "fungun";
		play.weaponLevel = 42;
		play.bombTime = 20000;
		play.bombRadius = 15.f;


		Level::WaveQueue* qlvl = new Level::WaveQueue("testqueue", play);
		float radius = 64.f;
		Wave wav = Wave();
		EnemyDef enem;

		//qlvl_->addAFX("../assets/spriterip.ogg", 1, 0.2, 400, 1024);
		qlvl->addAFX("../assets/wind.ogg", 0, 1, 1500, 2000);
		qlvl->setPrimary(b2Color(0.4f, 0.0f, 0.4f));
		qlvl->setSecondary(b2Color(0.1f, 0.1f, 0.1f));
		qlvl->setTertiary(b2Color(0.1f, 0.8f, 0.1f));
		qlvl->setBoundsRadius(radius);
		qlvl->setBoundsPoints(32);

		for (int s = 3; s <= 8; ++s)
		{
			wav = Wave();
			switch (s)
			{
			case 3:
				enem = EnemyDef::triDef();
				break;
			case 4:
				enem = EnemyDef::squDef();
				break;
			case 5:
				enem = EnemyDef::penDef();
				break;
			case 6:
				enem = EnemyDef::hexDef();
				break;
			case 7:
				enem = EnemyDef::hepDef();
				break;
			case 8:
				enem = EnemyDef::octDef();
				break;
			}

			for (int w = s * 2; w > 0; --w)
			{
				enem.position = b2Vec2(randFloat(-1, 1), randFloat(-1, 1));
				enem.position.Normalize();
				enem.position *= radius * 0.4;

				enem.hpScale *= 1;

				wav.addEnemy(enem);
			}

			qlvl->addWaveToQueue(wav);
		}

		levels_.push_back(qlvl);
	}

	//Test weapon level
	{
		PlayerDef play(bplay);
		play.weapon = "fungun";
		play.weaponLevel = 999;
		play.bombTime = 1000;
		play.bombRadius = 15.f;

		Level::WaveQueue* weaplvl = new Level::WaveQueue("testweap", play);
		float radius = 64.f;
		Wave wav = Wave();
		std::string weap = "";
		EnemyDef enem;
		float x, y;

		weaplvl->addAFX("../assets/wind.ogg", 0, 1, 1500, 2000);
		weaplvl->setPrimary(b2Color(0.6f, 0.6f, 0.6f));
		weaplvl->setSecondary(b2Color(0.1f, 0.1f, 0.1f));
		weaplvl->setTertiary(b2Color(0.9f, 0.9f, 0.1f));
		weaplvl->setRespiteTimeMAX(1000);
		weaplvl->setBoundsRadius(radius);
		weaplvl->setBoundsPoints(16);

		for (int l = 0; l < 9; ++l)
		{
			wav = Wave();
			for (int s = 0; s < 9; ++s)
			{
				enem = EnemyDef();
				enem.vertices = static_cast<int>(randFloat(3, 8) + 1);

				switch (s)
				{
				case 0:
					weap = "shotgun";
					enem.colPrim = b2Color(.75f, .75f, .75f);
					enem.colSecn = b2Color(.5f, .5f, .5f);
					enem.colTert = b2Color(.75f, .75f, .75f);
					break;

				case 1:
					weap = "pistol";
					enem.colPrim = b2Color(1, .5f, 0);
					enem.colSecn = b2Color(.75f, .25f, 0);
					enem.colTert = b2Color(1, .5f, 0);
					break;

				case 2:
					weap = "rifle";
					enem.colPrim = b2Color(1, 1, 0);
					enem.colSecn = b2Color(.75f, .75f, 0);
					enem.colTert = b2Color(1, 1, 0);
					break;

				case 3:
					weap = "cannon";
					enem.colPrim = b2Color(0.2f, 0.2f, 0.2f);
					enem.colSecn = b2Color(.1f, .1f, .1f);
					enem.colTert = b2Color(0.2f, 0.2f, 0.2f);
					break;

				case 4:
					weap = "thumper";
					enem.colPrim = b2Color(0, 0.75f, 0);
					enem.colSecn = b2Color(0, 0.5f, 0);
					enem.colTert = b2Color(0, 0.75f, 0);
					break;

				case 5:
					weap = "launcher";
					enem.colPrim = b2Color(0.7f, 0, 0);
					enem.colSecn = b2Color(0.5f, 0, 0);
					enem.colTert = b2Color(0.7f, 0, 0);
					break;

				case 6:
					weap = "coilgun";
					enem.colPrim = b2Color(0, 0, 1);
					enem.colSecn = b2Color(0, 0, .7f);
					enem.colTert = b2Color(0, 0, 1);
					break;

				case 7:
					weap = "railgun";
					enem.colPrim = b2Color(0, 1.f, 1.f);
					enem.colSecn = b2Color(0.7f, 0.7f, 0.7f);
					enem.colTert = b2Color(0.7f, 0.8f, 0.1f);
					break;

				case 8:
					weap = "werfer";
					enem.colPrim = b2Color(0.8f, 0.5f, 0.1f);
					enem.colSecn = b2Color(0.7f, 0.8f, 0.1f);
					enem.colTert = b2Color(0.2f, 0, 0);
				}

				y = -(cos((2 * M_PI) * s / 9)) * 5;
				x = -(sin((2 * M_PI) * s / 9)) * 5;

				enem.position = (b2Vec2(x, y));
				enem.heading = (enem.position);
				enem.ai = 1;
				enem.hpScale = l;
				enem.damageScale = 0;
				enem.weapon = weap;
				enem.weaponLevel = l;

				wav.addEnemy(enem);
			}

			weaplvl->addWaveToQueue(wav);
		}

		levels_.push_back(weaplvl);
	}

	//Layer level
	{
		PlayerDef play(bplay);
		play.weapon = "fungun";
		play.weaponLevel = 999;
		play.bombTime = 15000;
		play.bombRadius = 15.f;

		Level::WaveQueue* laylvl = new Level::WaveQueue("testlayer", play);
		Wave wav = Wave();
		float radius = 32;
		b2Vec2 centre(0, 0);
		b2Vec2 pos(0, 0);
		float spawnradius = 20.f;
		EnemyDef def;
		int groups = 10;
		int shapes = 6;

		int baseHPScale = 1.25;
		float baseSize = 0.25f;

		laylvl->addAFX("../assets/wind.ogg", 0, 1, 1500, 2000);
		laylvl->setPrimary(b2Color(0.9f, 0.6f, 0.4f));
		laylvl->setSecondary(b2Color(0.5f, 0.1f, 1.f));
		laylvl->setTertiary(b2Color(0.7f, 0.3f, 0.4f));
		laylvl->setRespiteTimeMAX(1000);
		laylvl->setBoundsRadius(radius);
		laylvl->setBoundsPoints(24);

		//for 4 sizes
		for (float i = 1; i <= groups; ++i)
		{
			centre.y = spawnradius * (cos((M_PI * 2) / groups * i));
			centre.x = spawnradius * (-sin((M_PI * 2) / groups * i));

			//for 5 shape sizes
			for (float j = 1, max = shapes; j <= max; ++j)
			{
				float ang = atan2f(centre.y, centre.x);
				pos.y = 1.f * (cos((M_PI * 2) / max * j));
				pos.x = 1.f * (-sin((M_PI * 2) / max * j));

				def.size = i * baseSize;
				def.heading = pos + centre;

				def.position = pos;
				def.position *= (def.size * 2);
				def.position += centre;

				def.vertices = 3;
				def.vertices += (((int)j - 1) % 6);

				def.hpScale = 5 * baseHPScale;
				def.colPrim = b2Color(0.2f * i, 0.1f * j, 1.f - j / 10);
				def.colSecn = b2Color(0.2f * j, 0.1f * (i / j), 0.05f * (i + j));
				def.colTert = b2Color(0.1f * i, 0.3f * i, 1.f * (j - i));
				def.ai = 2;

				wav.addEnemy(def);
			}
		}

		laylvl->addWaveToQueue(wav);
		levels_.push_back(laylvl);
	}

	//Size test level
	{
		PlayerDef play(bplay);
		play.weapon = "fungun";
		play.weaponLevel = 42;
		play.bombTime = 5000;
		play.bombRadius = 15.f;

		Level::WaveQueue* sizelvl = new Level::WaveQueue("testsize", play);
		Wave wav = Wave();
		float radius = 32;
		b2Vec2 pos(0, 0);
		EnemyDef def;

		sizelvl->addAFX("../assets/wind.ogg", 0, 1, 1500, 2000);
		sizelvl->setPrimary(b2Color(0.1f, 0.3f, 0.8f));
		sizelvl->setSecondary(b2Color(0.2f, 0.7f, 7.f));
		sizelvl->setTertiary(b2Color(0.7f, 0.5f, 0.1f));
		sizelvl->setRespiteTimeMAX(1000);
		sizelvl->setBoundsRadius(radius);
		sizelvl->setBoundsPoints(16);
		
		for (int i = 1; i < 7; ++i)
		{
			for (int j = 1; j < 7; ++j)
			{
				b2Vec2 pos(-17.5 + j * 5, 2 - i * pow(1.25, i));
				EnemyDef e(ShapeDef(pos, b2Vec2_zero + pos, i + 2));
				e.size = 0.5 * i;
				e.hpScale = 5 * (j - 1);
				e.colPrim = b2Color(0.1 * i, 0.05 * j, 0.4);
				e.colSecn = b2Color(0.7, 0.4 + (0.05 * i), 0.9 - (0.1 * j));
				e.colTert = b2Color(1 - (0.05 * (i + j)), 0.7, 0.025 * (i * j));
				e.ai = 2;

				wav.addEnemy(e);
			}

			sizelvl->addWaveToQueue(wav);
		}
		levels_.push_back(sizelvl);
	}

	//Test sloop level
}

void GameWorld::clearLevelList()
{
	for (std::list<Level::LevelI*>::iterator iter = levels_.begin(), end = levels_.end(); iter != end; ++iter)
	{
		delete (*iter);
	}

	levels_.clear();
}

void GameWorld::loadLevel(Level::LevelI* level)
{
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
}

void GameWorld::startLevel()
{
	worldLevel_->start();
}

void GameWorld::resetLevel()
{
	//Reload current level
	loadLevel(*currentLevel_);
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

	delete worldLevel_;
}

void GameWorld::selectLevel(std::string name)
{
	std::list<Level::LevelI*>::iterator find = std::find_if(levels_.begin(), levels_.end(), [name](Level::LevelI* lvl) { return lvl->getID() == name; });
	
	if (find != levels_.end())
	{
		currentLevel_ = find;
	}
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

	for (int i = 0; i < 4; ++i)
	{
		b2Vec2 pos;
		float minRad =  bounds_->getRadius() * 0.5f;
		float rad = bounds_->getRadius() * 0.9f;
		pos.y = -(cos((2 * M_PI) * 32 / randFloat(0, 32)));
		pos.x = -(sin((2 * M_PI) * 32 / randFloat(0, 32)));

		pos *= randFloat(minRad, rad);

		EnemyDef enem = ShapeDef(pos, pos, randFloat(3, 6));
		enem.hpScale = randFloat(2, 5);
		enem.speedScale = 0.5f;
		enem.size = randFloat(0.5f, 1.5f);
		enem.upgrade = false;
		enem.ai = 2;
		enem.colPrim = b2Color(randFloat(0.f, 1.f), randFloat(0.f, 1.f), randFloat(0.f, 1.f));
		enem.colSecn = b2Color(randFloat(0.f, 1.f), randFloat(0.f, 1.f), randFloat(0.f, 1.f));
		enem.colTert = b2Color(randFloat(0.f, 1.f), randFloat(0.f, 1.f), randFloat(0.f, 1.f));
		enem.faction = 2;

		enem.weapon = c[(int)std::floor(randFloat(0, 4))];
		enem.weaponLevel = (int)std::floor(randFloat(0, 4));

		addEnemy(enem);
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

bool GameWorld::hasControlled() { return (controlled_ != nullptr); }
Shape * GameWorld::getControlled() { return controlled_; }

Shape* GameWorld::getPlayer() { return player_; }
Bounds& const GameWorld::getBounds() { return *bounds_; }
std::list<Enemy*>& const GameWorld::getShapes() {	return shapes_; }
std::list<Projectile*>& const GameWorld::getProjectiles() { return projectiles_; }
std::list<Side*>& const GameWorld::getSides() { return sides_; }
std::list<Pickup::PickupI*>& const GameWorld::getPickups() { return pickups_; }
std::list<Force*>& const GameWorld::getForces() { return forces_; }
Level::LevelI& const GameWorld::getWorldLevel() {	return *worldLevel_; }
Level::LevelI & const GameWorld::getCurrentLevel() { return *(*currentLevel_); }

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
	bool player = (player_ != nullptr && player_->getAlive());
	int sides = player ? player_->getSidesCollected() : 0;

	worldLevel_->updateStatus(sides, enemies, player);
	worldLevel_->update(dt);

	//If there's a player and DEBUG and we're ready
	if (player && worldLevel_->getStarted() && worldLevel_->getWaveReady())
	{
		//Spawn a wave
		Wave w = worldLevel_->getWave();
		std::vector<EnemyDef> wv = w.getWave();

		for (auto iter = wv.begin(), end = wv.end(); iter != end; ++iter)
		{
			addEnemy(*iter);
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
	if (!nuke && player_ != nullptr)
	{
		if (player_->getBombReady())
		{
			float range = player_->getBombRadius();
			//Explode shapes
			if (shapes_.empty() == false)
			{
				for (std::list<Enemy*>::iterator shapeIt = shapes_.begin();
				shapeIt != shapes_.end(); ++shapeIt)
				{
					if (b2Distance((*shapeIt)->getPosition(), player_->getPosition()) < range)
						(*shapeIt)->setAlive(false);
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

			addForce(player_->getPosition(), 0.2f, range, 200);
			addForce(player_->getPosition(), -0.1f, range, 200);
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
				(*shapeIt)->setAlive(false);
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

		addEnemy(enem);

	}
}

void GameWorld::f2()
{
}

void GameWorld::f3()
{
	if (player_ != nullptr)
		addPickup(Pickup::Type::ATTRACT, player_->getPosition(), 5000, 64.f);
}

void GameWorld::f4()
{
	currentLevel_++;
	if (currentLevel_ == levels_.end())
		currentLevel_ = levels_.begin();
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
}

void GameWorld::f8()
{
	addPickup(Pickup::Type::SHIELD, b2Vec2(-10, 0), 20000);
	addPickup(Pickup::Type::SIGHT, b2Vec2(10, 0), 20000);
}

void GameWorld::f9()
{
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
