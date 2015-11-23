#include "Drawer.hpp"

Drawer::Drawer(sf::RenderWindow& window) : window_(&window){}

void Drawer::drawPolygon(const sf::Vector2f * verts, int vertexCount, const sf::Color& col, const sf::Color& out, int indent)
{
	sf::ConvexShape poly(vertexCount);

	for (int p = 0; p < vertexCount; p++)
	{
		poly.setPoint(p, verts[p]);
	}

	poly.setOutlineThickness(indent);
	poly.setFillColor(col);
	poly.setOutlineColor(out);

	window_->draw(poly);
}

void Drawer::drawCircle(const sf::Vector2f & center, float radius, const sf::Color& col, const sf::Color& out, int indent)
{
	sf::CircleShape circ(radius);
	circ.setPosition(center);

	circ.setOutlineThickness(indent);
	circ.setFillColor(col);
	circ.setOutlineColor(out);

	window_->draw(circ);
}

void Drawer::drawLine(const sf::Vector2f & a, const sf::Vector2f & b, const sf::Color & col)
{
	sf::Vertex line[] =
	{
		sf::Vertex(a, col),
		sf::Vertex(b, col)
	};

	window_->draw(line, 2, sf::Lines);
}

void Drawer::drawPoint(const sf::Vector2f & p, const sf::Color & col)
{
	sf::Vertex point(p, col);

	window_->draw(&point, 1, sf::Points);
}
