#include "Camera.hpp"

Camera::Camera(sf::Vector2u screenSize) : 
screenSize_(screenSize),
sf::View(sf::Vector2f(screenSize.x / 2, screenSize.y / 2), sf::Vector2f(screenSize)),
target_(nullptr)
{

}

void Camera::setTarget(Entity* target)
{
	target_ = target;
}

Entity* Camera::getTarget()
{
	return target_;
}

void Camera::clearTarget()
{
	target_ = nullptr;
}

void Camera::update(int milliseconds)
{
	if (target_ != nullptr)
	{
		setCenter(B2toSF(target_->getPosition(), true));
	}
}

sf::View * Camera::getView()
{
	return static_cast<sf::View*>(this);
}

void Camera::zoom(float factor) 
{
	sf::View::zoom(factor);
}

void Camera::resetZoom()
{
	setSize(screenSize_);
}

sf::Vector2f Camera::B2toSF(const b2Vec2& vec, bool scale) const
{
	return sf::Vector2f(vec.x * (scale ? _SCALE_ : 1.f), vec.y * (scale ? _SCALE_ : 1.f));
}