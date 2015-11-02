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
		con.update(frameTime.asMilliseconds());

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
	//con.update(dt.asMilliseconds());
	//l.out(l.message, 'G', "Update");
	if (con.checkDown(XINPUT_GAMEPAD_A))
	{
		l.out(l.message, 'G', "A Button");
	}

	if (con.checkDown(XINPUT_GAMEPAD_B))
	{
		l.out(l.message, 'G', "B Button");
	}

	if (con.checkDown(XINPUT_GAMEPAD_X))
	{
		l.out(l.message, 'G', "X Button");
	}

	if (con.checkDown(XINPUT_GAMEPAD_Y))
	{
		l.out(l.message, 'G', "Y Button");
	}

	if (con.checkDown(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		l.out(l.message, 'G', "Left Bumper");
	}

	if (con.checkDown(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		l.out(l.message, 'G', "Right Bumper");
	}

	if (con.checkDown(XINPUT_GAMEPAD_LEFT_THUMB))
	{
		l.out(l.message, 'G', "Left Click");
	}

	if (con.checkDown(XINPUT_GAMEPAD_RIGHT_THUMB))
	{
		l.out(l.message, 'G', "Right Click");
	}

	if (con.checkDPadX() != 0)
	{
		std::ostringstream DP;
		DP << "DPad X " << con.checkDPadX();
		l.out(l.message, 'G', DP.str().c_str());
	}

	if (con.checkDPadY() != 0)
	{
		std::ostringstream DP;
		DP << "DPad Y " << con.checkDPadY();
		l.out(l.message, 'G', DP.str().c_str());
	}

	if (con.checkDown(XINPUT_GAMEPAD_BACK))
	{
		l.out(l.message, 'G', "Back Button");
	}

	if (con.checkDown(XINPUT_GAMEPAD_START))
	{
		l.out(l.message, 'G', "Start Button");
	}

	if (con.checkReleased(XINPUT_GAMEPAD_A))
	{
		if (con.checkTimeHeld(XINPUT_GAMEPAD_A) > 0)
		{
			std::ostringstream HT;
			HT << con.checkTimeHeld(XINPUT_GAMEPAD_A);
			l.out(l.message, 'G', HT.str().c_str() );
		}
	}

	if (con.checkLeftTrigger() > .5f)
	{
		std::ostringstream LS;
		LS << "\n" << "LX " << con.checkLeftX() << "\n" << "LY " << con.checkLeftY();
		l.out(l.message, 'G', LS.str().c_str() );
	}

	if (con.checkRightTrigger() > .5f)
	{
		std::ostringstream RS;
		RS << "\n" << "RX " << con.checkRightX() << "\n" << "RY " << con.checkRightY();
		l.out(l.message, 'G', RS.str().c_str());
	}

}

void Game::render()
{
	window_.clear();
	//l.out(l.message, 'G', "Render");

	//Render stuff

	window_.display();
}