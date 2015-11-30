// Game.hpp
// Handles loops, game logic, box2d world

#ifndef MS_GAME_HPP
#define MS_GAME_HPP

#include "stdafx.h"

//SFML
#include <SFML/Graphics.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>

//Box2D
#include <Box2D/Box2D.h>

//Controller
#include "XController.hpp"

//Game
#include "GameWorld.hpp"

//Rendering
#include "Drawer.hpp"
#include "GameDrawer.hpp"

//////Testing
#include <sstream>

class Game
{
private:
	b2Vec2 SFtoB2(const sf::Vector2f &vec);
	void processEvents();
	void update(sf::Time dt);
	void render();

	//Control bools
	bool mousein_;
	bool update_;
	bool pause_;
	bool quit_;

	//Drawing
	bool fullscreen_;
	sf::VideoMode videoMode_;
	sf::RenderWindow window_;
	GameDrawer* drawer_;

	//This will be broken out into inputmanager
	bool checkController(sf::Time dt);
	XController con_;

	//Game world 
	GameWorld* world_;

	//DebugDraw
	SFMLDebugDraw dd_;

	//Testing

public:
	Game();
	int run();
};
#endif