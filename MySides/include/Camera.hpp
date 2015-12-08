#ifndef MS_CAMERA_HPP
#define MS_CAMERA_HPP

#include "stdafx.h"

#include <SFML/Graphics/View.hpp>
#include "Entity.hpp"

class Camera : private sf::View {
public:
	Camera(sf::Vector2u screenSize);

	void setTarget(Entity* target);
	Entity* getTarget();
	void clearTarget();

	void update(int milliseconds);
	void move(sf::Vector2f direction);
	sf::View* getView();

	void zoom(float factor);
	void resetZoom();

	//some sort of zoom functionality

private:
	Entity* target_;
	sf::Vector2f screenSize_;
	sf::Vector2f B2toSF(const b2Vec2& vec, bool scale) const;
	
};
#endif