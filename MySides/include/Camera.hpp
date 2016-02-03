#ifndef MS_CAMERA_HPP
#define MS_CAMERA_HPP

#include "stdafx.h"

#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "Bounds.hpp"
#include "Shape.hpp"
#include "Projectile.hpp"
#include "Side.hpp"

class Camera : public sf::View {
public:
	Camera(sf::RenderTarget& target);

	/*
	* \brief Sets the follow target.
	* The camera will follow this ship until the camera is unlocked or the ship is destroyed.
	*/
	void setTarget(Shape* target);
	Shape* getTarget();

	//! Clears the camera's target, can keep camera on target's last position or reset
	void clearTarget(bool savexf);

	//! Loads a font, required for drawing HUD
	void loadFont(std::string filename);

	//! Updates camera's center to where the target is
	void update(int dt);

	//! Move controls for the camera
	void move(sf::Vector2f xf);	void moveReset();

	//! Zoom controls for camera
	void zoomIn();
	void zoomOut();
	void zoomSet(float mult);
	void zoomReset();

	//! HUD Drawing controls, both of these change render target for their duration
	void drawHUD();
	void drawPause();
	void drawOver(int sides, int time);

	float getZoomPercent() const; //!< Returns zoom factor as a percentage

	void updateBounds(sf::Vector2f newSize);
private:
	Shape* target_;			//!< The target followed ship
	sf::Vector2f screenSize_; //!< Keep a record of the screen size for resets

	float zoomFactor_; //!< The internal factor of the zoom
	float zoomStep_; //!< Zoom step [0.25 == 25%]

	sf::Vector2f move_; //!< Translation Vector of camera

	sf::Font* font_; //!< Font to draw HUD with
	sf::Text text_; //!< HUD is drawn with a single text
	sf::RenderTarget& rentrg_; //!< HUD is drawn to this target

	int radarSize_; //!< Radar size within HUD
	sf::Vector2f radarPos_; //!< Radar position within HUD

	void drawText(std::string info, sf::Vector2f pos, sf::Color color); //! Draws a color text at pos to HUD, called by drawHUD()
	void drawTextAligned(std::string info, sf::Vector2f pos, sf::Color color); //! Draws a color text at pos to HUD, called by drawHUD()
	sf::Vector2f B2toSF(const b2Vec2& vec, bool scale = true) const;
};
#endif