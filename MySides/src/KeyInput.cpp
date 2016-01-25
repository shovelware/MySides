#include "KeyInput.hpp"

KeyInput::KeyInput()
{
}

void KeyInput::update()
{
	prvKeys_ = curKeys_;
	curKeys_.clear();

	for (int i = 0; i < Key::KeyCount; ++i)
	{
		Key k = static_cast<Key>(i);

		if (Keyboard::isKeyPressed(k))
		{
			curKeys_.push_back(k);
		}
	}
}

bool KeyInput::isKeyDown(const Key key) const
{
	bool response = false;

	//Simple check usage
	response = checkKeyDown(curKeys_, key);

	return response;
}

bool KeyInput::isKeyUp(const Key key) const
{
	bool response = false;

	//Up is NOT(down)
	response = !(checkKeyDown(curKeys_, key));

	return response;
}

bool KeyInput::isKeyPressed(const Key key) const
{
	bool response = false;

	bool cur = checkKeyDown(curKeys_, key);
	bool prv = checkKeyDown(prvKeys_, key);

	//Just pressed means prv up && cur down
	response = (prv == false && cur == true);

	return response;
}

bool KeyInput::isKeyReleased(const Key key) const
{
	bool response = false;

	bool cur = checkKeyDown(curKeys_, key);
	bool prv = checkKeyDown(prvKeys_, key);

	//Just released means prv down && cur up
	response = (prv == true && cur == false);

	return response;
}

bool KeyInput::checkKeyDown(list<Key> list, Key key) const
{
	bool response = false;

	//Checks if key is in list
	response = (std::find(list.begin(), list.end(), key) != list.end());

	return response;
}