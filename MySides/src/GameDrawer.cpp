#include "GameDrawer.hpp"

sf::Vector2f GameDrawer::B2toSF(const b2Vec2& vec, bool scale) const
{
	return sf::Vector2f(vec.x * (scale ? _SCALE_ : 1.f), vec.y * (scale ? _SCALE_ : 1.f));
}

sf::Color GameDrawer::B2toSF(const b2Color& col) const
{
	return sf::Color(col.r, col.g, col.b, col.a);
}

GameDrawer::GameDrawer(sf::RenderWindow& win, GameWorld* world) : Drawer(win), world_(world) 
{
}

void GameDrawer::draw()
{
	////Get a reference or friend function or what have you
	//for (Shape s : world_->shapes_)
	//{
	//
	//}
	//
	//for (Projectile p : world_->projectiles_)
	//{
	//
	//}
}

/*GameDrawer::DrawShape(const &Shape s)
{
	s.draw(this);
}
*/