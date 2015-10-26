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

class Game
{
private:
	void processEvents();
	void update(sf::Time dt);
	void render();	

	sf::RenderWindow window_;
public:
	Game();
	int run();
};
#endif