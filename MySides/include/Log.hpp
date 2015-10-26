//Log.hpp
// Globally accessibly cout for logging and debugging
// F - FATAL	W - WARNING		! - PRIORITY
// E - ERROR	M - MESSAGE		I - LOGGER
// FEWM can be toggled on and off, I is internal messages that
// may become overly verbose. ! can't be stopped.

#ifndef LOG_H
#define LOG_H

#include <iostream>

using std::cout;
using std::endl;

class Log {
private:
	enum TYPEFLAG {
		FATAL = 0x01,
		ERROR = 0x02,
		WARNING = 0x04,
		MESSAGE = 0x08,
		//0x10
		//0x20
		//0x40
		LOGGER = 0x80
	};

	//Bit flags for output, all enabled by default ctor
	unsigned int options_;

public:
	Log() : options_(TYPEFLAG::FATAL | TYPEFLAG::ERROR | TYPEFLAG::WARNING | TYPEFLAG::MESSAGE | TYPEFLAG::LOGGER) {}

	//For use in other locations
	enum messagetype {
		fatal = 'F',
		error = 'E',
		warning = 'W',
		message = 'M',
		logger = 'I',
		priority = '!'
	};

	void out(char type, char location, char* output) const
	{
		switch (type)
		{
		case messagetype::fatal:
			if (options_ & TYPEFLAG::FATAL)
			{
				cout << "[" << (char)fatal << "][" << location << "] " << output << endl;
			}
			break;

		case messagetype::error:
			if (options_ & TYPEFLAG::ERROR)
			{
				cout << "[" << (char)error << "][" << location << "] " << output << endl;
			}
			break;

		case messagetype::warning:
			if (options_ & TYPEFLAG::WARNING)
			{
				cout << "[" << (char)warning << "][" << location << "] " << output << endl;
			}
			break;

		case messagetype::message:
			if (options_ & TYPEFLAG::MESSAGE)
			{
				cout << "["<< (char)message <<"][" << location << "] " << output << endl;
			}
			break;

		case messagetype::logger:
			if (options_ & TYPEFLAG::LOGGER)
			{
				cout << "[" << (char)logger << "][" << location << "] " << output << endl;
			}
			break;

		case messagetype::priority:
			cout << "[" << (char)priority <<"][" << location << "] " << output << endl;
			break;

		default:
			cout << "[X][" << location << "] " << "INVALID LOGGER OUTPUT TYPE" << endl;
			break;
		}
	}
	
	void newLine() const
	{
		cout << endl;
	}

	void separator() const
	{
		cout << endl << "==== \t ==== \t ==== \t ==== \t" << endl << endl;
	}

	void typeEnable(char type, char location)
	{
		switch (type)
		{
		case messagetype::fatal:
			options_ |= TYPEFLAG::FATAL;
			out(logger, location , "FATAL ENABLED");
			break;

		case messagetype::error:
			options_ |= TYPEFLAG::ERROR;
			out(logger, location, "ERROR ENABLED");
			break;

		case messagetype::warning:
			options_ |= TYPEFLAG::WARNING;
			out(logger, location, "WARNING ENABLED");
			break;

		case messagetype::message:
			options_ |= TYPEFLAG::MESSAGE;
			out(logger, location, "MESSAGE ENABLED");
			break;

		case messagetype::logger:
			options_ |= TYPEFLAG::LOGGER;
			out(logger, location, "LOGGER MESSAGES ENABLED");
			break;

		default:
			out(logger, location, "INVALID TYPE TO ENABLE");
			break;
		}

	}

	void typeDisable(char type, char location)
	{
		switch (type)
		{
		case messagetype::fatal:
			options_ &= ~TYPEFLAG::FATAL;
			out(logger, location, "FATAL DISABLED");
			break;

		case messagetype::error:
			options_ &= ~TYPEFLAG::ERROR;
			out(logger, location, "ERROR DISABLED");
			break;

		case messagetype::warning:
			options_ &= ~TYPEFLAG::WARNING;
			out(logger, location, "WARNING DISABLED");
			break;

		case messagetype::message:
			options_ &= ~TYPEFLAG::MESSAGE;
			out(logger, location, "MESSAGE DISABLED");
			break;

		case messagetype::logger:
			out(logger, location, "LOGGER MESSAGES DISABLED");
			options_ &= ~TYPEFLAG::LOGGER;
			break;

		default:
			out(priority, location, "INVALID TYPE TO DISABLE");
			break;
		}
	}
};

#endif