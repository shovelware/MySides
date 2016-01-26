#include "stdafx.h"
#include "Game.hpp"

//Globally accessible logger, usage: extern Log l;
Log l;

//inline float random functions for now
inline float randFloat(float MAX) { return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / MAX)); };
inline float randFloat(float MIN, float MAX) { return MIN + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (MAX - MIN))); };

Game::Game() : videoMode_(1280, 720, 32), window_(videoMode_, "My Sides!", sf::Style::Titlebar, sf::ContextSettings(0u, 0u, 8u)), mousein_(false), quit_(false), fullscreen_(false), dd_(window_)
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
	camera_ = new Camera(window_);

	//Seed random
	srand(static_cast <unsigned> (time(0)));
	
	//Drawing
	drawer_ = new GameDrawer(window_, world_);
	
	//Debugdraw
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	//flags += b2Draw::e_jointBit;
	//flags += b2Draw::e_aabbBit;
	//flags += b2Draw::e_pairBit;
	//flags += b2Draw::e_centerOfMassBit;
	dd_.SetFlags(flags);
	world_->SetDebugDraw(&dd_);

	//Testing


	//Display a blank window before we start our game loop
	//Avoids nasty white windows
	window_.clear(sf::Color::Black);
	window_.display();

	//Begin game
	world_->resetLevel();
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

				key_.update();
				handleInput(tickTime);

				if (!pause_)
				{
					update(tickTime);
				}

				//Take this tick out of the accumulator
				accumulator -= tickTime;
				
				//Render this frame
				render();
			}

		}

	}
#pragma endregion

#pragma region Shutdown

	//Exiting
	window_.close();

	//Free resources
	delete world_;
	delete drawer_;
	delete lptr;
	delete camera_;

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
		//Rudimentary update count
		//static char x = 0;
		//x++;
		//l.out(l.priority, 'G', &x);

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

void Game::handleInput(sf::Time dt)
{
	//If the world has a controlled body
	if (world_->hasControlled())
	{
		world_->move(b2Vec2(con_.checkLeftX(), con_.checkLeftY()));
		world_->fire(b2Vec2(con_.checkRightX(), con_.checkRightY()));

		//Keyboard backup controls
		b2Vec2 mv(0, 0);

		if (key_.isKeyDown(Key::W)) { mv.y += -1; }
		if (key_.isKeyDown(Key::S)) { mv.y += 1; }
		if (key_.isKeyDown(Key::A)) { mv.x += -1; }
		if (key_.isKeyDown(Key::D)) { mv.x += 1; }

		if (key_.isKeyDown(Key::LAlt)) { mv *= 0.75f; }

		world_->move(mv);

		b2Vec2 fr(0, 0);

		if (key_.isKeyDown(Key::Up)) { fr.y += -1; }
		if (key_.isKeyDown(Key::Down)) { fr.y += 1; }
		if (key_.isKeyDown(Key::Left)) { fr.x += -1; }
		if (key_.isKeyDown(Key::Right)) { fr.x += 1; }

		world_->fire(fr);

		//world_->controlled()->rotate(con_.checkRightX() / 10);

		//Resize bounds
		if (con_.checkPressed(XINPUT_GAMEPAD_A))
		{
			float base = 32;
			float lt = con_.checkLeftTrigger() * 96;

			world_->resizeBounds(base + lt);

			std::cout << base + lt << "  " << world_->getBoundsSide() << std::endl;


		}

		if (con_.checkDown(XINPUT_GAMEPAD_B))
		{
			world_->controlled()->stopRotate();
		}

		//Orientation testing
		if (con_.checkDown(XINPUT_GAMEPAD_X))
		{
			world_->controlled()->orient(b2Vec2_zero);
		}

		//Spawn enemy
		if (con_.checkPressed(XINPUT_GAMEPAD_Y))
		{
			float lt = (con_.checkLeftTrigger() > 0 ? con_.checkLeftTrigger() : 0.1f);
			int spawn = lt * 10;

			for (spawn; spawn > 0; --spawn)
			{
				float x, y, rad = world_->getBoundsRadius() * 0.7f;
				y = -(cos((2 * M_PI) * 32 / randFloat(0, 32)));
				x = -(sin((2 * M_PI) * 32 / randFloat(0, 32)));

				x *= rad;
				y *= rad;

				world_->addEnemy(x, y);
			}
		}

		if (con_.checkPressed(XINPUT_GAMEPAD_LEFT_SHOULDER))
		{

		}

		if (con_.checkPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{

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

		if (con_.checkReleased(XINPUT_GAMEPAD_BACK))
		{
			if (con_.checkTimeHeld(XINPUT_GAMEPAD_BACK) > 500)
			{
				camera_->zoomReset();
			}

			else
			{
				camera_->zoom(.5f);
			}
		}

		if (con_.checkPressed(XINPUT_GAMEPAD_LEFT_SHOULDER) && con_.checkDown(XINPUT_GAMEPAD_RIGHT_SHOULDER) || 
			con_.checkDown(XINPUT_GAMEPAD_LEFT_SHOULDER) && con_.checkPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			world_->resetLevel();
		}

		//Quit button
		if (con_.checkDown(XINPUT_GAMEPAD_START) && con_.checkDown(XINPUT_GAMEPAD_BACK))
		{
			if (con_.checkTimeHeld(XINPUT_GAMEPAD_START) > 750U && con_.checkTimeHeld(XINPUT_GAMEPAD_START) > 750U)
			{
				quit_ = true;
			}
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
	camera_->setTarget(world_->controlled());

	camera_->update(dt.asMilliseconds());
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

	window_.setView(*(camera_));

	//Render stuff

	//b2Shape* x = world_->controlled()->getVertices();
	//b2Shape::Type y = x->GetType();
	drawer_->draw();
	//world_->DrawDebugData();

	window_.display();
}