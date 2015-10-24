//Log.hpp
// Globally accessibly cout for logging and debugging
// 0 - FATAL	2 - WARNING
// 1 - ERROR	3 - MESSAGE

#ifndef LOG_H
#define LOG_H

#include <iostream>

using std::cout;
using std::endl;

class Log {
private:
	Log() {}
	Log(Log const&) = delete;
	void operator=(Log const&) = delete;

	enum verbosity_ {
		FATAL = 0x01,
		ERROR = 0x02,
		WARNING = 0x04,
		MESSAGE = 0x08
	};

	unsigned int options = FATAL | ERROR | WARNING | MESSAGE;

public:
	static Log& getInstance()
	{
		static Log instance;

		return instance;
	}

	void lout(unsigned int type, char* message)
	{
		switch (type)
		{
		case 0:
			if (options & FATAL)
			{
				cout << "[F] " << message << endl;
			}
			break;
		case 1:
			if (options & ERROR)
			{
				cout << "[E] " << message << endl;
			}
			break;
		case 2:
			if (options & WARNING)
			{
				cout << "[W] " << message << endl;
			}
			break;
		case 3:
			if (options & MESSAGE)
			{
				cout << "[M] " << message << endl;
			}
			break;
		default:
			cout << "MALFORMED LOUT" << endl;
			break;
		}
	}

	void toggleType(unsigned int type)
	{
		switch (type)
		{
		case 0:
			options = options ^ FATAL;
			break;
		case 1:
			options = options ^ ERROR;
			break;
		case 2:
			options = options ^ WARNING;
			break;
		case 3:
			options = options ^ MESSAGE;
			break;
		default:
			lout(1, "Invalid Type Toggle");
			break;
		}
	}

};

#endif