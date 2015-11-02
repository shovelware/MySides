// Game.hpp
// Handles loops, game logic, box2d world

#ifndef GAME_H
#define GAME_H

#include "stdafx.h"

//SFML
#include <SFML/Graphics.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>

//Box2D
#include <Box2D/Box2D.h>

//Controller
#include "XController.hpp"

//////
#include <sstream>

class Game
{
private:
	void processEvents();
	void update(sf::Time dt);
	void render();	

	bool mousein_;
	bool focus_;

	bool update_;
	bool quit_;

	bool fullscreen_;

	XController con;

	sf::VideoMode videoMode_;
	sf::RenderWindow window_;
public:
	Game();
	int run();
};
#endif