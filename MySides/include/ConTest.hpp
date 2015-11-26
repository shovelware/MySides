// ConTest.hpp
// Simple class for testing controller input, 
// check after updating the controller outside

#ifndef MS_CONTEST_HPP
#define MS_CONTEST_HPP

#include "stdafx.h"
#include "XController.hpp"

#include <sstream>
extern Log l;

class ConTest {
private:
	XController con_;
public:
	ConTest(XController con) : con_(con)
	{

	}

	void check(int milliseconds)
	{
		if (con_.checkDown(XINPUT_GAMEPAD_A))
		{
			l.out(l.message, 'G', "A Button");
		}

		if (con_.checkDown(XINPUT_GAMEPAD_B))
		{
			l.out(l.message, 'G', "B Button");
		}

		if (con_.checkDown(XINPUT_GAMEPAD_X))
		{
			l.out(l.message, 'G', "X Button");
		}

		if (con_.checkDown(XINPUT_GAMEPAD_Y))
		{
			l.out(l.message, 'G', "Y Button");
		}

		if (con_.checkDown(XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			l.out(l.message, 'G', "Left Bumper");
		}

		if (con_.checkDown(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			l.out(l.message, 'G', "Right Bumper");
		}

		if (con_.checkDown(XINPUT_GAMEPAD_LEFT_THUMB))
		{
			l.out(l.message, 'G', "Left Click");
		}

		if (con_.checkDown(XINPUT_GAMEPAD_RIGHT_THUMB))
		{
			l.out(l.message, 'G', "Right Click");
		}

		if (con_.checkDPadX() != 0)
		{
			std::ostringstream DP;
			DP << "DPad X " << con_.checkDPadX();
			l.out(l.message, 'G', DP.str().c_str());
		}

		if (con_.checkDPadY() != 0)
		{
			std::ostringstream DP;
			DP << "DPad Y " << con_.checkDPadY();
			l.out(l.message, 'G', DP.str().c_str());
		}

		if (con_.checkPressed(XINPUT_GAMEPAD_BACK))
		{
			l.out(l.message, 'G', "Back Button");
			l.typeDisable(l.message, 'G');
		}

		if (con_.checkPressed(XINPUT_GAMEPAD_START))
		{
			l.out(l.message, 'G', "Start Button");
			l.typeEnable(l.message, 'G');
		}

		if (con_.checkReleased(XINPUT_GAMEPAD_A))
		{
			if (con_.checkTimeHeld(XINPUT_GAMEPAD_A) > 0)
			{
				std::ostringstream HT;
				HT << con_.checkTimeHeld(XINPUT_GAMEPAD_A);
				l.out(l.message, 'G', HT.str().c_str());
			}
		}

		if (con_.checkLeftTrigger() > .5f)
		{
			std::ostringstream LS;
			LS << con_.checkLeftTrigger() << "\n" << "LX " << con_.checkLeftX() << "\n" << "LY " << con_.checkLeftY();
			l.out(l.message, 'G', LS.str().c_str());
		}

		if (con_.checkRightTrigger() > .5f)
		{
			std::ostringstream RS;
			RS << con_.checkRightTrigger() << "\n" << "RX " << con_.checkRightX() << "\n" << "RY " << con_.checkRightY();
			l.out(l.message, 'G', RS.str().c_str());
		}
	}
};

#endif