// GameDrawer.hpp [BASE: Drawer.hpp]
// Takes b2 as input, draws at sf::V2 scale

#ifndef MS_GAMEDRAWER_HPP
#define MS_GAMEDRAWER_HPP

#include "Drawer.hpp"
#include "GameWorld.hpp"

class GameDrawer : public Drawer
{
public:
	GameDrawer(sf::RenderWindow& win, GameWorld* world);
	void draw();
private:
	sf::Vector2f B2toSF(const b2Vec2& vec, bool scale) const;
	sf::Color B2toSF(const b2Color& col) const;

	GameWorld* world_;

	void drawPlayer(Shape& p);
	void drawShape(Shape& s);
	void drawBounds(Bounds& b);
	void drawProjectile(Projectile& p);
	void drawSide(Side& s);
};

#endif