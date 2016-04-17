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
#include "Armory.hpp"

#include "SideDef.hpp"
#include "ForceDef.hpp"

#include "LevelMenu.hpp"
#include "Levels.hpp"

#include "Pickup.hpp"
#include "Sight.hpp"
#include "Shield.hpp"
#include "Attractor.hpp"

#include "SoundSystem.hpp"

class GameWorld : protected b2World {
public:
	GameWorld();
	~GameWorld();

	Shape* getControlled();
	bool hasControlled() const;
	bool inMenu() const;

	//Spawning works off definitions
	void addPlayer(const PlayerDef& def);
	void addEnemy(const EnemyDef& def);
	void addProjectile(const ProjectileDef& def);
	void addSide(const SideDef& def);
	void addPickup(Pickup::Type type, b2Vec2 position, int time, float strength = 0);
	void addShrapnel(Projectile* src);
	void addForce(b2Vec2 pos, float force, float radius, int time, int faction = 0);
	void addForce(const ForceDef& def);

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
	void clearEnemies();
	void clearWorld(bool clearPlayer = true);

	//Level control
	void loadMenu();
	void loadLevel(Level::LevelI* level);
	void startLevel();
	void resetLevel();
	void unloadLevel();
	void selectLevel(std::string name);
	void selectLevel(int index);
	void menuToLevel();
	void returnToMenu();

	//Bounds manipulation
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

	//Return a reference to items for drawing
	Shape* getPlayer();
	Bounds& const getBounds();
	std::list<Enemy*>& const getShapes();
	std::list<Projectile*>& const getProjectiles();
	std::list<Side*>& const getSides();
	std::list<Pickup::PickupI*>& const getPickups();
	std::list<Force*>& const getForces();
	Level::LevelI& const getWorldLevel();
	Level::LevelI& const getSelectedLevel();
	float getTransitionProgress();


	//Update & Pause
	void step(int dt);
	void update(int dt);
	void pause();
	bool getPaused() const;
	void resume();

	//Haptic feedback passbacks
	int getHapticL() const;
	int getHapticR() const;

	//Bomb
	void bomb(bool nuke = false);

	//Entity tracking
	int hiSides;
	int enemies;
	int freesides;

	/////Debug
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
	
	//Entities
	Player* player_;
	Bounds* bounds_;
	std::list<Enemy*> shapes_;
	std::list<Projectile*> projectiles_;
	std::list<Side*> sides_;
	std::list<Weapon::WeaponI*> weapons_;
	std::list<Pickup::PickupI*> pickups_;
	std::list<Force*> forces_;

	//Controlled shape pointer
	Shape* controlled_;

	//Callbacks for entities
	std::function<void(ProjectileDef&)> addProj_;
	std::function<void(std::vector<ProjectileDef>&, std::string)> fireWeap_;
	std::function<void(SideDef&)> addSide_;
	std::function<void(b2Vec2 pos, float force, float radius, int time, int faction)> addForce_;
	//std::function<void(ShapeDef &def)> addShape_;

	//Weapons
	Weapon::Armory* armory_;
	
	//Body creation
	b2Body* addDynamicBody(const b2Vec2& pos);
	b2Body* addStaticBody (const b2Vec2& pos);
	b2Body* addBulletBody (const b2Vec2& pos);

	//Keep inside bounds
	void popInside(Entity* ent);

	//Levels
	bool menu_;
	Level::Menu* menuLevel_;
	Level::LevelI* worldLevel_;
	std::list<Level::LevelI*> levels_;
	std::list<Level::LevelI*>::iterator selectedLevel_;
	void populateLevelList();
	void clearLevelList();

	//Updating
	void updatePlayer(int dt);
	void updateEnemy(int dt);
	void updateProjectile(int dt);
	void updateSide(int dt);
	void updatePickup(int dt);
	void updateMenu(int dt);
	void updateLevel(int dt);
	void updateForce(int dt);

	//Cleanup
	void cullWeapons();
	void cleanPickups(Shape* shape);

	//SFX & Music
	sf::Vector2f B2toSF(const b2Vec2& vec, bool scale) const;
	SoundSystem audio_;
	int lastSides_; //For detecting collection

	//Transitions
	int transitionDir_;
	int transitionTime_;
	int transitionTimeMAX_;

	//Just for fun
	void randomiseCol(Entity* e);
};

#endif