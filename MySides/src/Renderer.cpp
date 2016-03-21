#include "Renderer.hpp"

Renderer::Renderer(sf::RenderWindow& window) : window_(&window)
{}

void Renderer::drawPolygon(const sf::Vector2f * verts, int vertexCount, const sf::Color& col, const sf::Color& out, int border)
{
	poly.setPointCount(vertexCount);
	
	for (int p = 0; p < vertexCount; p++)
	{
		poly.setPoint(p, verts[p]);
	}

	poly.setOutlineThickness(border);
	poly.setFillColor(col);
	poly.setOutlineColor(out);

	window_->draw(poly);
}

void Renderer::drawCircle(const sf::Vector2f & center, float radius, const sf::Color& col, const sf::Color& out, int border)
{
	circ.setRadius(radius);
	circ.setPosition(center);
	circ.setOrigin(radius, radius);

	circ.setOutlineThickness(border);
	circ.setFillColor(col);
	circ.setOutlineColor(out);

	window_->draw(circ);
}

void Renderer::drawLine(const sf::Vector2f & a, const sf::Vector2f & b, const sf::Color & col)
{
	sf::Vertex line[] =
	{
		sf::Vertex(a, col),
		sf::Vertex(b, col)
	};

	window_->draw(line, 2, sf::Lines);
}

void Renderer::drawPoint(const sf::Vector2f & p, const sf::Color & col)
{
	sf::Vertex point(p, col);

	window_->draw(&point, 1, sf::Points);
}
