#include "stdafx.h"
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

	sf::Color p = sf::Color::Blue;
	sf::Color s = sf::Color::Black;
	sf::Color t = sf::Color::White;

	Bounds* bounds = world_->getBounds();
	if (bounds != nullptr)
	{
		p = B2toSF(bounds->getPrimary());
		s = B2toSF(bounds->getSecondary());
		t = B2toSF(bounds->getTertiary());
	}

	drawBar(box, timel, tim, s, p, t);
	drawString(box, std::to_string((int)(tim - timel)), t, 2.f);
}

void HUD::drawShapeStatus(sf::FloatRect box)
{
	Shape* c = world_->getControlled();

	if (c != nullptr)
	{
		float hp = c->getHP();
		float hpM = c->getHPMax();
		float uhp = c->getUHP();
		float uhpM = c->getUHPMax();
		float totM = hpM + uhpM; //Total max health

		sf::Color p = B2toSF(c->getPrimary());
		sf::Color s = B2toSF(c->getSecondary());
		sf::Color t = B2toSF(c->getTertiary());

		sf::Color tex = s;
		
		drawBar(box, hp, totM, p, t, s, 4);

		//sf::FloatRect fillRect(box);
		//fillRect.left += line;
		//fillRect.top += line;
		//fillRect.height -= line * 2;
		//fillRect.width -= line * 2;
		//
		//fillRect.width *= (min / max);

		if (uhpM != 0 && uhp != 0)
		{
			//Append a rectangle of uhp 
			sf::FloatRect uhpRect = sf::FloatRect(
				(box.left + 4) + (box.width - 8) * (hp / (totM)),
				box.top + 4,
				(box.width - 8) * (uhp / (totM)),
				box.height - 8);
			//std::cout << uhp << std::endl;
				drawRect(uhpRect, s);
		}
		//drawRect(topBox + );
		//drawString(box, std::to_string((int)(hp + uhp)), tex, 1.75f);
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

		sf::Color p = B2toSF(c->getPrimary());
		sf::Color s = B2toSF(c->getSecondary());
		sf::Color t = B2toSF(c->getTertiary());
		
		drawBar(box, min, max, t, p, s);

		sf::Color txt(s);
		if (!ready)
			txt = p;

		drawString(box, std::to_string((int)min), txt, 2.f);
	}
}

void HUD::drawSideStatus(sf::FloatRect box)
{
	Shape* c = world_->getControlled();

	if (c != nullptr)
	{
		float sids = c->getSidesCollected();

		sf::Color p = B2toSF(c->getPrimary());
		sf::Color s = B2toSF(c->getSecondary());
		sf::Color t = B2toSF(c->getTertiary());

		drawBar(box, sids, 999.f, s, p, s);

		drawString(box, std::to_string((int)sids), t, 4.f);
	}
}

void HUD::loadFont(std::string filename, unsigned int size)
{
	sf::Font* fnt = new sf::Font();

	if (fnt->loadFromFile("../Assets/" + filename))
	{
		//If we load successfully, assign font
		font_ = fnt;

		text_ = sf::Text("", *font_);
		text_.setCharacterSize(size);
	}

	else delete fnt;
}

void HUD::drawString(sf::FloatRect box, std::string info, sf::Color col, float scale)
{
	if (font_ != nullptr)
	{
		text_.setString(info);

		sf::Color outlineCol = sf::Color::Black;
		if ((col.r + col.g + col.b) < (128.f * 3.f))
			outlineCol = sf::Color::White;
		
		text_.setScale(sf::Vector2f(scale * 1.2f, scale * 1.2f));
		sf::FloatRect txt = text_.getLocalBounds();
		text_.setOrigin(txt.left + txt.width / 2, txt.top + txt.height / 2);
		text_.setPosition(box.left + box.width / 2, box.top + box. height / 2);

		text_.setColor(outlineCol);
		trg_.draw(text_);

		text_.setScale(sf::Vector2f(scale, scale));
		txt = text_.getLocalBounds();
		text_.setOrigin(txt.left + txt.width / 2, txt.top + txt.height / 2);
		text_.setPosition(box.left + box.width / 2, box.top + box.height / 2);

		text_.setColor(col);
		trg_.draw(text_);

	}
}

void HUD::drawBar(sf::FloatRect box, float min, float max, sf::Color fill, sf::Color back, sf::Color outline, int line)
{
	drawRect(box, back, outline, -line);


	sf::FloatRect fillRect(box);
	fillRect.left += line;
	fillRect.top += line;
	fillRect.height -= line * 2;
	fillRect.width -= line * 2;

	if (max != 0)
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

sf::Color HUD::B2toSF(const b2Color& col) const
{
	return sf::Color(
		255U * col.r,
		255U * col.g,
		255U * col.b,
		255U * col.a);
}