// XController.hpp
// Detects and reports Xbox controller input with XInput
// Heavily influenced by code from https://katyscode.wordpress.com/2013/08/30/xinput-tutorial-part-1-adding-gamepad-support-to-your-windows-game/

#ifndef XCONTROLLER_H
#define XCONTROLLER_H

//Include xinput
#include <Xinput.h>
#pragma comment(lib, "XInput9_1_0.lib")

//maps for timing
#include <map>
#include <iterator>

class XController {
private:
	int controllerId_;
	const int DISCONNECTED = -1;

	XINPUT_STATE curState_;
	XINPUT_STATE prvState_;

	std::map<WORD, unsigned int> heldTimes_;

	float deadzoneLX_, deadzoneLY_;
	float deadzoneRX_, deadzoneRY_;
	float thresholdLT_, thresholdRT_;
	
	float leftX_, leftY_;
	float rightX_, rightY_;
	const short STICK_MAX = SHRT_MAX;

	float leftTrigger_, rightTrigger_;
	const BYTE TRIGGER_MAX = 255;

	//unsigned short leftVibe, rightVibe;
	//const unsigned short VIBE_MAX = USHRT_MAX;

	void initButtons();
	bool checkConnection();
public:
	XController();
	int getPort() const;

	bool isDown(WORD button) const;
	bool isUp(WORD button) const;

	bool isPressed(WORD button) const;
	bool isReleased(WORD button) const;

	bool isHeld(WORD button) const;

	unsigned int timeHeld(WORD button) const;

	float leftX() const;
	float leftY() const;
	//float leftdX() const;
	//float leftdY() const;

	float rightX() const;
	float rightY() const;
	//float rightdX() const;
	//float rightdY() const;
	
	bool setDeadzoneLX(float deadzone);
	bool setDeadzoneLY(float deadzone);
	bool setDeadzoneRX(float deadzone);
	bool setDeadzoneRY(float deadzone);

	int dPadX() const;
	int dPadY() const;

	float leftTrigger() const;
	float rightTrigger() const;

	bool leftTriggerDown() const;
	bool rightTriggerDown() const;

	bool setThresholdLT(float threshold);
	bool setThresholdRT(float threshold);

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
};

#endif