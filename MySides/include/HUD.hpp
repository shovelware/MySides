#ifndef MS_HUD_HPP
#define MS_HUD_HPP

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "GameWorld.hpp"

class HUD {
public:
	HUD(sf::RenderTarget& target, GameWorld* world);
	~HUD();

	void drawLevelStatus(sf::FloatRect box);
	void drawShapeStatus(sf::FloatRect box);
	void drawWeaponStatus(sf::FloatRect box);
	void drawSideStatus(sf::FloatRect box);
	
	void loadFont(std::string filename);
	void drawString(sf::FloatRect box, std::string info, sf::Color col = sf::Color::Green);
private:
	void drawBar(sf::FloatRect box, float min, float max, sf::Color fill, sf::Color back, sf::Color outline);
	void drawRect(sf::FloatRect rect, sf::Color fill, sf::Color out = sf::Color::Black, int dent = 0);
	void drawCirc(sf::Vector2f pos, float radius, sf::Color fill, sf::Color out = sf::Color::Black, int dent = 0);

	sf::RenderTarget& trg_;
	GameWorld* world_;

	sf::CircleShape circ_; //pos, col, out, dent
	sf::RectangleShape rect_; //pos, col, out, dent

	sf::Font* font_;
	sf::Text text_;
};

#endif