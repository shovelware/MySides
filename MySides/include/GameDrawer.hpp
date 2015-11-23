// GameDrawer.hpp
// Takes b2 as input

#ifndef GAMEDRAWER_HPP
#define GAMEDRAWER_HPP

#include "Drawer.hpp"

class GameDrawer : protected Drawer
{
	sf::Vector2f B2toSF(const b2Vec2& vec, bool scale) const;
	sf::Color B2toSF(const b2Color& col) const;
};

#endif