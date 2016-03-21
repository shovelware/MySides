// GameRenderer.hpp [BASE: Renderer.hpp]
// Takes b2 as input, draws at sf::V2 scale

#ifndef MS_GAMERenderer_HPP
#define MS_GAMERenderer_HPP

#include "Renderer.hpp"
#include "GameWorld.hpp"

#include <Thor/Vectors.hpp>

class GameRenderer : public Renderer
{
public:
	GameRenderer(sf::RenderWindow& win, GameWorld* world);
	void render();
private:
	sf::Vector2f B2toSF(const b2Vec2& vec, bool scale) const;
	sf::Color B2toSF(const b2Color& col) const;

	sf::Color tweakAlpha(const sf::Color& col, int alpha) const;
	sf::Color blend(const sf::Color& a, int aParts, const sf::Color& b, int bParts) const;

	GameWorld* world_;

	void drawShape(Shape* const s);
	void drawBounds(Bounds* const b);
	void drawProjectile(Projectile* const p);
	void drawSide(Side* const s);
};

#endif