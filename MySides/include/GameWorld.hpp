// GameWorld.hpp
// Inherits from Box2d World, added functionality for game control

#ifndef MS_GAMEWORLD_HPP
#define MS_GAMEWORLD_HPP

#include "stdafx.h"

#include <Box2D\Box2D.h>
#include <SFML/Audio.hpp>
#include <SFMLDebugDraw.h>

#include <list>
#include <algorithm>
#include <functional>

#include "ContactListener.hpp"

#include "Entity.hpp"
#include "Bounds.hpp"
#include "Shape.hpp"
#include "Projectile.hpp"
#include "Side.hpp"
#include "Force.hpp"

#include "Player.hpp"
#include "Enemy.hpp"

#include "ShapeDef.hpp"
#include "SideDef.hpp"

#include "Pickup.hpp"
#include "Sight.hpp"
#include "Shield.hpp"
#include "Attractor.hpp"

#include "Armory.hpp"

#include "SoundSystem.hpp"

class GameWorld : protected b2World {
public:
	GameWorld();
	~GameWorld();

	Shape* getControlled();
	bool hasControlled();

	//Spawning works off definitions
	void addPlayer(const ShapeDef& def, Weapon::WeaponI* weapon = nullptr);
	void addEnemy(const ShapeDef& def, Weapon::WeaponI* weapon = nullptr);
	void addProjectile(const ProjectileDef& def);
	void addSide(const SideDef& def);
	void addPickup(Pickup::Type type, b2Vec2 position, int time, float strength = 0);
	void addShrapnel(Projectile* src);
	void addForce(b2Vec2 pos, float force, float radius, int time);

	//Called by updates
	void removePlayer();
	void removeEnemy(Enemy* e);
	void removeProjectile(Projectile* p);
	void removeSide(Side* s);
	void removeWeapon(Weapon::WeaponI* w);
	void removePickup(Pickup::PickupI* p);
	void removeForce(Force* f);

	//Weapon handling
	bool requisition(Shape* shape, std::string weaponName, int weaponCode);
	void armShape(Shape* shape, Weapon::WeaponI* weapon);
	void disarmShape(Shape* shape);
	
	//Add multiple projectiles
	void fireWeapon(std::vector<ProjectileDef>& defs, std::string id);

	std::function<Shape*()> getControlled_;

	//void loadLevel();
	//b2Vec2 randomPos(); //On a circle, in an arc, from centre, whatever, reuse code test stuff
	//

	//Cleaning up
	void resetLevel();
	void clearWorld(bool clearPlayer = true);

	//Bounds manipulation
	float getBoundsRadius();
	void resizeBounds(float radius);
	float getBoundsSide();

	//Controlled manipulation
	void move(b2Vec2& direction);
	void look(b2Vec2& direction);
	void trigger(b2Vec2& direction);
	void release();
	void reup();

	//Legacy
	void controlNextEnemy();
	void controlPrevEnemy();

	//Passthroughs to b2world
	void SetDebugDraw(b2Draw* debugDraw);
	void DrawDebugData();

	//Return a reference to lists for drawing
	Bounds* getBounds();
	Shape* getPlayer();
	std::list<Enemy*>& getShapes();
	std::list<Projectile*>& getProjectiles();
	std::list<Side*>& getSides();
	std::list<Pickup::PickupI*>& getPickups();
	std::list<Force*>& getForces();
	int getSpawns() { return spawns_; } //////DELETE THIS

	//Update & Pause
	void step(int dt);
	void update(int dt);
	void pause();
	bool getPaused() const;
	void resume();

	//Haptic feedback passbacks
	int getHapticL() const;
	int getHapticR() const;

	//////SPRINT 3 SLINGING
	int hiSides;
	unsigned int hiTime;

	void bomb(bool nuke = false);

	int enemies;
	int freesides;

	int maxTime = 120;
	int getTimeInLevel() { return timeInLevel_ / 1000; }

	/////Debug
	//Debug vars
	std::string dstr;
	int di;
	void testBed();
	void f1();
	void f2();
	void f3();
	void f4();
	void f5();
	void f6();
	void f7();
	void f8();
	void f9();
	void f0();
private:
	//B2 stuff
	const b2Vec2 GRAVITY = b2Vec2(0, 0);
	const int VELOCITY_ITERS = 6;
	const int POSITION_ITERS = 2;
	ContactListener contactListener_;
	b2FrictionJointDef boundsFriction_;
	
	//Passthroughs
	bool pause_;
	int leftHaptic_;
	int rightHaptic_;
	
	//And callbacks for entities
	std::function<void(ProjectileDef&)> addProj_;
	std::function<void(std::vector<ProjectileDef>&, std::string)> fireWeap_;
	std::function<void(SideDef&)> addSide_;
	std::function<void(b2Vec2 pos, float force, float radius, int time)> addForce_;
	//std::function<void(ShapeDef &def)> addShape_;

	//Entities
	Player* player_;
	Bounds* bounds_;
	std::list<Enemy*> shapes_;
	std::list<Projectile*> projectiles_;
	std::list<Side*> sides_;
	std::list<Weapon::WeaponI*> weapons_;
	std::list<Pickup::PickupI*> pickups_;
	std::list<Force*> forces_;

	//Weapons
	Weapon::Armory* armory_;
	
	//Controlled shape pointer
	Shape* controlled_;

	//Body creation
	b2Body* addDynamicBody(const b2Vec2& pos);
	b2Body* addStaticBody (const b2Vec2& pos);
	b2Body* addBulletBody (const b2Vec2& pos);

	//Keep inside bounds
	void popInside(Entity* ent);

	//AI spawning
	void spawnEnemy();
	unsigned int lastSpawn_;
	unsigned int spawnTime_;
	unsigned int spawns_;

	//Timing
	unsigned int timeInLevel_;

	//Updating
	void updatePlayer(int dt);
	void updateEnemy(int dt);
	void updateProjectile(int dt);
	void updateSide(int dt);
	void updatePickup(int dt);
	void updateLevel(int dt);
	void updateForce(int dt);

	//Cleanup
	void cullWeapons();
	void cleanPickups(Shape* shape);

	//SFX & Music
	sf::Vector2f B2toSF(const b2Vec2& vec, bool scale) const;
	SoundSystem audio_;

	//Just for fun
	void randomiseCol(Entity* e);
};

#endif