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


class GameWorld : protected b2World {
public:
	GameWorld();
	~GameWorld();

	Shape* getControlled();
	bool hasControlled();

	//These should go soon
	void addPlayer(float x, float y, bool control);
	void addEnemy(float x, float y);

	//Spawning works off definitions
	//void addPlayer(ShapeDef &def);
	//void addEnemy(ShapeDef &def)
	void addProjectile(ProjectileDef &def);
	void addSide(SideDef &def);

	void armShape(Shape* shape);
	void disarmShape(Shape* shape);

	//And callbacks for entities
	std::function<void(ProjectileDef&)> addProj_;	
	std::function<void(SideDef &def)> addSide_;
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
	Bounds* getBounds();
	Shape* getPlayer();
	std::list<Enemy*>& getShapes();
	std::list<Projectile*>& getProjectiles();
	std::list<Side*>& getSides();

	void update(int dt);

	////TEMP
	void PutEnemy() 
	{
		float x, y, rad = getBoundsRadius() * 0.7f;
		y = -(cos((2 * M_PI) * 32 / randFloat(0, 32)));
		x = -(sin((2 * M_PI) * 32 / randFloat(0, 32)));

		x *= rad;
		y *= rad;

		addEnemy(x, y);
	}

	//////SPRINT 3 SLINGING
	int hiSides;
	unsigned int hiTime;

	void bomb();

	int enemies;
	int freesides;

	int maxTime = 120;
	int getTimeInLevel() { return timeInLevel_ / 1000; }

private:
	const b2Vec2 GRAVITY = b2Vec2(0, 0.1);
	const int VELOCITY_ITERS = 6;
	const int POSITION_ITERS = 2;
	
	Player* player_;
	std::list<Enemy*> shapes_;
	std::list<Projectile*> projectiles_;
	std::list<Side*> sides_;

	Shape* controlled_;
	
	ContactListener contactListener_;
	Bounds* bounds_;

	b2Body* addDynamicBody(float x, float y);
	b2Body* addStaticBody (float x, float y);
	b2Body* addBulletBody (float x, float y);

	void popInside(Entity* ent);

	//AI spawning
	void spawnEnemy();
	unsigned int lastSpawn_;
	unsigned int spawnTime_;
	unsigned int spawns_;

	//Timing
	unsigned int timeInLevel_;

	//SFX & Music
	sf::Music bgm_;
	sf::SoundBuffer fireBuffer, spawnBuffer, dieBuffer, lossBuffer, dropBuffer, collectBuffer;
	sf::Sound fireSound, spawnSound, dieSound, lossSound, dropSound, collectSound;

	//Translates sound from b2v2 to sfv3 audio
	void positionSound(sf::Sound& sound, b2Vec2 pos, bool scale);

	//Translates a position from b2v2 to sfv3 listener
	void positionListener(b2Vec2 pos, bool scale);
};

#endif