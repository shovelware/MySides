#include "stdafx.h"
#include "Game.hpp"

//Globally accessible logger, usage: extern Log l;
Log l;

Game::Game() : videoMode_(1280, 720, 32), window_(videoMode_, "My Sides!", sf::Style::Titlebar), mousein_(false), quit_(false), fullscreen_(false), dd_(window_)
{
}

int Game::run()
{
#pragma region Startup

	//Window info
	sf::Vector2u windowSize = window_.getSize();
	
	//Fixed Timestep
	sf::Clock frameClock;
	sf::Time frameTime = sf::Time::Zero;
	sf::Time tickTime = sf::Time(sf::seconds(_TICKTIME_));
	sf::Time accumulator = sf::Time::Zero;
	
	//Logging
	Log* lptr = new Log();
	l = *lptr;

	//World
	world_ = new GameWorld();

	//Drawing
	drawer_ = new GameDrawer(window_, world_);
	
	//Debugdraw
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	//flags += b2Draw::e_jointBit;
	//flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	//flags += b2Draw::e_centerOfMassBit;
	dd_.SetFlags(flags);
	world_->SetDebugDraw(&dd_);

	//Testing

	//Bodies
	world_->addEnemy(10, 4);
	world_->addEnemy(12, 4);
	world_->addEnemy(15, 4);
	world_->addEnemy(17, 4);
	world_->addEnemy(20, 4);
	
	world_->addEnemy(10, 16);
	world_->addEnemy(12, 16);
	world_->addEnemy(15, 16);
	world_->addEnemy(17, 16);
	world_->addEnemy(20, 16);

	world_->addPlayer(15, 10, true);

	//Display a blank window before we start our game loop
	//Avoids nasty white windows
	window_.clear(sf::Color::Black);
	window_.display();
#pragma endregion

#pragma region Game Loop

	//Game loop
	while (!quit_)
	{
		//Take inputs, hand other events to a vector
		//process these events in processevents later
		//processEvents(inputManager.update());?
		processEvents();

		update_ = (window_.hasFocus());// && mousein_);

		//Get delta time since last frame
		frameTime = frameClock.restart();
		
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
			//Only update if the controller is connected
			//if (checkController(tickTime))
			{
				//Update with this tick
				checkController(tickTime); //XOR with if above
				if (!pause_)
				update(tickTime);

				//Take this tick out of the accumulator
				accumulator -= tickTime;
			}

		}

		render();
	}
#pragma endregion

#pragma region Shutdown

	//Exiting
	window_.close();

	//Free resources
	delete world_;
	delete drawer_;
	delete lptr;

	return EXIT_SUCCESS;

#pragma endregion
}

b2Vec2 Game::SFtoB2(const sf::Vector2f & vec) //Do we really need to spawn things in screen space co-ords?
{
	return b2Vec2(vec.x / _SCALE_, vec.y / _SCALE_ );
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

			//Key press event
		case sf::Event::KeyPressed:
			//Escape : Quit game
			if (evt.key.code == sf::Keyboard::Key::Escape)
				quit_ = true;

			//Return: Toggle Fullscreen
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
			
			//Space : Pause game
			if (evt.key.code == sf::Keyboard::Key::Space)
			{
				pause_ = !pause_;
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

bool Game::checkController(sf::Time dt)
{
	bool connected = false;

	//If we're paused, don't update the controller's button holds
	if (pause_)
	{
		connected = con_.update(-1);
	}

	//Normal update
	else connected = con_.update(dt.asMilliseconds());
	
	return connected;
}

void Game::update(sf::Time dt)
{
	//If the world has a controlled body
	if (world_->hasControlled())
	{
		world_->move(b2Vec2(con_.checkLeftX(), con_.checkLeftY()));
		world_->fire(b2Vec2(con_.checkRightX(), con_.checkRightY()));

		//world_->player()->rotate(con_.checkRightX() / 10);

		//HALT
		if (con_.checkDown(XINPUT_GAMEPAD_A))
		{
			world_->player()->stopMove();
		}

		if (con_.checkDown(XINPUT_GAMEPAD_B))
		{
			world_->player()->stopRotate();
		}

		//Orientation testing
		if (con_.checkDown(XINPUT_GAMEPAD_X))
		{
			world_->player()->orient(b2Vec2_zero);
		}

		//Control swapping
		if (con_.checkPressed(XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			world_->controlPrev();
		}

		if (con_.checkPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			world_->controlNext();
		}

		//Bounds resizing
		if (con_.checkPressed(XINPUT_GAMEPAD_RIGHT_THUMB))
		{
			world_->resizeBounds(5);
		}

		if (con_.checkPressed(XINPUT_GAMEPAD_LEFT_THUMB))
		{
			world_->resizeBounds(10);
		}

		//DPad firing
		if (con_.checkPressed(XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			world_->fire(b2Vec2(1, 0));
		}

		if (con_.checkPressed(XINPUT_GAMEPAD_DPAD_LEFT))
		{
			world_->fire(b2Vec2(-1, 0));
		}

		if (con_.checkPressed(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			world_->fire(b2Vec2(0, 1));
		}

		if (con_.checkPressed(XINPUT_GAMEPAD_DPAD_UP))
		{
			world_->fire(b2Vec2(0, -1));
		}
	}

	world_->update(dt.asMilliseconds());

	//Update counter
	//static int x;
	//x++;
	//std::ostringstream UD;
	//UD << "Update" << x;
	//l.out(l.message, 'G', UD.str().c_str());
}

void Game::render()
{
	window_.clear();
	//l.out(l.message, 'G', "Render");

	//Render stuff
	world_->DrawDebugData();

	//b2Shape* x = world_->player()->getVertices();
	//b2Shape::Type y = x->GetType();

	drawer_->drawPoint(sf::Vector2f(10, 15));
	drawer_->drawLine(sf::Vector2f(11, 16), sf::Vector2f(24, 28));
	drawer_->drawCircle(sf::Vector2f(40, 40), 8);

	window_.display();
}