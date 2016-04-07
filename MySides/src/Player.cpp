#include "Player.hpp"

#include "Projectile.hpp"
#include "Side.hpp"

Player::Player(b2Body* body, const ShapeDef& def, std::function<void(SideDef&)>& callback) :
	Shape(body, def, callback),
	bombTimeMax_(30000),
	bombTime_(0),
	bombRange_(15)
{
	body_->GetFixtureList()->SetUserData("player");
	shapeFixDef_.userData = "player";
}

//Only deals with the effects of this collision on this entity
bool Player::collide(Entity * other, b2Contact& contact, std::string tag)
{
	bool handled = false;

	if (tag == "projectile")
	{
		Projectile* proj = static_cast<Projectile*>(other);

		if (proj->getFaction() != faction_ && faction_ != GOD)
		{
			takeDamage(proj->getDamage(), proj->getDirection());

			if (alive_ == false)
				contact.SetEnabled(false);
		}

		else contact.SetEnabled(false);

		handled = true;
	}

	else if (tag == "side")
	{
		char* tagA = static_cast<char*>(contact.GetFixtureA()->GetUserData());
		char* tagB = static_cast<char*>(contact.GetFixtureB()->GetUserData());

		if (tagA == "side" || tagB == "side")
		{
			Side* side = static_cast<Side*>(other);
			collect(side->getValue());
			side->collect();
			hasCollected_ = true;
		}

		handled = true;
	}

	return handled;
}

float Player::getBombRange() const { return bombRange_; }
bool Player::getBombReady() const { return bombTime_ == 0; }

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
