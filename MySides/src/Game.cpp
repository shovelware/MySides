#include "stdafx.h"
#include "Game.hpp"

//Globally accessible logger, usage: extern Log l;
Log l;

Game::Game() : videoMode_(1280, 720, 32), window_(videoMode_, "My Sides!", sf::Style::Titlebar), mousein_(false), quit_(false), fullscreen_(false)
{
}

int Game::run()
{

#pragma region Gamestuff

	//Window info
	sf::Vector2u windowSize = window_.getSize();
	
	//Fixed Timestep
	sf::Clock frameClock;
	sf::Time frameTime = sf::Time::Zero;
	sf::Time tickTime = sf::Time(sf::seconds(1.f / 30.f));
	sf::Time accumulator = sf::Time::Zero;

	//Logging
	l = *(new Log());

#pragma endregion

	//Display a blank window before we start our game loop
	//Avoids nasty white windows
	window_.display();
	
	//Game loop
	while (!quit_)
	{
		//Take inputs, hand other events to a vector
		//process these events in processevents later
		//processEvents(inputManager.update());?
		processEvents();

		update_ = (window_.hasFocus() && mousein_);

		//Get delta time since last frame
		frameTime = frameClock.restart();

		//update our controls with the frametime
		//input.update(frameTime);

		//If we don't want to update, dump the frametime
		//and skip the rest of the loop
		if (!update_)
		{
			continue;
		}

		accumulator += frameTime;

		//Update to number of physics steps
		while (accumulator >= tickTime)
		{
			update(tickTime);
			accumulator -= tickTime;
		}

		render();
	}
	
	window_.close();
	return EXIT_SUCCESS;
}

void Game::processEvents()
{
	sf::Event evt;
	while (window_.pollEvent(evt))
	{
		switch (evt.type)
		{
		case sf::Event::Closed:
			quit_ = true;
			break;

		case sf::Event::KeyPressed:
			if (evt.key.code == sf::Keyboard::Key::Escape)
				quit_ = true;

			if (evt.key.code == sf::Keyboard::Key::Return)
				if (!fullscreen_)
				{
					window_.create(sf::VideoMode::getDesktopMode(), "My Sides!", sf::Style::Fullscreen);
					window_.display();
					fullscreen_ = true;
				}

				else if (fullscreen_)
				{
					window_.create(videoMode_, "My Sides!", sf::Style::Titlebar);
					window_.display();
					fullscreen_ = false;
				}
			break;

		case sf::Event::MouseLeft:
			mousein_ = false;
			break;

		case sf::Event::MouseEntered:
			mousein_ = true;
			break;
		}
	}
}

void Game::update(sf::Time dt)
{
	l.out(l.message, 'G', "Update");
}

void Game::render()
{
	window_.clear();
	l.out(l.message, 'G', "Render");

	//Render stuff

	window_.display();
}
