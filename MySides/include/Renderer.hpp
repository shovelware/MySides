// Renderer.hpp
// Basic SFML draw methods: Polygons, circles, lines, points, etc.
// Wrapped in a box2D converter for shapes

#ifndef MS_RENDERER_HPP
#define MS_RENDERER_HPP

#include <stdafx.h>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

//!	@class	Renderer
//!	@brief	Basic drawing class that can emulate b2Draw's functionality
//!	@see GameRenderer

//!	Re-uses an internal Poly and Circle shape for every applicable draw.

class Renderer {
public:
	Renderer(sf::RenderWindow &window);

	//! Draw a polygon, with primary and secondary colours and a default indented border of 1
	void drawPolygon(const sf::Vector2f* verts, int vertexCount, const sf::Color& col = sf::Color::Cyan, const sf::Color& out = sf::Color::Magenta, int border = -1);

	//! Draw a circle, with primary and secondary colours and a default indented border of 1
	void drawCircle(const sf::Vector2f& center, float radius, const sf::Color& col = sf::Color::Cyan, const sf::Color& out = sf::Color::Magenta, int border = -1);

	//! Draw a line from a to b with a colour
	void drawLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Color& col = sf::Color::Green);

	//! Draw a point with a colour
	void drawPoint(const sf::Vector2f& p, const sf::Color& col = sf::Color::Red);
private:
	sf::CircleShape circ;
	sf::ConvexShape poly;

	sf::RenderWindow* window_;
};
#endif