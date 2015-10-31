#include "XController.hpp"

XController::XController() :
	deadzoneLX_(0.05f), deadzoneLY_(0.02f), 
	deadzoneRX_(0.05f), deadzoneRY_(0.02f), 
	thresholdLT_(0.4f), thresholdRT_(0.4f),
	controllerId_(-1)
{
	initButtons();
}

//initialises the heldtimes map with all controller buttons
void XController::initButtons()
{
	heldTimes_.emplace(XINPUT_GAMEPAD_DPAD_UP, 0);
	heldTimes_.emplace(XINPUT_GAMEPAD_DPAD_DOWN, 0);
	heldTimes_.emplace(XINPUT_GAMEPAD_DPAD_LEFT, 0);
	heldTimes_.emplace(XINPUT_GAMEPAD_DPAD_RIGHT, 0);

	heldTimes_.emplace(XINPUT_GAMEPAD_START, 0);
	heldTimes_.emplace(XINPUT_GAMEPAD_BACK, 0);

	heldTimes_.emplace(XINPUT_GAMEPAD_LEFT_THUMB, 0);
	heldTimes_.emplace(XINPUT_GAMEPAD_RIGHT_THUMB, 0);

	heldTimes_.emplace(XINPUT_GAMEPAD_LEFT_SHOULDER, 0);
	heldTimes_.emplace(XINPUT_GAMEPAD_RIGHT_SHOULDER, 0);

	heldTimes_.emplace(XINPUT_GAMEPAD_A, 0);
	heldTimes_.emplace(XINPUT_GAMEPAD_B, 0);
	heldTimes_.emplace(XINPUT_GAMEPAD_X, 0);
	heldTimes_.emplace(XINPUT_GAMEPAD_Y, 0);
}

//Checks for any controller connected and uses the first one found
bool XController::checkConnection()
{
	//-1 means disconnected
	int id = DISCONNECTED;

	for (int i = 0; i < XUSER_MAX_COUNT && id == -1; ++i)
	{
		//create and flush a state to test
		XINPUT_STATE state;
		memset(&state, 0, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &state) == 0);
		{
			id = i;
		}

		controllerId_ = id;

		return controllerId_ != DISCONNECTED;
	}
}

//Returns port number of controller
int XController::getPort() const
{
	return controllerId_ + 1;
}

#pragma region Buttons

//Returns if a button is down at all
bool XController::isDown(WORD button) const
{
	//cur down
	return (curState_.Gamepad.wButtons & button);
}

//Returns if a button is up
bool XController::isUp(WORD button) const
{
	//cur up
	return ((curState_.Gamepad.wButtons & button) == false);
}

//Returns if a button was just pressed
bool XController::isPressed(WORD button) const
{
	//cur down
	//prv up
	return ((curState_.Gamepad.wButtons & button) && ((prvState_.Gamepad.wButtons & button) == false));
}

//Returns if a buttons was just released
bool XController::isReleased(WORD button) const
{
	//cur up
	//prv down
	return ((curState_.Gamepad.wButtons & button) == 0 && (prvState_.Gamepad.wButtons & button));
}

//Returns if a button is being held
bool XController::isHeld(WORD button) const
{
	//cur down
	//prv down
	return ((curState_.Gamepad.wButtons & button) && (prvState_.Gamepad.wButtons & button));
}

unsigned int XController::timeHeld(WORD button) const
{
	//Find the button in the map
	std::map<WORD, unsigned int>::const_iterator iter(heldTimes_.find(button));

	//If it doesn't exist, no time
	if (iter == heldTimes_.end())
	{
		return 0;
	}

	//else return time
	else return iter->second;
	
}

#pragma endregion

#pragma region Sticks

//Returns Left Stick X axis between -100f and 100f
float XController::leftX() const
{
	return leftX_;
}

//Returns Left Stick Y axis between -100f and 100f
float XController::leftY() const
{
	return leftY_;
}

//Returns Right Stick X axis between -100f and 100f
float XController::rightX() const
{
	return rightX_;
}

//Returns Right Stick Y axis between -100f and 100f
float XController::rightY() const
{
	return rightY_;
}

//Sets Left Stick X axis deadzone, between 0f and .99f, returns success
bool XController::setDeadzoneLX(float deadzone)
{
	deadzoneLX_ = ((0.f < deadzone && deadzone < .99f) ? deadzone : deadzoneLX_);

	return (deadzoneLX_ == deadzone);
}

//Sets Left Stick Y axis deadzone, between 0f and .99f, returns success
bool XController::setDeadzoneLY(float deadzone)
{
	deadzoneLY_ = ((0.f < deadzone && deadzone < .99f) ? deadzone : deadzoneLY_);

	return (deadzoneLY_ == deadzone);
}

//Sets Right Stick X axis deadzone, between 0f and .99f, returns success
bool XController::setDeadzoneRX(float deadzone)
{
	deadzoneRX_ = ((0.f < deadzone && deadzone < .99f) ? deadzone : deadzoneRX_);

	return (deadzoneRX_ == deadzone);
}

//Sets Right Stick Y axis deadzone, between 0f and .99f, returns success
bool XController::setDeadzoneRY(float deadzone)
{
	deadzoneRY_ = ((0.f < deadzone && deadzone < .99f) ? deadzone : deadzoneRY_);

	return (deadzoneRY_ == deadzone);
}

#pragma endregion

#pragma region Dpad

//Returns x axis of Dpad, -1 is left, 0 is nothing, 1 is right
int XController::dPadX() const
{
	int left = -1 * (curState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
	int right = (curState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);

	return left + right;
}

//Returns y axis of Dpad, -1 is up, 0 is nothing, 1 is down
int XController::dPadY() const
{
	int up = -1 * (curState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
	int down = (curState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);

	return up + down;
}

#pragma endregion

#pragma region Triggers

//Returns Left Trigger state between 0f and 100f
float XController::leftTrigger() const
{
	return leftTrigger_;
}

//Returns Right Trigger state between 0f and 100f
float XController::rightTrigger() const
{
	return rightTrigger_;
}

//Returns if Left Trigger is further than threshold
bool XController::leftTriggerDown() const
{
	return (leftTrigger_ > (100 * thresholdLT_));
}

//Returns if Right Trigger is further than threshold
bool XController::rightTriggerDown() const
{
	return (rightTrigger_ > (100 * thresholdRT_));
}

//Set threshold of Left Trigger, between 0f and .99f, returns success
bool XController::setThresholdLT(float threshold)
{
	thresholdLT_ = ((0.f < threshold && threshold < .99f) ? threshold : thresholdLT_);

	return (thresholdLT_ == threshold);
}

//Set threshold of Right Trigger, between 0f and .99f, returns success
bool XController::setThresholdRT(float threshold)
{
	thresholdRT_ = ((0.f < threshold && threshold < .99f) ? threshold : thresholdRT_);

	return (thresholdRT_ == threshold);
}

#pragma endregion

//Checks connection, updates controller states, returns connection state
bool XController::update(int milliseconds)
{
	//If we're not connected, try to connect
	if (controllerId_ == DISCONNECTED)
	{
		checkConnection();
	}

	//Else we're connected and good to update
	else
	{
		//Update our previous state
		memcpy(&prvState_, &curState_, sizeof(XINPUT_STATE));

		//Flush our current state
		memset(&curState_, 0, sizeof(XINPUT_STATE));

		//Update our current state and check if we're still good
		if (XInputGetState(controllerId_, &curState_) != 0)
		{
			controllerId_ = -1;
			return false;
		}

		//Now we can start checking things, but only if there's been a change
		if (curState_.dwPacketNumber != prvState_.dwPacketNumber)
		{
			//Button held times update
			for (std::map<WORD, unsigned int>::iterator mIter = heldTimes_.begin(), mEnd = heldTimes_.end(); mIter != mEnd; ++mIter)
			{
				//If a button is down, add the held time
				if (curState_.Gamepad.wButtons & mIter->first)
				{
					mIter->second += milliseconds;
				}

				//If a button was up and it was up last update, clear the time
				else if (prvState_.Gamepad.wButtons ^ mIter->first)
				{
					mIter->second = 0;
				}
			}//end button update

			//Left Stick Update
			//Get normalised stick position
			float normLY = fmaxf(-1, (float)curState_.Gamepad.sThumbLY / STICK_MAX);
			float normLX = fmaxf(-1, (float)curState_.Gamepad.sThumbLX / STICK_MAX);
			//Apply deadzones
			if (deadzoneLX_ > 0) leftX_ *= 1 / (1 - deadzoneLX_);
			if (deadzoneLY_ > 0) leftY_ *= 1 / (1 - deadzoneLY_);

			//Right Stick Update
			float normRY = fmaxf(-1, (float)curState_.Gamepad.sThumbRY / STICK_MAX);
			float normRX = fmaxf(-1, (float)curState_.Gamepad.sThumbRX / STICK_MAX);
			if (deadzoneRX_ > 0) rightX_ *= 1 / (1 - deadzoneRX_);
			if (deadzoneRY_ > 0) rightY_ *= 1 / (1 - deadzoneRY_);

			//Trigger update
			leftTrigger_ = (float)curState_.Gamepad.bLeftTrigger / TRIGGER_MAX;
			rightTrigger_ = (float)curState_.Gamepad.bRightTrigger / TRIGGER_MAX;
		}//end if there's a change
	}//end else do actual update

	//Return update success, we're still connected
	return true;
	

	//We couldn't connect or update, something's wrong
	return false;
}

