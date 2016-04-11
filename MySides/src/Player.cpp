#include "Player.hpp"

#include "Projectile.hpp"
#include "Side.hpp"

Player::Player(b2Body* body, const ShapeDef& def, std::function<void(SideDef&)>& callback) :
	Shape(body, def, callback),
	bombTimeMax_(30000),
	bombTime_(0),
	bombRange_(15)
{
	//We spawn faster
	spawnTimeMAX_ /= 2;
	spawnTime_ /= 2;

	body_->GetFixtureList()->SetUserData("player");
	shapeFixDef_.userData = "player";
	collector_ = true;
}

//Only deals with the effects of this collision on this entity
bool Player::collide(Entity * other, b2Contact& contact, std::string tag)
{
	bool handled = Shape::collide(other, contact, tag);

	if (!handled)
	{
	}
	
	return handled;
}

float Player::getBombRange() const { return bombRange_; }
bool Player::getBombReady() const { return bombTime_ == 0; }
int Player::getBombTime() const { return bombTime_; }
int Player::getBombTimeMax() const { return bombTimeMax_; }

void Player::bomb()
{
	bombTime_ = bombTimeMax_;
}

void Player::update(int milliseconds)
{
	Shape::update(milliseconds);
	bombTime_ = (bombTime_ - milliseconds >= 0 ? bombTime_ - milliseconds : 0);
	hasCollected_ = false;
}

bool Player::getCollected() const
{
	return hasCollected_;
}
