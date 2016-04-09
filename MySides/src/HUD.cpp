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
	Level& level = world_->getCurrentLevel();
	int limitMin = level.getLimit();
	int limitMax = level.getLimitMAX();
	int time = level.getTime();

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

	sf::FloatRect eighthBox(box.left + box.width - (box.width / 8), box.height + (box.height / 2), box.width / 8, box.height / 8);

	drawBar(box, limitMin, limitMax, s, p, t);
	drawString(eighthBox, std::to_string((int)(time)), t, 1.f);
	drawStringLeft(box, std::to_string(limitMin), t, 1.5f);
	drawStringRight(box, std::to_string(limitMax), t, 1.5f);
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
		//drawStringLeft(box, std::to_string((int)(hp)), tex, 0.75f);
		//drawString(box, std::to_string((int)(hp + uhp)), tex, 1.75f);
		//drawStringRight(box, std::to_string((int)(uhp)), tex, 0.75f);
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
		bool loading = c->getWeaponLoading();
		int level = c->getWeaponLevel();

		sf::Color p = B2toSF(c->getPrimary());
		sf::Color s = B2toSF(c->getSecondary());
		sf::Color t = B2toSF(c->getTertiary());
		
		//HUD element indent
		int dent = 2;
		
		//Replace bg and bars with this if you want no gaps
		drawBar(box, min, max, t, p, s);

		//Draw Ammo Number
		sf::Color txtcol(p);
		if (loading)
			txtcol = t;
		else if (ready)
			txtcol = s;
		drawString(box, std::to_string((int)min), txtcol, 2.f);

		//Draw level value
		sf::FloatRect eighthBox(box.left, box.height + (box.height / 2), box.width / 8, box.height / 8);
		//int powlv = pow(level, 2);
		//if (level > 8) powlv = level; //Special weapons have special levels
		//std::string lvlstr = std::string("V" + std::to_string(powlv % 10 > 1 ? powlv / 10 : 0) + "." + std::to_string(powlv % 10));
		//lvlstr = std::to_string(powlv);
		int lv = (level < 10 ? level * 10 : level);
		std::string lvlstr = std::to_string(lv / 10) + "." + std::to_string(lv % 10);
		drawString(eighthBox, lvlstr, t, 1.f);
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

void HUD::drawDebugStatus(sf::FloatRect box)
{
	sf::FloatRect leftBox(box.left, box.top, box.width / 10, box.height);
	sf::FloatRect rightBox(box.left + (box.width / 10), box.top, (box.width / 10) * 9, box.height);

	drawString(leftBox, std::to_string(world_->di));
	drawStringLeft(rightBox, world_->dstr);
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

void HUD::drawString(sf::FloatRect box, std::string info, sf::Color col, float sizeScale)
{
	if (font_ != nullptr)
	{
		text_.setString(info);

		sf::Color outlineCol = sf::Color::Black;
		if ((col.r + col.g + col.b) < (128.f * 3.f))
			outlineCol = sf::Color::White;
		
		text_.setScale(sf::Vector2f(sizeScale * 1.2f, sizeScale * 1.2f));
		sf::FloatRect txt = text_.getLocalBounds();
		text_.setOrigin(txt.left + txt.width / 2, txt.top + txt.height / 2);
		text_.setPosition(box.left + box.width / 2, box.top + box. height / 2);

		text_.setColor(outlineCol);
		trg_.draw(text_);

		text_.setScale(sf::Vector2f(sizeScale, sizeScale));
		text_.setColor(col);
		trg_.draw(text_);
	}
}

void HUD::drawStringLeft(sf::FloatRect box, std::string info, sf::Color col, float sizeScale)
{
	if (font_ != nullptr)
	{
		text_.setString(info);

		sf::Color outlineCol = sf::Color::Black;
		if ((col.r + col.g + col.b) < (128.f * 3.f))
			outlineCol = sf::Color::White;

		text_.setScale(sf::Vector2f(sizeScale * 1.2f, sizeScale * 1.2f));
		float margin = text_.getCharacterSize() * sizeScale;
		sf::FloatRect txt = text_.getLocalBounds();
		text_.setOrigin(txt.left + txt.width / 2, txt.top + txt.height / 2);
		text_.setPosition(box.left + txt.width, box.top + box.height / 2);

		text_.setColor(outlineCol);
		trg_.draw(text_);

		text_.setScale(sf::Vector2f(sizeScale, sizeScale));

		text_.setColor(col);
		trg_.draw(text_);

	}
}

void HUD::drawStringRight(sf::FloatRect box, std::string info, sf::Color col, float sizeScale)
{
	if (font_ != nullptr)
	{
		text_.setString(info);

		sf::Color outlineCol = sf::Color::Black;
		if ((col.r + col.g + col.b) < (128.f * 3.f))
			outlineCol = sf::Color::White;

		text_.setScale(sf::Vector2f(sizeScale * 1.2f, sizeScale * 1.2f));
		float margin = text_.getCharacterSize() * sizeScale;
		sf::FloatRect txt = text_.getLocalBounds();
		text_.setOrigin(txt.left + txt.width / 2, txt.top + txt.height / 2);
		text_.setPosition(box.left + box.width - txt.width, box.top + box.height / 2);
		
		text_.setColor(outlineCol);
		trg_.draw(text_);

		text_.setScale(sf::Vector2f(sizeScale, sizeScale));
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

	//Draw fill bars if we're fillable
	if (max > 0)
	{
		if (max >= min)
		{
			sf::FloatRect barBox(box.left + line, box.top + line, box.width - line * 2, box.height - line * 2);

			float div = line * 0.5f;
			float divsW = (max - 1) * div;
			float splitMax = max + divsW;

			if (splitMax < barBox.width)
			{
				float roundW = (barBox.width - divsW) / max;
				sf::FloatRect roundRect(barBox.left, barBox.top, roundW, barBox.height);
				for (int i = 0; i < min; ++i)
				{
					drawRect(roundRect, fill);
					roundRect.left += (div + roundW);
				}
			}

			else if (max != 0)
			{
				fillRect.width *= fminf(max / max, (min / max));
				drawRect(fillRect, fill);
			}
		}

		else drawRect(fillRect, fill);
	}

	//drawRect(fillRect, fill);
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