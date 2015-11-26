// Logtest.hpp
// Simple class for testing and explaining log outputs, need only be constructed

#ifndef MS_LOGTEST_H
#define MS_LOGTEST_H

#include "stdafx.h"

extern Log l;

class logtest {
private:
public:
	logtest::logtest()
	{
		char loc = 'T';
		l.out(l.priority, loc, "[TYPE][LOCATION] MESSAGE");
		l.out(l.priority, loc, "Location is a char that can be spoofed, use care");
		l.out(l.priority, loc, "Priority output can't be disabled");
		l.newLine();

		l.out(l.priority, loc, "All filters enabled by default");
		l.typeEnable(l.fatal, loc);
		l.typeEnable(l.error, loc);
		l.typeEnable(l.warning, loc);
		l.typeEnable(l.message, loc);
		l.typeEnable(l.logger, loc);
		l.newLine();

		l.out(l.fatal, loc, "Fatal problem");
		l.out(l.error, loc, "Error somwhere");
		l.out(l.warning, loc, "Warning of danger");
		l.out(l.message, loc, "Message from a thing");
		l.newLine();

		l.out(l.priority, loc, "Disable/Enable filters manually");
		l.out(l.error, loc, "Error output");
		l.typeDisable(l.error, loc);
		l.out(l.error, loc, "Error is Disabled, no output");
		l.typeEnable(l.error, loc);
		l.out(l.error, loc, "Reenabled");
		l.newLine();

		l.typeDisable(l.logger, loc);
		l.out(l.priority, loc, "logger type controls messages for internal \n\t logger notifications like type en/disable");
		l.typeEnable(l.logger, loc);

		l.separator();

		l.out(l.priority, loc, "Use newLine() to make breaks, separator() for lines");
		l.out(l.priority, loc, "Per class: extern Log l; -> en/disable filters -> log stuff");
	}
};

#endif