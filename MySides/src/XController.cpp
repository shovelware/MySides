#include "XController.hpp"

XController::XController() :
	deadzoneLX_(0.05f), deadzoneLY_(0.02f), 
	deadzoneRX_(0.05f), deadzoneRY_(0.02f), 
	thresholdLT_(0.5f), thresholdRT_(0.5f),
	controllerId_(-1)
{
	initButtons();
}

#pragma region Init

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

//Checks for any controller connected
//uses the first one found
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

#pragma endregion

int XController::getPort() const
{
	return controllerId_ + 1;
}

#pragma region Button checks

bool XController::isDown(WORD button) const
{
	return (curState_.Gamepad.wButtons & button);
}

bool XController::isUp(WORD button) const
{
	return (curState_.Gamepad.wButtons ^ button);
}

bool XController::isPressed(WORD button) const
{
	return ((curState_.Gamepad.wButtons & button) && (prvState_.Gamepad.wButtons & ~button));
}

#pragma endregion

bool XController::update(int milliseconds)
{
	//If we're not connected, try to connect
	if (controllerId_ == DISCONNECTED)
	{
		checkConnection();
	}

	//Otherwise we're good to update
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

		//Now we can start checking things

		//Update Button held times
		for (std::map<WORD, int>::iterator mIter = heldTimes_.begin(), mEnd = heldTimes_.end(); mIter != mEnd; ++mIter)
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

		//Return update success, we're still connected
		return true;
	}

	//We couldn't connect or update, something's wrong
	return false;
}

