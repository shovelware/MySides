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

void HUD::drawLevelStatus(sf::FloatRect const& box)
{
	//Level Info: Waves, limits, respite
	Level::LevelI& level = world_->getWorldLevel();

	int limitMin = level.getLimit();
	int limitMax = level.getLimitMAX();

	int alpha = level.getInfoAlpha();
	int beta = level.getInfoBeta();
	int gamma = level.getInfoGamma();
	
	int respiteTime = level.getRespiteTime();
	int respiteTimeMax = level.getRespiteTimeMAX();

	//Bar colours
	sf::Color p = B2toSF(level.getPrimary());
	sf::Color s = B2toSF(level.getSecondary());
	sf::Color t = B2toSF(level.getTertiary());

	//Rects
	sf::FloatRect barBox(box);
	sf::FloatRect eighthBox(box.left + box.width - (box.width / 8), box.height + (box.height / 2), box.width / 8, box.height / 8);

	barBox.height *= 0.75;

	sf::FloatRect waveBox(barBox.left, barBox.top + barBox.height - 2, barBox.width * 0.75f, box.height * 0.25f);
	sf::FloatRect botBox(box.left, box.height + box.height * 0.75, box.width, box.height / 8);

	//Draw limit bar
	drawBar(barBox, limitMin, limitMax, s, p, t);

	//Draw enemy counts
	drawStringLeft(botBox, std::to_string(alpha), t, 1.f);
	drawString(botBox, std::to_string(beta), t, 1.25f);
	drawStringRight(botBox, std::to_string(gamma), t, 1.f);

	//Draw respite bar if we must, or wave limit
	if (respiteTime > 0) { drawBar(waveBox, respiteTimeMax - respiteTime, respiteTimeMax, s, p, t, 2); }
	else drawBar(waveBox, beta - alpha, gamma - alpha, s, p, t, 2);

	//Draw limits min and max
	drawStringLeft(barBox, std::to_string(limitMin), t, 1.f);
	drawStringRight(barBox, std::to_string(limitMax), t, 1.f);
}

void HUD::drawShapeStatus(sf::FloatRect const& box)
{
	Shape* c = world_->getControlled();

	if (c != nullptr)
	{
		//Poll info
		float hp = c->getHP();
		float hpM = c->getHPMax();
		float uhp = c->getUHP();
		float uhpM = c->getUHPMax();
		float totM = hpM + uhpM; //Total max health

		//Bar colours
		sf::Color p = B2toSF(c->getPrimary());
		sf::Color s = B2toSF(c->getSecondary());
		sf::Color t = B2toSF(c->getTertiary());

		//Alive & spawn state
		bool alive = c->getAlive();
		float spawnProgress = (float)c->getSpawnTime() / (float)c->getSpawnTimeMax();
		
		//Text colour
		//sf::Color tex = s;

		//Rect
		sf::FloatRect barBox(box);

		//Adjust alpha and position for spawning animation
		if (!alive)
		{
			p.a *= 1 - spawnProgress;
			s.a *= 1 - spawnProgress;
			t.a *= 1 - spawnProgress;

			barBox.left += (barBox.width / 2) * (spawnProgress);
			barBox.width -= (barBox.width) * (spawnProgress);
		}

		
		//Draw HP Bar
		drawBar(barBox, hp, totM, p, t, s, 4);

		//Draw UHP
		if (uhpM != 0 && uhp != 0)
		{
			//Append a rectangle of uhp 
			sf::FloatRect uhpRect = sf::FloatRect(
				(barBox.left + 4) + (barBox.width - 8) * (hp / (totM)),
				barBox.top + 4,
				(barBox.width - 8) * (uhp / (totM)),
				barBox.height - 8);
			//std::cout << uhp << std::endl;
				drawRect(uhpRect, s);
		}

		//Draw number values: HP, HP + UHP, HP
		//drawStringLeft(box, std::to_string((int)(hp)), tex, 0.75f);
		//drawString(box, std::to_string((int)(hp + uhp)), tex, 1.75f);
		//drawStringRight(box, std::to_string((int)(uhp)), tex, 0.75f);
	}
}

void HUD::drawWeaponStatus(sf::FloatRect const& box)
{
	Shape* c = world_->getControlled();

	if (c != nullptr)
	{
		//Pull info

		//Weapon Info
		float min = c->getWeaponBar();
		float max = c->getWeaponBarMAX();
		bool ready = c->getWeaponReady();
		bool loading = c->getWeaponLoading();
		int level = c->getWeaponLevel();
		
		//Bar colour
		sf::Color p = B2toSF(c->getPrimary());
		sf::Color s = B2toSF(c->getSecondary());
		sf::Color t = B2toSF(c->getTertiary());
		
		//Rects
		sf::FloatRect barBox(box);
		sf::FloatRect eighthBox(box.left, box.height + box.height / 2, box.width / 8, box.height / 8);

		//Text colour
		sf::Color txtcol(p);
		if (loading)	txtcol = t;
		else if (ready)	txtcol = s;

		//Level string
		int lv = (level < 10 ? level * 10 : level);
		std::string lvlstr = std::to_string(lv / 10) + "." + std::to_string(lv % 10);


		//Draw Stuff

		//Draw bomb if we have it
		if (Player* player = dynamic_cast<Player*>(c))
		{
			int bombTimeMax = player->getBombTimeMax();
			int bombTime = bombTimeMax - player->getBombTime();

			barBox.height *= 0.75;

			sf::FloatRect bombBox(barBox.left + barBox.width / 4, barBox.top + barBox.height - 2, barBox.width * 0.75f, box.height * 0.25f);
			
			drawBar(bombBox, bombTime, bombTimeMax, t, p, s);
		}

		//Draw Ammo Bar
		drawBar(barBox, min, max, t, p, s);

		//Draw Ammo Number
		drawString(barBox, std::to_string((int)min), txtcol, 1.5f);

		//Draw Weapon Level
		drawString(eighthBox, lvlstr, t, 1.f);
	}
}

void HUD::drawSideStatus(sf::FloatRect const& box)
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

void HUD::drawDebugInfo(sf::FloatRect const& box)
{
	sf::FloatRect leftBox(box.left, box.top, box.width / 10, box.height);
	sf::FloatRect rightBox(box.left + (box.width / 10), box.top, (box.width / 10) * 9, box.height);

	drawString(leftBox, std::to_string(world_->di));
	drawStringLeft(rightBox, world_->dstr);
	drawString(rightBox, world_->getSelectedLevel().getID());
}

void HUD::drawLevelInfo(sf::FloatRect const& box)
{
	//Level Info: Time, completion, name
	Level::LevelI& level = world_->getWorldLevel();
	std::string id = level.getID();

	bool started = level.getStarted();
	bool complete = level.getComplete();

	int time = level.getTime();
	int timeMax = level.getTimeMAX();

	int timeComplete = level.getTimeComplete();
	bool timeStandard = level.getTimeStandard();

	//Bar colours
	sf::Color p = B2toSF(level.getPrimary());
	sf::Color s = B2toSF(level.getSecondary());
	sf::Color t = B2toSF(level.getTertiary());

	//Rects
	sf::FloatRect infoBox(box);
	sf::FloatRect nameBox(box.left, box.top, box.width, box.height * 0.6f);
	sf::FloatRect numBox(box.left + box.width * 0.1f, box.top + box.height * 0.6f, box.width * 0.8f, box.height * 0.2f);
	sf::FloatRect timeBox(box.left, box.top * box.height * 0.9f, box.width, box.height * 0.1f);

	sf::Color fill = s;
	sf::Color out = t;

	if (!started)
	{
		fill = s;
		out = p;
	}

	else if (complete)
	{
		fill = t;
		out = s;
	}
	
	drawRect(infoBox, fill, out, -2);
	drawString(nameBox, id, p, 1.5f);
	drawStringLeft(numBox, "T" + std::to_string(time), p, 1.f);
	drawString(numBox, "C" + std::to_string(timeComplete), p, 1.f);
	drawStringRight(numBox, "S" + std::to_string(timeStandard), p, 1.f);

	if (started) //Draw time
	{
		drawBar(timeBox, time, timeMax, s, p, t);
	}
}

void HUD::drawTransitionSquare(sf::FloatRect const& win)
{
	//Start 0 -> 1 Finish
	float trans = world_->getTransitionProgress();
	drawRect(win, sf::Color(0, 0, 0, trans * 255));
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

		outlineCol.a = col.a;
		
		text_.setScale(sf::Vector2f(sizeScale * 1.2f, sizeScale * 1.2f));
		sf::FloatRect txt = text_.getLocalBounds();
		text_.setOrigin(txt.left + txt.width / 2, txt.top + txt.height / 2);
		text_.setPosition(box.left + box.width / 2, box.top + box. height / 2);

		text_.setColor(outlineCol);
		trg_.draw(text_);

		text_.setScale(sf::Vector2f(sizeScale * 1.15f, sizeScale));
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

		outlineCol.a = col.a;

		text_.setScale(sf::Vector2f(sizeScale * 1.2f, sizeScale * 1.2f));
		float margin = text_.getCharacterSize() * sizeScale;
		sf::FloatRect txt = text_.getLocalBounds();
		text_.setOrigin(txt.left + txt.width / 2, txt.top + txt.height / 2);
		text_.setPosition(box.left + txt.width, box.top + box.height / 2);

		text_.setColor(outlineCol);
		trg_.draw(text_);

		text_.setScale(sf::Vector2f(sizeScale * 1.15f, sizeScale));

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


		outlineCol.a = col.a;

		text_.setScale(sf::Vector2f(sizeScale * 1.2f, sizeScale * 1.2f));
		float margin = text_.getCharacterSize() * sizeScale;
		sf::FloatRect txt = text_.getLocalBounds();
		text_.setOrigin(txt.left + txt.width / 2, txt.top + txt.height / 2);
		text_.setPosition(box.left + box.width - txt.width, box.top + box.height / 2);
		
		text_.setColor(outlineCol);
		trg_.draw(text_);

		text_.setScale(sf::Vector2f(sizeScale * 1.15f, sizeScale));
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
	if (max > -1)
	{
		if (max >= min && max != 0)
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