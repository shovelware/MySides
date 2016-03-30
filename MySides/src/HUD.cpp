#include "HUD.hpp"

HUD::HUD(sf::RenderTarget& target, GameWorld* world) :
	trg_(target),
	world_(world),
	font_(nullptr)
{

}

HUD::~HUD()
{
	if (font_ != nullptr)
	{
		delete font_;
	}
}

void HUD::drawLevelStatus(sf::FloatRect box)
{

	float tim = world_->maxTime;
	float timel = world_->getTimeInLevel();

	drawBar(box, timel, tim, sf::Color::Blue, sf::Color::Blue + sf::Color::Black, sf::Color::Black);
	drawString(box, std::to_string(tim - timel), sf::Color::Cyan);
}

void HUD::drawShapeStatus(sf::FloatRect box)
{
	Shape* c = world_->getControlled();

	if (c != nullptr)
	{
		float hp = c->getHP();
		float hpM = c->getHPMax();

		drawBar(box, hp, hpM, sf::Color::Red, sf::Color::Red + sf::Color::Black, sf::Color::Black);
		drawString(box, std::to_string(hp), sf::Color::Black);	
	}
}

void HUD::drawWeaponStatus(sf::FloatRect box)
{
	Shape* c = world_->getControlled();

	if (c != nullptr)
	{
		float min = c->getWeaponBar();
		float max = c->getWeaponBarMAX();
		bool ready = c->getWeaponReady();
		
		drawBar(box, min, max, sf::Color::Blue, sf::Color::Black, sf::Color::Black);

		sf::Color txt(sf::Color::Cyan + sf::Color::Black);
		if (!ready)
			txt = sf::Color::Red;

		drawString(box, std::to_string(min), txt);
	}
}

void HUD::drawSideStatus(sf::FloatRect box)
{
	Shape* c = world_->getControlled();

	if (c != nullptr)
	{
		float sids = c->getSidesCollected();

		drawBar(box, sids, 999.f, sf::Color::Cyan, sf::Color::Blue, sf::Color::Black);

		drawString(box, std::to_string(sids), sf::Color::Blue + sf::Color::Black);
	}
}

void HUD::loadFont(std::string filename)
{
	sf::Font* fnt = new sf::Font();

	if (fnt->loadFromFile("../Assets/" + filename))
	{
		//If we load successfully, assign font
		font_ = fnt;

		text_ = sf::Text("", *font_);
		text_.setCharacterSize(36);
	}

	else delete fnt;
}

void HUD::drawString(sf::FloatRect box, std::string info, sf::Color col)
{
	if (font_ != nullptr)
	{
		text_.setString(info);
		//sf::FloatRect txt = text_.getGlobalBounds();
		//text_.setOrigin(txt.left + txt.width / 2, txt.top + txt.height / 2);

		text_.setPosition(box.left, box.top);
		text_.setColor(col);
		trg_.draw(text_);
	}
}

void HUD::drawBar(sf::FloatRect box, float min, float max, sf::Color fill, sf::Color back, sf::Color outline)
{
	drawRect(box, outline);

	sf::FloatRect fillRect(box);
	fillRect.left += 1;
	fillRect.top += 1;
	fillRect.height -= 2;
	fillRect.width -= 2;

	drawRect(fillRect, back);

	fillRect.width *= (min / max);

	drawRect(fillRect, fill);
}

void HUD::drawRect(sf::FloatRect rect, sf::Color fill, sf::Color out, int dent)
{
	rect_.setPosition(rect.left, rect.top);
	rect_.setSize(sf::Vector2f(rect.width, rect.height));
	rect_.setFillColor(fill);
	rect_.setOutlineColor(out);
	rect_.setOutlineThickness(dent);

	trg_.draw(rect_);
}

void HUD::drawCirc(sf::Vector2f pos, float radius, sf::Color fill, sf::Color out, int dent)
{
	circ_.setPosition(pos);
	circ_.setRadius(radius);
	circ_.setOrigin(radius, radius);
	circ_.setFillColor(fill);
	circ_.setOutlineColor(out);
	circ_.setOutlineThickness(dent);

	trg_.draw(circ_);
}