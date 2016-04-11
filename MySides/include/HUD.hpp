#ifndef MS_HUD_HPP
#define MS_HUD_HPP

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "GameWorld.hpp"

class HUD {
public:
	HUD(sf::RenderTarget& target, GameWorld* world);
	~HUD();

	void drawLevelStatus(sf::FloatRect& const box);
	void drawShapeStatus(sf::FloatRect& const box);
	void drawWeaponStatus(sf::FloatRect& const box);
	void drawSideStatus(sf::FloatRect& const box);

	void drawDebugInfo(sf::FloatRect& const box);
	void drawLevelInfo(sf::FloatRect& const box);
	
	void loadFont(std::string filename, unsigned int size);
	void drawString(sf::FloatRect box, std::string info, sf::Color col = sf::Color::Green, float sizeScale = 1.f);
	void drawStringLeft(sf::FloatRect box, std::string info, sf::Color col = sf::Color::Green, float sizeScale = 1.f);
	void drawStringRight(sf::FloatRect box, std::string info, sf::Color col = sf::Color::Green, float sizeScale = 1.f);
private:
	void drawBar(sf::FloatRect box, float min, float max, sf::Color fill, sf::Color back, sf::Color outline, int line = 2);
	void drawRect(sf::FloatRect rect, sf::Color fill, sf::Color out = sf::Color::Black, int dent = 0);
	void drawCirc(sf::Vector2f pos, float radius, sf::Color fill, sf::Color out = sf::Color::Black, int dent = 0);


	sf::Color B2toSF(const b2Color& col) const;

	sf::RenderTarget& trg_;
	GameWorld* world_;

	sf::CircleShape circ_; //pos, col, out, dent
	sf::RectangleShape rect_; //pos, col, out, dent

	sf::Font* font_;
	sf::Text text_;
};

#endif