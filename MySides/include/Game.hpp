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

//Keyboard
#include "KeyInput.hpp"

//Game
#include "GameWorld.hpp"
#include "Camera.hpp"

//Rendering
#include "Drawer.hpp"
#include "GameDrawer.hpp"

//////Testing
#include <sstream>

class Game
{
public:
	Game();
	int run();

private:
	b2Vec2 SFtoB2(const sf::Vector2f &vec);
	void processEvents();
	void handleInput(sf::Time dt);
	void update(sf::Time dt);
	void render();

	void toggleFullscreen();

	//Control bools
	bool mousein_;
	bool update_;
	bool pause_;
	bool quit_;

	//Drawing
	bool fullscreen_;
	sf::VideoMode videoMode_;
	sf::ContextSettings contextSettings_;
	sf::RenderWindow window_;
	sf::Color voidCol_;
	GameDrawer* drawer_;

	//This will be broken out into inputmanager
	bool checkController(sf::Time dt);
	XController con_;
	
	//Keyboard
	KeyInput key_;


	//Game world 
	GameWorld* world_;

	//Camera
	Camera* camera_;

	//DebugDraw
	SFMLDebugDraw dd_;

	//Testing

};
#endif