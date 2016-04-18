#include "Player.hpp"

#include "Projectile.hpp"
#include "Side.hpp"

Player::Player(b2Body* body, const PlayerDef & def, std::function<void(SideDef&)>& callback) :
	Shape(body, def, callback),
	bombTimeMax_(def.bombTime),
	bombTime_(0),
	bombRadius_(def.bombRadius)
{
	//We spawn faster
	spawnTimeMAX_ /= 2;
	spawnTime_ /= 2;

	body_->GetFixtureList()->SetUserData("player");
	shapeFixDef_.userData = "player";
	collector_ = true;
}

Player::~Player()
{
	Shape::~Shape();
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

bool Player::getBombReady() const { return (bombTimeMax_ > 0 && bombTime_ == 0); }
float Player::getBombRadius() const { return bombRadius_; }
void Player::setBombRadius(float range) {bombRadius_ = range > 0 ? range : bombRadius_; }
int Player::getBombTime() const { return bombTime_; }
int Player::getBombTimeMax() const { return bombTimeMax_; }
void Player::setBombTimeMax(int time) { bombTimeMax_ = time > 0 ? time : bombTimeMax_; }

void Player::bomb()
{
	if (bombTimeMax_ > 0)
		bombTime_ = bombTimeMax_;
}

void Player::update(int milliseconds)
{
	Shape::update(milliseconds);
	bombTime_ = (bombTime_ - milliseconds >= 0 ? bombTime_ - milliseconds : 0);
}
