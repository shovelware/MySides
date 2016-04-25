// Game.hpp
// Handles loops, game logic, box2d world

#ifndef MS_GAME_HPP
#define MS_GAME_HPP

#ifndef _DEBUG
#define NDEBUG
#endif

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
#include "Renderer.hpp"
#include "GameRenderer.hpp"
#include "HUD.hpp"

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
	void update(sf::Time dt, bool force = false);
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
	GameRenderer* render_;

	//Input management
	bool checkController(sf::Time dt);
	XController con_;
	
	//Keyboard
	KeyInput key_;

	//Game world 
	GameWorld* world_;

	//Camera
	Camera* camera_;

	//HUD
	HUD* hud_;
	sf::FloatRect levelBarBox_;
	sf::FloatRect shapeBarBox_;
	sf::FloatRect sideBarBox_;
	sf::FloatRect weaponBarBox_;
	sf::FloatRect debugBox_;
	sf::FloatRect levelInfoBox_;
	sf::FloatRect screenSizeBox_;



	//DebugDraw
	SFMLDebugDraw dd_;
	bool renderDD_;
	bool renderGAME_;
	bool renderHUD_;
	bool renderCAM_;

	bool haptics_;

	//Testing
	bool debug_;
};
#endif