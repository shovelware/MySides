// Game.hpp
// Handles loops, game logic, box2d world

#ifndef GAME_H
#define GAME_H

#include "stdafx.h"

#include <SFML/Graphics.hpp>
#include <Thor\Vectors\VectorAlgebra2D.hpp>

class Game
{

public:
	Game();
	void run();
private:
	void processEvents();
	void update(sf::Time dt);
	void render();	

	sf::RenderWindow window_;

};
#endif