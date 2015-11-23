// Drawer.hpp
// Basic SFML draw methods: Polygons, circles, lines, points, etc.
// Wrapped in a box2D converter for shapes

#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <stdafx.h>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

struct DrawerStates
{

};

class Drawer {
private:
	sf::RenderWindow* window_;


public:
	Drawer(sf::RenderWindow &window);

	void drawPolygon(const sf::Vector2f* verts, int vertexCount, const sf::Color& col = sf::Color::Cyan, const sf::Color& out = sf::Color::Magenta, int indent = -1);
	void drawCircle(const sf::Vector2f& center, float radius, const sf::Color& col = sf::Color::Cyan, const sf::Color& out = sf::Color::Magenta, int indent = -1);

	void drawLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Color& col = sf::Color::Green);
	void drawPoint(const sf::Vector2f& p, const sf::Color& col = sf::Color::Red);
};
#endif