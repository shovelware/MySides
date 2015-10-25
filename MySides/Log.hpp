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
	const enum TYPEFLAG {
		FATAL = 0x01,
		ERROR = 0x02,
		WARNING = 0x04,
		MESSAGE = 0x08,
		//0x10
		//0x20
		//0x40
		LOGGER = 0x80
	};

	//Bit flags for output, all enabled by default
	unsigned int options_ = FATAL | ERROR | WARNING | MESSAGE | LOGGER;

public:
	Log() {}

	//For use in other locations
	enum Messagetype_ {
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
		case fatal:
			if (options_ & FATAL)
			{
				cout << "[" << (char)fatal << "][" << location << "] " << output << endl;
			}
			break;

		case error:
			if (options_ & ERROR)
			{
				cout << "[" << (char)error << "][" << location << "] " << output << endl;
			}
			break;

		case warning:
			if (options_ & WARNING)
			{
				cout << "[" << (char)warning << "][" << location << "] " << output << endl;
			}
			break;

		case message:
			if (options_ & MESSAGE)
			{
				cout << "["<< (char)message <<"][" << location << "] " << output << endl;
			}
			break;

		case logger:
			if (options_ & LOGGER)
			{
				cout << "[" << (char)logger << "][" << location << "] " << output << endl;
			}
			break;

		case priority:
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
		case fatal:
			options_ |= FATAL;
			out(logger, location , "FATAL ENABLED");
			break;

		case error:
			options_ |= ERROR;
			out(logger, location, "ERROR ENABLED");
			break;

		case warning:
			options_ |= WARNING;
			out(logger, location, "WARNING ENABLED");
			break;

		case message:
			options_ |= MESSAGE;
			out(logger, location, "MESSAGE ENABLED");
			break;

		case logger:
			options_ |= LOGGER;
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
		case fatal:
			options_ &= ~FATAL;
			out(logger, location, "FATAL DISABLED");
			break;

		case error:
			options_ &= ~ERROR;
			out(logger, location, "ERROR DISABLED");
			break;

		case warning:
			options_ &= ~WARNING;
			out(logger, location, "WARNING DISABLED");
			break;

		case message:
			options_ &= ~MESSAGE;
			out(logger, location, "MESSAGE DISABLED");
			break;

		case logger:
			out(logger, location, "LOGGER MESSAGES DISABLED");
			options_ &= ~LOGGER;
			break;

		default:
			out(priority, location, "INVALID TYPE TO DISABLE");
			break;
		}
	}
};

#endif