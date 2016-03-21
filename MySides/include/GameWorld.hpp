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

#include "Player.hpp"
#include "Enemy.hpp"

#include "ShapeDef.hpp"
#include "SideDef.hpp"

#include "Weapon.hpp"
#include "WeapRifle.hpp"
#include "WeapShotgun.hpp"

#include "SoundSystem.hpp"


class GameWorld : protected b2World {
public:
	GameWorld();
	~GameWorld();

	Shape* getControlled();
	bool hasControlled();

	//These should go soon
	void addPlayer(const b2Vec2& pos, bool control);
	void addEnemy(const b2Vec2& pos);

	//Spawning works off definitions
	void addPlayer(const ShapeDef& def);
	void addEnemy(const ShapeDef& def);
	void addProjectile(const ProjectileDef& def);
	void addSide(const SideDef& def);

	void armShape(Shape* shape);
	void disarmShape(Shape* shape);

	void fireWeapon(std::vector<ProjectileDef>& defs, std::string id);

	//And callbacks for entities
	std::function<void(ProjectileDef&)> addProj_;	
	std::function<void(std::vector<ProjectileDef>&, std::string)> fireWeap_;
	std::function<void(SideDef&)> addSide_;
	//std::function<void(ShapeDef &def)> addShape_;
	std::function<Shape*()> getControlled_;

	//Called by update
	void removePlayer();
	void removeEnemy(std::list<Enemy*>::iterator& e);
	void removeProjectile(std::list<Projectile*>::iterator& p);
	void removeSide(std::list<Side*>::iterator& s);

	//void loadLevel();
	//b2Vec2 randomPos(); //On a circle, in an arc, from centre, whatever, reuse code test stuff
	//

	//Cleaning up
	void resetLevel();
	void clearWorld();

	//Bounds manipulation
	float getBoundsRadius();
	void resizeBounds(float radius);
	float getBoundsSide();

	//Controlled manipulation
	void move(b2Vec2 direction);
	void fire(b2Vec2 direction);

	//Legacy
	void controlNextEnemy();
	void controlPrevEnemy();

	//Passthroughs to b2world
	void SetDebugDraw(b2Draw* debugDraw);
	void DrawDebugData();


	//Return a reference to shapes, bounds, projectiles for drawing
	Bounds* getBounds();
	Shape* getPlayer();
	std::list<Enemy*>& getShapes();
	std::list<Projectile*>& getProjectiles();
	std::list<Side*>& getSides();

	//Update & Pause
	void update(int dt);
	void pause();
	bool getPaused() const;
	void resume();

	////TEMP
	void PutEnemy() 
	{
		float x, y, rad = getBoundsRadius() * 0.7f;
		y = -(cos((2 * M_PI) * 32 / randFloat(0, 32)));
		x = -(sin((2 * M_PI) * 32 / randFloat(0, 32)));

		x *= rad;
		y *= rad;

		addEnemy(b2Vec2(x, y));
	}

	//////SPRINT 3 SLINGING
	int hiSides;
	unsigned int hiTime;

	void bomb();

	int enemies;
	int freesides;

	int maxTime = 120;
	int getTimeInLevel() { return timeInLevel_ / 1000; }

	/////Debug
	void testBed();

private:
	const b2Vec2 GRAVITY = b2Vec2(0, 0);
	const int VELOCITY_ITERS = 6;
	const int POSITION_ITERS = 2;
	
	bool pause_;

	Player* player_;
	std::list<Enemy*> shapes_;
	std::list<Projectile*> projectiles_;
	std::list<Side*> sides_;

	Shape* controlled_;
	
	ContactListener contactListener_;
	Bounds* bounds_;

	b2Body* addDynamicBody(const b2Vec2& pos);
	b2Body* addStaticBody (const b2Vec2& pos);
	b2Body* addBulletBody (const b2Vec2& pos);

	void popInside(Entity* ent);

	//AI spawning
	void spawnEnemy();
	unsigned int lastSpawn_;
	unsigned int spawnTime_;
	unsigned int spawns_;

	//Timing
	unsigned int timeInLevel_;

	//SFX & Music
	sf::Vector2f B2toSF(const b2Vec2& vec, bool scale) const;
	SoundSystem audio_;

	//Just for fun
	void randomiseCol(Entity* e);

};

#endif