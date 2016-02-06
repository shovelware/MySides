#include "Player.hpp"
#include "Bounds.hpp"
#include "Projectile.hpp"
#include "Side.hpp"

Player::Player(b2Body* body, ShapeDef def, std::function<void(SideDef&)>& callback) : 
	Shape(body, def, callback)
{
}

//Only deals with the effects of this collision on this entity
bool Player::collide(Entity * other, b2Contact& contact)
{
	bool handled = false;

	if (Shape* shape = dynamic_cast<Shape*>(other))
	{
		handled = true;
	}

	else if (Projectile* proj = dynamic_cast<Projectile*>(other))
	{
		if (proj->getOwner() != this)
		{
			takeDamage(proj->getDamage(), proj->getDirection());
		}

		else contact.SetEnabled(false);

		handled = true;
	}

	else if (Side* side = dynamic_cast<Side*>(other))
	{
		char* tagA = static_cast<char*>(contact.GetFixtureA()->GetUserData());
		char* tagB = static_cast<char*>(contact.GetFixtureB()->GetUserData());

		if (tagA == "side" || tagB == "side")
		{
			collect(side->getValue());
		}

		handled = true;
	}

	else if (Bounds* bounds = dynamic_cast<Bounds*>(other))
	{
		handled = true;
	}

	return handled;
}