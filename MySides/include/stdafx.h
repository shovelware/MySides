// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
//TargetVer
#include "targetver.h"

//Default
#include <stdio.h>
#include <tchar.h>

//SFML
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#pragma comment(lib,"opensteer-d.lib")
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#pragma comment(lib,"opensteer.lib")
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

//Box2D
#pragma comment(lib, "Box2D.lib")

//Custom
#include "Log.hpp"

//Globals
const double _TICKTIME_ = 1.f / 60.f;
const double _SCALE_ = 32.f;

//Helpful functions
static double	tickToSec(unsigned int ticks) { return _TICKTIME_ * ticks; }
static float	randFloat(float MAX) { return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / MAX)); };
static float	randFloat(float MIN, float MAX) { return MIN + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (MAX - MIN))); };
static bool		coinFlip() { return ((rand() % 2) == 0); }