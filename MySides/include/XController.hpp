// XController.hpp
// Detects and reports Xbox controller input with XInput
// Usage: Declare, update(milliseconds), ask for buttons
// Heavily influenced by code from https://katyscode.wordpress.com/2013/08/30/xinput-tutorial-part-1-adding-gamepad-support-to-your-windows-game/

#ifndef XCONTROLLER_HPP
#define XCONTROLLER_HPP

#include "stdafx.h"

//Include xinput
#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && !defined(_ARM_) && defined(_M_IX86)
#define _X86_
#endif

#include <Xinput.h>
#pragma comment(lib, "XInput9_1_0.lib")

//maps for timing
#include <map>
#include <iterator>

class XController {
public:
	XController();

	int getPort() const;

	//Buttons
	bool checkDown(WORD button) const;
	bool checkUp(WORD button) const;

	bool checkPressed(WORD button) const;
	bool checkReleased(WORD button) const;

	bool checkHeld(WORD button) const;

	unsigned int checkTimeHeld(WORD button) const;

	//Sticks
	float checkLeftX() const;
	float checkLeftY() const;
	float checkRightX() const;
	float checkRightY() const;

	bool checkLeftNeutral() const;
	bool checkRightNeutral() const;

	//float leftdX() const;
	//float leftdY() const;
	//float rightdX() const;
	//float rightdY() const;

	//Dpad
	int checkDPadX() const;
	int checkDPadY() const;

	//Triggers
	float checkLeftTrigger() const;
	float checkRightTrigger() const;

	bool checkLeftHairTrigger() const;
	bool checkRightHairTrigger() const;

	//Accessors
	bool setDeadzoneLX(float deadzone);
	bool setDeadzoneLY(float deadzone);
	bool setDeadzoneRX(float deadzone);
	bool setDeadzoneRY(float deadzone);
	bool setThresholdLT(float threshold);
	bool setThresholdRT(float threshold);

	float getDeadzoneLX() const;
	float getDeadzoneLY() const;
	float getDeadzoneRX() const;
	float getDeadzoneRY() const;
	float getThresholdLT() const;
	float getThresholdRT() const;

	//Update
	bool update(int milliseconds);

	//bool addVibration(float balance, float amount%, int milliseconds);

	//short absLeftX() const;
	//short absLeftY() const;
	//short absLeftdX() const;
	//short absRightdX() const;
	//
	//short absRightX() const;
	//short absRightY() const;
	//short absRightdX() const;
	//short absRightdY() const;
	//
	//short absLeftTrigger() const;
	//short absRightTrigger() const;
private:
	int controllerId_;
	const int DISCONNECTED = -1;

	XINPUT_STATE curState_;
	XINPUT_STATE prvState_;

	std::map<WORD, unsigned int> heldTimes_;

	float deadzoneLX_, deadzoneLY_;
	float deadzoneRX_, deadzoneRY_;
	const float DEADZONE_MAX = 1.f;

	float thresholdLT_, thresholdRT_;
	const float THRESHOLD_MAX = 1.f;
	
	float leftX_, leftY_;
	float rightX_, rightY_;
	const short STICK_MAX = 32767;

	float leftTrigger_, rightTrigger_;
	const BYTE TRIGGER_MAX = 255;

	//unsigned short leftVibe, rightVibe;
	//const unsigned short VIBE_MAX = USHRT_MAX;

	void initButtons();
	bool checkConnection();
};

#endif