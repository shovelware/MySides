#include "XController.hpp"

XController::XController() :
	deadzoneLX_(0.15f), deadzoneLY_(0.15f),
	deadzoneRX_(0.15f), deadzoneRY_(0.15f),
	thresholdLT_(0.4f), thresholdRT_(0.4f),
	controllerId_(-1)
{
	initButtons();
}

//Initialises the heldtimes map with all controller buttons
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
	//We assume we're disconnected
	int id = DISCONNECTED;

	//Step through all controller connection possibilities
	for (DWORD i = 0; i < XUSER_MAX_COUNT && id == -1; ++i)
	{
		//create and flush a state to test
		XINPUT_STATE state;
		memset(&state, 0, sizeof(XINPUT_STATE));

		//Try to check the state, only succeeds if there's a controller there
		if (XInputGetState(i, &state) == 0x0)
		{
			id = i;
		}
	}

	controllerId_ = id;

	return controllerId_ != DISCONNECTED;
}

//Returns port number of controller
int XController::getPort() const
{
	return controllerId_ + 1;
}

#pragma region Buttons

//Returns if a button is down at all
bool XController::checkDown(WORD button) const
{
	//cur down
	return (curState_.Gamepad.wButtons & button);
}

//Returns if a button is up at all
bool XController::checkUp(WORD button) const
{
	//cur up
	return ((curState_.Gamepad.wButtons & button) == false);
}

//Returns if a button was just pressed
bool XController::checkPressed(WORD button) const
{
	//cur down
	//prv up
	return ((curState_.Gamepad.wButtons & button) && ((prvState_.Gamepad.wButtons & button) == false));
}

//Returns if a buttons was just released
bool XController::checkReleased(WORD button) const
{
	//cur up
	//prv down
	return ((curState_.Gamepad.wButtons & button) == 0 && (prvState_.Gamepad.wButtons & button));
}

//Returns if a button is being held
bool XController::checkHeld(WORD button) const
{
	//cur down
	//prv down
	return ((curState_.Gamepad.wButtons & button) && (prvState_.Gamepad.wButtons & button));
}

//Returns the time a button was held in milliseconds, or NULL if button doesn't exist
unsigned int XController::checkTimeHeld(WORD button) const
{
	//Find the button in the map
	std::map<WORD, unsigned int>::const_iterator iter(heldTimes_.find(button));

	//If it doesn't exist, no time
	if (iter == heldTimes_.end())
	{
		return NULL;
	}

	//else return time
	else return iter->second;
	
}

#pragma endregion

#pragma region Sticks

//Returns Left Stick X axis between left -1f and 1f right
float XController::checkLeftX() const {	return leftX_; }

//Returns Left Stick Y axis between up -1f and 1f down
float XController::checkLeftY() const {	return -leftY_; }

//Returns Right Stick X axis between left -1f and 1f right
float XController::checkRightX() const { return rightX_; }

//Returns Right Stick Y axis between up -1f and 1f down
float XController::checkRightY() const { return -rightY_; }

//Returns true if Right Stick is neutral, factoring in deadzones
bool XController::checkLeftNeutral() const
{
	return (leftX_ == 0 && leftY_ == 0);
}

//Returns true if Left Stick is neutral, factoring in deadzones
bool XController::checkRightNeutral() const
{
	return (rightX_ == 0 && rightY_ == 0);
}

#pragma endregion

#pragma region Dpad

//Returns x axis of Dpad, -1 is left, 0 is nothing, 1 is right
int XController::checkDPadX() const
{
	int left = -1 * (bool)(curState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
	int right = (bool)(curState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);

	return left + right;
}

//Returns y axis of Dpad, -1 is up, 0 is nothing, 1 is down
int XController::checkDPadY() const
{
	int up = -1 * (bool)(curState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
	int down = (bool)(curState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);

	return up + down;
}

#pragma endregion

#pragma region Triggers

//Returns Left Trigger state between 0f and 1f
float XController::checkLeftTrigger() const { return leftTrigger_; }

//Returns Right Trigger state between 0f and 1f
float XController::checkRightTrigger() const{ return rightTrigger_; }

//Returns if Left Trigger is further than threshold
bool XController::checkLeftHairTrigger() const { return (leftTrigger_ > (1 * thresholdLT_)); }

//Returns if Right Trigger is further than threshold
bool XController::checkRightHairTrigger() const { return (rightTrigger_ > (1 * thresholdRT_)); }

#pragma endregion

#pragma region Accessors

//Sets Left Stick X axis deadzone, between 0f and .99f, returns success
bool XController::setDeadzoneLX(float deadzone)
{
	deadzoneLX_ = ((0.f < deadzone && deadzone < DEADZONE_MAX) ? deadzone : deadzoneLX_);
	return (deadzoneLX_ == deadzone);
}

//Sets Left Stick Y axis deadzone, between 0f and .99f, returns success
bool XController::setDeadzoneLY(float deadzone)
{
	deadzoneLY_ = ((0.f < deadzone && deadzone < DEADZONE_MAX) ? deadzone : deadzoneLY_);
	return (deadzoneLY_ == deadzone);
}

//Sets Right Stick X axis deadzone, between 0f and .99f, returns success
bool XController::setDeadzoneRX(float deadzone)
{
	deadzoneRX_ = ((0.f < deadzone && deadzone < DEADZONE_MAX) ? deadzone : deadzoneRX_);
	return (deadzoneRX_ == deadzone);
}

//Sets Right Stick Y axis deadzone, between 0f and .99f, returns success
bool XController::setDeadzoneRY(float deadzone)
{
	deadzoneRY_ = ((0.f < deadzone && deadzone < DEADZONE_MAX) ? deadzone : deadzoneRY_);
	return (deadzoneRY_ == deadzone);
}

//Sets threshold of Left Trigger, between 0f and .99f, returns success
bool XController::setThresholdLT(float threshold)
{
	thresholdLT_ = ((0.f < threshold && threshold < THRESHOLD_MAX) ? threshold : thresholdLT_);
	return (thresholdLT_ == threshold);
}

//Sets threshold of Right Trigger, between 0f and .99f, returns success
bool XController::setThresholdRT(float threshold)
{
	thresholdRT_ = ((0.f < threshold && threshold < THRESHOLD_MAX) ? threshold : thresholdRT_);
	return (thresholdRT_ == threshold);
}

//Gets Left Stick X axis deadzone
float XController::getDeadzoneLX() const { return deadzoneLX_; }

//Gets Left Stick Y axis deadzone
float XController::getDeadzoneLY() const { return deadzoneLY_; }

//Gets Right Stick X axis deadzone
float XController::getDeadzoneRX() const { return deadzoneRX_; }

//Gets Right Stick Y axis deadzone
float XController::getDeadzoneRY() const { return deadzoneRY_; }

//Gets Left Trigger threshold
float XController::getThresholdLT() const { return thresholdLT_; }

//Gets Right Trigger threshold
float XController::getThresholdRT() const { return thresholdRT_; }

#pragma endregion

//Checks connection, updates controller states, returns connection state
//Negative time indicates rollover of held time tracking, but we still want input
//Use Case: paused game, but still have menu control
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
		if (XInputGetState(controllerId_, &curState_) != 0x0)
		{
			controllerId_ = DISCONNECTED;
			return false;
		}

		//Now we can start checking things
		//Button held times update
		for (std::map<WORD, unsigned int>::iterator bIter = heldTimes_.begin(), bEnd = heldTimes_.end(); bIter != bEnd; ++bIter)
		{
			//If a button is down, add the held time
			if (curState_.Gamepad.wButtons & bIter->first)
			{
				//But only if the time > 0, otherwise leave it as is
				bIter->second = bIter->second + (milliseconds > 0 ? milliseconds : 0);
			}

			//If a button was up and it was up last update, clear the time
			else if ((prvState_.Gamepad.wButtons & bIter->first) == 0)
			{
				//But only if time > 0, otherwise we should keep what we have
				if (milliseconds > 0)
				{
					bIter->second = 0;
				}
			}
		}
		//for (std::pair<const WORD, int> b : heldTimes_)
		//{
		//	//If a button is down, add the held time
		//	if (curState_.Gamepad.wButtons & b.first)
		//	{
		//		//But only if the time > 0, otherwise leave it as is
		//		b.second = b.second + (milliseconds > 0 ? milliseconds : 0);
		//	}
		//		
		//	//If a button was up and it was up last update, clear the time
		//	else if ((prvState_.Gamepad.wButtons & b.first) == 0)
		//	{
		//		//But only if time > 0, otherwise we should keep what we have
		//		if (milliseconds > 0)
		//		{
		//			b.second = 0;
		//		}
		//	}
		//}//end button update

		//Left Stick Update
		//Get normalised stick position
		float normLY = fmaxf(-1, (float)curState_.Gamepad.sThumbLY / STICK_MAX);
		float normLX = fmaxf(-1, (float)curState_.Gamepad.sThumbLX / STICK_MAX);
		
		//Get position betweeen deadzone and edge
		leftX_ = (abs(normLX) < deadzoneLX_ ? 0 : (abs(normLX) - deadzoneLX_) * (normLX / abs(normLX)));
		leftY_ = (abs(normLY) < deadzoneLY_ ? 0 : (abs(normLY) - deadzoneLY_) * (normLY / abs(normLY)));
		
		//Scale position to between -1 and 1
		if (deadzoneLX_ > 0) leftX_ *= 1 / (DEADZONE_MAX - deadzoneLX_);
		if (deadzoneLY_ > 0) leftY_ *= 1 / (DEADZONE_MAX - deadzoneLY_);

		//Right Stick Update
		float normRY = fmaxf(-1, (float)curState_.Gamepad.sThumbRY / STICK_MAX);
		float normRX = fmaxf(-1, (float)curState_.Gamepad.sThumbRX / STICK_MAX);
		rightX_ = (abs(normRX) < deadzoneRX_ ? 0 : (abs(normRX) - deadzoneRX_) * (normRX / abs(normRX)));
		rightY_ = (abs(normRY) < deadzoneRY_ ? 0 : (abs(normRY) - deadzoneRY_) * (normRY / abs(normRY)));
		if (deadzoneRX_ > 0) rightX_ *= 1 / (DEADZONE_MAX - deadzoneRX_);
		if (deadzoneRY_ > 0) rightY_ *= 1 / (DEADZONE_MAX - deadzoneRY_);

		//Trigger update
		leftTrigger_ = (float)curState_.Gamepad.bLeftTrigger / TRIGGER_MAX;
		rightTrigger_ = (float)curState_.Gamepad.bRightTrigger / TRIGGER_MAX;

		//Return update success, we're still connected
		return true;

	}//end else do actual update

	//We couldn't connect or update, something's wrong
	return false;
}