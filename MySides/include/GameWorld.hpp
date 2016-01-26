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
#include "Weapon.hpp"
//#include "Projectile.hpp"
#include "Side.hpp"

//Checks if entity is alive, predicate for remove_if
static bool isAlive(Entity* e)
{
	return e->getAlive();
}

//Checks if entity is active, predicate for remove_if
static bool isActive(Entity e){
	return e.getActive();
}

class GameWorld : protected b2World {
public:
	GameWorld();

	Shape * controlled();
	bool hasControlled();

	//These should go soon
	void addPlayer(float x, float y, bool control);
	void addEnemy(float x, float y);
	void addProjectile(float x, float y, float vx, float vy);
	void addSide(float x, float y, float nx, float ny, float size);

	//Spawning works off definitions
	//void addPlayer(ShapeDef def);
	//void addEnemy(ShapeDef def)
	void addProjectile(ProjectileDef &def);
	void addSide(SideDef def);

	//And callbacks for entities
	std::function<void(ProjectileDef&)> addProj_;	
	//std::function<void(SideDef &def)> addSide_;
	//std::function<void(ShapeDef &def)> addShape_;

	//Called by update
	void removePlayer(std::list<Shape>::iterator& p);
	void removeEnemy(std::list<Shape>::iterator& e);
	void removeProjectile(std::list<Projectile>::iterator& p);
	void removeSide(std::list<Side>::iterator& s);

	//void clear(bool clearPlayer);
	//void loadLevel();
	//b2Vec2 randomPos(); //On a circle, in an arc, from centre, whatever, reuse code test stuff
	//

	void resetLevel();
	void clearWorld();

	float getBoundsRadius();
	void resizeBounds(float radius);
	float getBoundsSide();

	void move(b2Vec2 direction);
	void fire(b2Vec2 direction);

	void controlNextEnemy();
	void controlPrevEnemy();

	void SetDebugDraw(b2Draw* debugDraw);
	void DrawDebugData();

	//Return a reference to shapes, bounds, projectiles for drawing
	Bounds& getBounds();
	std::list<Shape>& getPlayers();
	std::list<Shape>& getShapes();
	std::list<Projectile>& getProjectiles();
	std::list<Side>& getSides();

	void update(int dt);
private:
	const b2Vec2 GRAVITY = b2Vec2(0, 0.1);
	const int VELOCITY_ITERS = 6;
	const int POSITION_ITERS = 2;

	std::list<Shape> players_;
	std::list<Shape> shapes_;
	std::list<Projectile> projectiles_;
	std::list<Side> sides_;
	std::list<Entity> addons_; // Addons for shapes (Store here?)

	std::list<Shape>::iterator controlled_;
	
	ContactListener contactListener_;
	Bounds bounds_;

	b2Body* addDynamicBody(float x, float y);
	b2Body* addStaticBody (float x, float y);
	b2Body* addBulletBody (float x, float y);

	//SFX & Music
	sf::Music bgm_;
	sf::SoundBuffer fireBuffer, spawnBuffer, dieBuffer, lossBuffer, collectBuffer;
	sf::Sound fireSound, spawnSound, dieSound, lossSound, collectSound;

	//Translates sound from b2v2 to sfv3 audio
	void positionSound(sf::Sound& sound, b2Vec2 pos, bool scale);

	//Translates a position from b2v2 to sfv3 listener
	void positionListener(b2Vec2 pos, bool scale);

};

#endif