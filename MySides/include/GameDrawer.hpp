// GameDrawer.hpp [BASE: Drawer.hpp]
// Takes b2 as input, draws at sf::V2 scale

#ifndef MS_GAMEDRAWER_HPP
#define MS_GAMEDRAWER_HPP

#include "Drawer.hpp"
#include "GameWorld.hpp"

class GameDrawer : public Drawer
{
private:
	sf::Vector2f B2toSF(const b2Vec2& vec, bool scale) const;
	sf::Color B2toSF(const b2Color& col) const;

	GameWorld* world_;
public:
	GameDrawer(sf::RenderWindow& win, GameWorld* world);
	void draw();
};

#endif