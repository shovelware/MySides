#include "stdafx.h"
#include "Game.hpp"

#include "Logtest.hpp"

//Globally accessible logger, use extern Log l;
Log l;

Game::Game() : window_(sf::VideoMode(1280, 720, 32), "My Sides!")
{
}

void Game::run()
{

#pragma region Gamestuff

	//Window info
	sf::Vector2u windowSize = window_.getSize();

	//Fixed Timestep
	sf::Clock frameClock;
	sf::Time tickTime = sf::Time(sf::seconds(1.f / 60.f));
	sf::Time accumulator = sf::Time::Zero;

	//Logging
	l = *(new Log());
	logtest l;

	
#pragma endregion

	//Game loop
	bool quit = false;
	while (window_.isOpen())
	{
		//Get delta time since last frame
		sf::Time frameTime = frameClock.restart();
		accumulator += frameTime;

		//Take inputs
		processEvents();

		//Update to number of physics steps
		while (accumulator >= tickTime)
		{
			update(tickTime);
			accumulator -= tickTime;
		}

		render();
	}
}

void Game::processEvents()
{
	sf::Event evt;
	while (window_.pollEvent(evt))
	{
		if (evt.type == sf::Event::Closed)
			window_.close();

		if (evt.type == sf::Event::KeyPressed && evt.key.code == sf::Keyboard::Key::Escape)
		{
			window_.close();
		}
	}
}

void Game::update(sf::Time dt)
{

}

void Game::render()
{
	window_.clear();


	//Render stuff

	window_.display();
}
