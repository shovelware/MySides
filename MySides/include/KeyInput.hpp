#ifndef _KEYINPUT_H
#define _KEYINPUT_H

#include "stdafx.h"
#include <SFML/Window/Keyboard.hpp>
#include <unordered_map>

using sf::Keyboard;

typedef sf::Keyboard::Key Key;
using std::list;

/*!
 * \brief Keyboard handler class.
 * Holds and handles all key inputs.
 * Should be updated with game and then queried for keys.
 */

class KeyInput {
public:
	KeyInput();
	//! Updates all keys
	void update();

	//! Is a key down
	bool isKeyDown(const Key key) const;
	//! Is a key up
	bool isKeyUp(const Key key) const;

	//! Has a key just been pressed
	bool isKeyPressed(const Key key) const;

	//! Has a key just been released
	bool isKeyReleased(const Key key) const;
private:
	bool checkKeyDown(list<Key> list, Key key) const;

	list<Key> curKeys_;
	list<Key> prvKeys_;
};
#endif
