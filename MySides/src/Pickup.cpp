#include "Pickup.hpp"

Pickup::PickupI::PickupI(b2Body* body, int time) :
	Entity(body),
	owner_(nullptr),
	collected_(false),
	time_(time)
{
	//Usual box2d setup
	b2FixtureDef def;
	def.userData = "pickup";

	def.density = 0.25f;
	def.friction = 0.4f;
	def.restitution = 1.f;
	def.isSensor = true;

	b2CircleShape shape;
	shape.m_radius = .5f;

	def.shape = &shape;

	body_->CreateFixture(&def);

	setPrimary(b2Color(1, 1, 1, 1));
	setSecondary(b2Color(0, 0, 0, 1));
	setTertiary(b2Color(0, 1, 0, 1));
}

void Pickup::PickupI::update(int milliseconds)
{
	//If collected, we need to track time
	if (collected_)
	{
		//Tick down
		if (time_ > -1)
		{
			time_ = (time_ - milliseconds) >= 0 ? time_ - milliseconds : 0;
			//std::cout << time_ << std::endl;
		}
	}
	//Else just hang out
}

void Pickup::PickupI::setOwner(Shape* o)
{
	owner_ = o;
	if (o != nullptr)
	{
		setPrimary(o->getPrimary());
		setSecondary(o->getSecondary());
		setTertiary(o->getTertiary());
		faction_ = owner_->getFaction();
	}
}

Shape* Pickup::PickupI::getOwner() const
{
	return owner_;
}

bool Pickup::PickupI::getCollected() const
{
	return collected_;
}