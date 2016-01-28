#include "Camera.hpp"


Camera::Camera(sf::RenderTarget& target) :
	rentrg_(target),
	screenSize_(target.getSize()),
	View(sf::Vector2f(target.getSize().x / 2, target.getSize().y / 2), sf::Vector2f(target.getSize())),
	radarSize_(128),
	radarPos_(screenSize_.x - radarSize_, radarSize_),
	zoomFactor_(1),
	zoomStep_(0.05f),
	font_(nullptr)
{
}

void Camera::setTarget(Shape * target)
{
	target_ = target;
}

Shape * Camera::getTarget()
{
	return target_;
}

void Camera::clearTarget(bool savexf)
{
	if (savexf)
	{
		move_ += B2toSF(target_->getPosition());
	}

	target_ = nullptr;
}

void Camera::loadFont(std::string filename)
{
	sf::Font* fnt = new sf::Font();

	if (fnt->loadFromFile("../Assets/" + filename));
	{
		//If we load successfully, assign font
		font_ = fnt;

		text_ = sf::Text("", *font_);
		text_.setCharacterSize(50);
	}
}

void Camera::update(int dt)
{
	if (target_ != nullptr)
	{
		setCenter(B2toSF(target_->getPosition()) + move_);
	}

	else setCenter(move_);
}

void Camera::move(sf::Vector2f xf)
{
	move_ += xf;
}

void Camera::moveReset()
{
	move_ = sf::Vector2f(0, 0);
}

void Camera::zoomIn()
{
	zoomFactor_ -= zoomStep_;
	zoom(1 - zoomStep_);
}

void Camera::zoomOut()
{
	zoomFactor_ += zoomStep_;
	zoom(1 + zoomStep_);
}

void Camera::zoomSet(float mult)
{
	zoom(mult);
}

void Camera::zoomReset()
{
	zoomFactor_ = 1;
	reset(sf::FloatRect(sf::Vector2f(0, 0), screenSize_));
}

void Camera::drawHUD()
{
	rentrg_.setView(rentrg_.getDefaultView());

	bool fnt = (font_ != nullptr);
	bool trg = (target_ != nullptr);


	//Draw general text items
	if (fnt)
	{
		//Zoom
		drawText(std::string("Z: ") + std::to_string((int)getZoomPercent()),
			sf::Vector2f(0, 58),
			sf::Color::White
			);

		//Take camera pos, might add target pos later
		sf::Vector2f pos = move_;

		if (trg)
		{
			//Add target to pos if we have one
			pos += B2toSF(target_->getPosition());

			//Draw controlled text items
			//Radar range
			//text_.setString(std::string(std::to_string(target_->getRadarRange()) + "sm"));

			//Sides bar
			drawText(std::string("Sides: ") + std::to_string(target_->getSidesCollected()),
				sf::Vector2f(0, screenSize_.y - 60),
				sf::Color::Red);

			//Health bar
			drawText(std::string("HP: ") + std::to_string(target_->getHP()) + "/" + std::to_string(target_->getHPMax()),
				sf::Vector2f(screenSize_.x - 100, screenSize_.y - 60),
				sf::Color::Red);
			////Fire capabilities bar
			//Accel bar

		}

		//Pos
		drawText(std::string("X: ") + std::to_string(pos.x) + std::string("\nY: ") + std::to_string(pos.y),
			sf::Vector2f(0, 0),
			sf::Color::White);
	}

	rentrg_.setView(*this);
}

void Camera::drawPause()
{
	rentrg_.setView(rentrg_.getDefaultView());
	sf::Vector2f offset(0, 20);
	
	drawTextAligned("PAUSED", sf::Vector2f(screenSize_.x / 2, screenSize_.y / 2), sf::Color::Cyan);

	drawTextAligned("START : CONTINUE", sf::Vector2f(screenSize_.x / 2, screenSize_.y / 2) + offset * 2.f, sf::Color::Cyan);


	drawTextAligned("LS : MOVE", sf::Vector2f(screenSize_.x / 2, screenSize_.y / 2) + offset * 4.f, sf::Color::Cyan);
	drawTextAligned("RS : SHOOT", sf::Vector2f(screenSize_.x / 2, screenSize_.y / 2) + offset * 5.f, sf::Color::Cyan);
	drawTextAligned("LB : ZOOM-", sf::Vector2f(screenSize_.x / 2, screenSize_.y / 2) + offset * 6.f, sf::Color::Cyan);
	drawTextAligned("RB : ZOOM+", sf::Vector2f(screenSize_.x / 2, screenSize_.y / 2) + offset * 7.f, sf::Color::Cyan);

	//drawTextAligned("RB : ZOOM+", sf::Vector2f(screenSize_.x / 2, screenSize_.y / 2) + offset * 7.f, sf::Color::Cyan);
	//drawTextAligned("RB : ZOOM+", sf::Vector2f(screenSize_.x / 2, screenSize_.y / 2) + offset * 8.f, sf::Color::Cyan);
	//drawTextAligned("RB : ZOOM+", sf::Vector2f(screenSize_.x / 2, screenSize_.y / 2) + offset * 9.f, sf::Color::Cyan);
	rentrg_.setView(*this);
}

float Camera::getZoomPercent() const
{
	return 200 - (zoomFactor_ * 100);
}

void Camera::drawText(std::string info, sf::Vector2f pos = sf::Vector2f(0, 0), sf::Color color = sf::Color::White)
{
	//Check if hud font and text are initialised
	if (font_ != nullptr)
	{
		text_.setString(info);
		text_.setPosition(pos);
		text_.setColor(color);
		rentrg_.draw(text_);
	}
}

void Camera::drawTextAligned(std::string info, sf::Vector2f pos = sf::Vector2f(0, 0), sf::Color color = sf::Color::White)
{
	//Check if hud font and text are initialised
	if (font_ != nullptr)
	{

		text_.setString(info);

		sf::Vector2f offset(text_.getLocalBounds().width / 2, text_.getLocalBounds().height / 2);

		text_.setPosition(pos - offset);
		text_.setColor(color);
		rentrg_.draw(text_);
	}
}

sf::Vector2f Camera::B2toSF(const b2Vec2& vec, bool scale) const
{
	return sf::Vector2f(vec.x * (scale ? _SCALE_ : 1.f), vec.y * (scale ? _SCALE_ : 1.f));
}