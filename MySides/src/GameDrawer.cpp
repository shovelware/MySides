#include "GameDrawer.hpp"

sf::Vector2f GameDrawer::B2toSF(const b2Vec2& vec, bool scale) const
{
	return sf::Vector2f(vec.x * (scale ? _SCALE_ : 1.f), vec.y * (scale ? _SCALE_ : 1.f));
}

sf::Color GameDrawer::B2toSF(const b2Color& col) const
{
	return sf::Color(col.r, col.g, col.b, col.a);
}

GameDrawer::GameDrawer(GameWorld world, sf::RenderWindow win) : Drawer(win), world_(&world) 
{
}