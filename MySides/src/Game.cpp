#include "Game.hpp"

//Globally accessible logger, usage: extern Log l;
Log l;

//inline float random functions for now
//inline float randFloat(float MAX) { return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / MAX)); };
//inline float randFloat(float MIN, float MAX) { return MIN + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (MAX - MIN))); };

Game::Game() : 
	videoMode_(720, 720, 32),
	contextSettings_(24u, 8u, 8u),
	window_(videoMode_, "My Sides!", sf::Style::Titlebar, contextSettings_),
	mousein_(false),
	quit_(false),
	fullscreen_(false),
	dd_(window_),
	voidCol_(64,64,64),
	haptics_(false),
	renderDD_(false),
	renderGAME_(true),
	renderCAM_(false),
	renderHUD_(true)
{
}

int Game::run()
{
#pragma region Startup

	//Window info
	sf::Vector2u windowSize = window_.getSize();
	window_.setVerticalSyncEnabled(true);
	
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
	camera_->loadFont("game_over.ttf");

	hud_ = new HUD(window_, world_);
	//hud_->loadFont("game_over.ttf", 36);
	hud_->loadFont("pixelmix.ttf", 12);

	//Start fullscreen
	//toggleFullscreen();

	//Start paused

	//Seed random
	srand(static_cast <unsigned> (time(0)));
	
	//Drawing
	render_ = new GameRenderer(window_, world_);
	
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
	//pause_ = true;

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
		if (accumulator >= tickTime && !quit_)
		{
			//Only update if the controller is connected
			//if (checkController(tickTime))
			{
				//Do game logic stuff
				//Update with this tick
				checkController(tickTime); //XOR with if above

				key_.update();
				handleInput(tickTime);

				update(tickTime);

				//Take this tick out of the accumulator
				accumulator -= tickTime;
			}
		}

		if (!pause_)
			world_->step(frameTime.asMilliseconds());

		render();
	}
#pragma endregion

#pragma region Shutdown

	//Exiting
	window_.close();

	//Free resources
	delete world_;
	delete render_;
	delete lptr;
	delete camera_;
	delete hud_;

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
	//Keyboard backup controls		
	//W,A,S,D : Movement
	b2Vec2 mv(0, 0);
	if (key_.isKeyDown(Key::W)) { mv.y += -1; }
	if (key_.isKeyDown(Key::S)) { mv.y += 1; }
	if (key_.isKeyDown(Key::A)) { mv.x += -1; }
	if (key_.isKeyDown(Key::D)) { mv.x += 1; }

	if (key_.isKeyDown(Key::LAlt)) { mv *= 0.25f; }

	if (mv.Length() > 0)
		world_->move(mv);

	//Arrows : Look
	b2Vec2 fr(0, 0);
	if (key_.isKeyDown(Key::Up)) { fr.y += -1; }
	if (key_.isKeyDown(Key::Down)) { fr.y += 1; }
	if (key_.isKeyDown(Key::Left)) { fr.x += -1; }
	if (key_.isKeyDown(Key::Right)) { fr.x += 1; }
	world_->look(fr);

	//Space : Fire
	if (key_.isKeyDown(Key::Space))
	{
		world_->trigger(fr);
	}

	//U,H,J,K : Camera controls
	sf::Vector2f look(0, 0);
	if (key_.isKeyDown(Key::U)) { look.y -= 10; }
	if (key_.isKeyDown(Key::H)) { look.x -= 10; }
	if (key_.isKeyDown(Key::J)) { look.y += 10; }
	if (key_.isKeyDown(Key::K)) { look.x += 10; }
	camera_->lean(look, pause_);


	//I,O,L : Camera controls
	if (key_.isKeyDown(Key::I)) { camera_->zoomIn(); }
	if (key_.isKeyDown(Key::O)) { camera_->zoomOut(); }
	if (key_.isKeyPressed(Key::L)) { camera_->zoomReset(); }

	//Escape : Quit
	if (key_.isKeyPressed(Key::Escape)) { quit_ = true; }

	//Return : Fullscreen
	if (key_.isKeyPressed(Key::Return)) { toggleFullscreen(); }

	//P : Pause
	if (key_.isKeyPressed(Key::P)) { pause_ = !pause_; }

	//F5 : Reset Level
	if (key_.isKeyPressed(Key::F5))
	{
		world_->resetWorld();
		camera_->setTarget(world_->getPlayer());
		camera_->setCenter(sf::Vector2f(0, 0));
	}

	//X : Bomb
	if (key_.isKeyPressed(Key::X))
	{
		world_->bomb();
	}

	//1234567890 : Test functions
	if (key_.isKeyPressed(Key::Num1)) { world_->f1(); }
	if (key_.isKeyPressed(Key::Num2)) { world_->f2(); }
	if (key_.isKeyPressed(Key::Num3)) { world_->f3(); }
	if (key_.isKeyPressed(Key::Num4)) { world_->f4(); }
	if (key_.isKeyPressed(Key::Num5)) { world_->f5(); }
	if (key_.isKeyPressed(Key::Num6)) { world_->f6(); }
	if (key_.isKeyPressed(Key::Num7)) { world_->f7(); }
	if (key_.isKeyPressed(Key::Num8)) { world_->f8(); }
	if (key_.isKeyPressed(Key::Num9)) { world_->f9(); }
	if (key_.isKeyPressed(Key::Num0)) { world_->f0(); }

	//Debug controls
	if (key_.isKeyDown(Key::BackSpace) || (0.5f <= con_.checkLeftTrigger() && con_.checkLeftTrigger() <= 1.f))
	{
		//Y : Toggle through rendering
		if (con_.checkPressed(XINPUT_GAMEPAD_LEFT_THUMB))
		{
			if (renderDD_ && renderGAME_)
			{
				renderGAME_ = false;
			}

			else if (renderDD_)
			{
				renderDD_ = false; 
				renderGAME_ = true;
			}

			else if (renderGAME_)
			{
				renderDD_ = true;
			}
		}

		// < : Render debugDraw
		if (key_.isKeyPressed(Key::Comma) )
		{
			renderDD_ = !renderDD_;
		}
		
		// > : Render Graphics
		if (key_.isKeyPressed(Key::Period))
		{
			renderGAME_ = !renderGAME_;
		}

		// ? : Render HUD 
		if (key_.isKeyPressed(Key::Slash))
		{
			renderHUD_ = !renderHUD_;
		}

		// : : Render Camera Info
		if (key_.isKeyPressed(Key::SemiColon))
		{
			renderCAM_ = !renderCAM_;
		}

		// Insert : Reset debug string
		if (key_.isKeyPressed(Key::Insert))
		{
			world_->dstr = "X";
			//std::cout << world_->di << " | " << world_->dstr << std::endl;
		}

		// Del : Reset debug int
		if (key_.isKeyPressed(Key::Delete))
		{
			world_->di = -1;
		}

		// RShift : Advance one step (B)
		if (key_.isKeyPressed(Key::RShift) || con_.checkPressed(XINPUT_GAMEPAD_B))
		{
			world_->step(_TICKTIME_ * 1000);
			update(sf::Time(sf::seconds(_TICKTIME_)), true);
		}

		// X : Nuke world
		if (key_.isKeyPressed(Key::X))
		{
			world_->bomb(true);
		}

		// T : Clear map
		if (key_.isKeyPressed(Key::T))
		{
			world_->clearWorld(false);
		}

		// - : Debug int decrement
		if (key_.isKeyPressed(Key::Dash)  || 
			con_.checkDown(XINPUT_GAMEPAD_DPAD_LEFT))
		{
			int time = (con_.checkTimeHeld(XINPUT_GAMEPAD_DPAD_LEFT) - 400);

			if (con_.checkPressed(XINPUT_GAMEPAD_DPAD_LEFT) ||
				(time > 0 && time % 160 == 0))
			{
				world_->di--;
			}
		}

		// + : Debug int increment
		if (key_.isKeyPressed(Key::Equal) ||
			con_.checkDown(XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			int time = (con_.checkTimeHeld(XINPUT_GAMEPAD_DPAD_RIGHT) - 400);

			if (con_.checkPressed(XINPUT_GAMEPAD_DPAD_RIGHT) ||
				(time > 0 && time % 160 == 0))
			{
				world_->di++;
			}
		}

		//G : Scrolling Weapon Select
		if (key_.isKeyPressed(Key::G) || con_.checkPressed(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			if (world_->dstr == "launcher" || world_->dstr == "fungun" || world_->dstr == "X") world_->dstr = "pistol";
			else if (world_->dstr == "pistol") world_->dstr = "rifle";
			else if (world_->dstr == "rifle") world_->dstr = "shotgun";
			else if (world_->dstr == "shotgun") world_->dstr = "cannon";
			else if (world_->dstr == "cannon") world_->dstr = "coilgun";
			else if (world_->dstr == "coilgun") world_->dstr = "railgun";
			else if (world_->dstr == "railgun") world_->dstr = "werfer";
			else if (world_->dstr == "werfer") world_->dstr = "thumper";
			else if (world_->dstr == "thumper") world_->dstr = "launcher";
			else world_->dstr == "launcher";

			if (world_->di > 8)
				world_->di = 0;
		}

		//Scrolling fun select
		if (key_.isKeyPressed(Key::F)|| con_.checkPressed(XINPUT_GAMEPAD_DPAD_UP))
		{
			world_->dstr = "fungun";

			int max = 10;
			int i[10];
			i[0] = 44;
			i[1] = 42;
			i[2] = 47;
			i[3] = 60;
			i[4] = 88;
			i[5] = 111;
			i[6] = 250;
			i[7] = 404;
			i[8] = 666;
			i[9] = 888;

			for (int w = 0; w < max; ++w)
			{
				if (w == max -1)
				{
					world_->di = i[0];
					break;
				}

				if (world_->di == i[w])
				{
					world_->di = i[w + 1];
					break;
				}
			}
		}

		//R/Y : Requisition
		if (key_.isKeyPressed(Key::R) || con_.checkPressed(XINPUT_GAMEPAD_Y))
		{

			world_->requisition(world_->getPlayer(), world_->dstr, world_->di);
		}

	}//end debug key check

	//Controller Controls
	//LS : Move
	world_->move(b2Vec2(con_.checkLeftX(), con_.checkLeftY()));

	//RS : Look
	world_->look(b2Vec2(con_.checkRightX(), con_.checkRightY()));
	if (!pause_) camera_->lean(sf::Vector2f(con_.checkRightX() * 10, con_.checkRightY() * 10), pause_);

	//RT : Trigger
	if (con_.checkRightHairTrigger())
	{
		world_->trigger(b2Vec2(con_.checkRightX(), con_.checkRightY()));
	}

	//RT && Space Released : Release Trigger
	else if (!con_.checkRightHairTrigger() && key_.isKeyUp(Key::Space))
	{
		world_->release();
	}

	//LT + RC : Bomb
	if (con_.checkLeftHairTrigger() &&
		con_.checkDown(XINPUT_GAMEPAD_RIGHT_THUMB))
	{
		world_->bomb();
	}

	//A : Testing in world/resume
	if (con_.checkPressed(XINPUT_GAMEPAD_A))
	{
		world_->f9();
	}

	//B : Testing in player
	if (con_.checkPressed(XINPUT_GAMEPAD_B))
	{
	}

	//X : Reup
	if (con_.checkPressed(XINPUT_GAMEPAD_X))
	{
		world_->reup();
	}

	//Y: Restart
	if (con_.checkPressed(XINPUT_GAMEPAD_Y))
	{
		if (pause_)
		{
			world_->resetLevel();
			camera_->setCenter(sf::Vector2f(0, 0));
		}
		//float lt = (con_.checkLeftTrigger() > 0 ? con_.checkLeftTrigger() : 0.1f);
		//int spawn = lt * 10;
		//
		//for (spawn; spawn > 0; --spawn)
		//{
		//	float x, y, rad = world_->getBoundsRadius() * 0.7f;
		//	y = -(cos((2 * M_PI) * 32 / randFloat(0, 32)));
		//	x = -(sin((2 * M_PI) * 32 / randFloat(0, 32)));
		//
		//	x *= rad;
		//	y *= rad;
		//
		//	world_->addEnemy(x, y);
		//}
	}

	//LB + RB : Reset zoom
	if (con_.checkDown(XINPUT_GAMEPAD_LEFT_SHOULDER) && con_.checkDown(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		camera_->zoomReset();
	}

	else 
	{
		//LB : 
		if (con_.checkTimeHeld(XINPUT_GAMEPAD_LEFT_SHOULDER) > 75)
		{
			camera_->zoomOut();
		}

		//RB : 
		if (con_.checkTimeHeld(XINPUT_GAMEPAD_RIGHT_SHOULDER) > 75)
		{
			camera_->zoomIn();
		}
	}

	//LC :
	if (con_.checkDown(XINPUT_GAMEPAD_LEFT_THUMB))
	{
	}

	//RC :
	if (con_.checkPressed(XINPUT_GAMEPAD_RIGHT_THUMB))
	{
	}

	//DPad : Camera Lean (Forced)
	sf::Vector2f dlook = sf::Vector2f(0, 0);
	if (con_.checkDown(XINPUT_GAMEPAD_DPAD_RIGHT))	{ dlook.x += 10; }
	if (con_.checkDown(XINPUT_GAMEPAD_DPAD_LEFT))	{ dlook.x -= 10; }
	if (con_.checkDown(XINPUT_GAMEPAD_DPAD_DOWN))	{ dlook.y += 10; }
	if (con_.checkDown(XINPUT_GAMEPAD_DPAD_UP))		{ dlook.y -= 10; }
	if (thor::length(dlook) > 0) camera_->lean(dlook, pause_);

	//Start : Pause
	if (con_.checkPressed(XINPUT_GAMEPAD_START))
	{
		pause_ = !pause_;
	}

	//Back : Quit / Reset
	if (con_.checkPressed(XINPUT_GAMEPAD_BACK))
	{
		if (pause_)
		{
			quit_ = true;
		}
	}
}

bool Game::checkController(sf::Time dt)
{
	bool connected = false;

	//If we're paused, don't update the controller's button holds
	//if (pause_)
	//{
	//	connected = con_.update(-1);
	//}

	//Normal update
	//else 
	connected = con_.update(dt.asMilliseconds());
	
	return connected;
}

void Game::update(sf::Time dt, bool force)
{
	bool hadTarget = world_->hasControlled();

	//Forced updates or if we're not paused
	if (force || !pause_)
	{
		if (haptics_)
		{
			con_.setVibrationL(world_->getHapticL());
			con_.setVibrationR(world_->getHapticR());
		}

		world_->update(dt.asMilliseconds());
	}

	//Stop vibration when paused
	else con_.stopVibration();

	bool hasTarget = world_->hasControlled();
	
	//Move the camera to controlled
	if (hasTarget)
		camera_->setTarget(world_->getControlled());

	else if (!hasTarget && hadTarget)
	{
		camera_->clearTarget(true);
	}

	camera_->update(dt.asMilliseconds());
	sf::Listener::setPosition(sf::Vector3f(camera_->getCenter().x, camera_->getCenter().y, 0));

	//End game pause
	if (world_->hasControlled() == false && !pause_)
	{
		pause_ = true;
	}

	//Update counter
	//static int x;
	//x++;
	//std::ostringstream UD;
	//UD << "Update" << x;
	//l.out(l.message, 'G', UD.str().c_str());
}

void Game::render()
{
	//Clear and set to camera
	window_.clear(voidCol_);
	window_.setView(*(camera_));

	//Render gameworld
	if (renderGAME_) { render_->render(); }

	//Debug render
	if (renderDD_) { world_->DrawDebugData(); }

	//Camera extras
	if (renderCAM_)
	{
		camera_->drawHUD();
		camera_->drawSpr3(world_->maxTime - world_->getTimeInLevel(), world_->enemies, world_->freesides);
	}

	//Draw HUD
	window_.setView(window_.getDefaultView());
	if (renderHUD_)
	{
		sf::Vector2u size = window_.getSize();

		hud_->drawLevelStatus(sf::FloatRect(20, 20, 100, 40));
		hud_->drawShapeStatus(sf::FloatRect(size.x / 2.f - 200, 25, 400, 30));
		hud_->drawSideStatus(sf::FloatRect(20, size.y - 40, size.x - 40, 20));
		hud_->drawWeaponStatus(sf::FloatRect(size.x - 120, 20, 100, 40));
		hud_->drawDebugStatus(sf::FloatRect(0, size.y - 120, size.x, 60));
	}

	//Draw pause menu
	if (pause_)
	{
		if (world_->hasControlled())
		{
			camera_->drawPause();
		}

		else camera_->drawOver(world_->hiSides, world_->hiTime);
	}

	window_.display();
}

void Game::toggleFullscreen()
{	
	if (!fullscreen_)
	{
		sf::VideoMode fsmode = sf::VideoMode::getDesktopMode();
		
		float fsh = fsmode.height;
		float fsw = fsmode.width;
		float fswf = fsh / fsw;

		window_.create(fsmode, "My Sides!", sf::Style::Fullscreen, contextSettings_);

		camera_->setViewport(sf::FloatRect((1 - fswf) / 2, 0, fswf, 1));
		camera_->updateBounds(sf::Vector2f(fsmode.width, fsmode.height));
		window_.setMouseCursorVisible(false);

		window_.display();
		fullscreen_ = true;
	}

	else if (fullscreen_)
	{
		window_.create(videoMode_, "My Sides!", sf::Style::Titlebar, contextSettings_);
		
		camera_->setViewport(sf::FloatRect(0, 0, 1, 1));
		camera_->updateBounds(sf::Vector2f(videoMode_.width, videoMode_.height));
		window_.setMouseCursorVisible(true);

		window_.display();
		fullscreen_ = false;
	}
}
