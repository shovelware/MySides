#include "Pickup.hpp"

Pickup::PickupI::PickupI(b2Body* body) :
	Entity(body),
	owner_(nullptr),
	collected_(false)
{

}

void Pickup::PickupI::setOwner(Shape* o)
{
	owner_ = o;
}

Shape* Pickup::PickupI::getOwner() const
{
	return owner_;
}

bool Pickup::PickupI::getCollected() const
{
	return collected_;
}