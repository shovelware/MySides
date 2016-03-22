#include "Shield.hpp"
#include "Projectile.hpp"


Pickup::Shield::Shield(b2Body* body, const PickupDef& def) :
	Pickup::PickupI(body, def.time),
	radius_(def.size),
	hpMAX_(def.strength),
	hp_(def.strength)
{
	//Body is initially made by pickup base class
}

void Pickup::Shield::onCollect()
{
	body_->DestroyFixture(body_->GetFixtureList());

	//Usual box2d setup
	b2FixtureDef def;
	def.userData = "pickup";

	def.density = 1.0f;
	def.friction = 0.8f;
	def.restitution = 1.f;
	def.isSensor = true;

	b2CircleShape shape;
	shape.m_radius = radius_;

	def.shape = &shape;

	body_->CreateFixture(&def);

	collected_ = true;
}

void Pickup::Shield::takeDamage(int damage)
{
	hp_ -= damage;
}

bool Pickup::Shield::collide(Entity* other, b2Contact& contact)
{

	bool handled = false;

	if (Shape* shape = dynamic_cast<Shape*>(other))
	{
		if (!collected_)
		{
			setOwner(shape);
		}

		handled = true;
	}

	else if (Projectile* proj = dynamic_cast<Projectile*>(other))
	{
		if (collected_)
		{
			//Enemy projectiles after collection
			if (proj->getOwner() != owner_)
			{
				takeDamage(proj->getDamage());
			}
		}

		//Contacts for my projectiles, and all projectiles when not collected
		else contact.SetEnabled(false);
	
		handled = true;
	}
	//
	//else if (Side* side = dynamic_cast<Side*>(other))
	//{
	//	//char* tagA = static_cast<char*>(contact.GetFixtureA()->GetUserData());
	//	//char* tagB = static_cast<char*>(contact.GetFixtureB()->GetUserData());
	//	//
	//	//if (tagA == "side" || tagB == "side")
	//	//{
	//	//	collect(side->getValue());
	//	//}
	//
	//	handled = true;
	//}
	//
	//else if (Bounds* bounds = dynamic_cast<Bounds*>(other))
	//{
	//	handled = true;
	//}
	//
	return handled;
}

void Pickup::Shield::update(int milliseconds)
{
	//Do our collection stuff outside b2 step
	if (owner_ != nullptr && collected_ == false)
	{
		onCollect();
	}

	if (collected_ == true)
	{
		setPosition(owner_->getPosition());

		if (hp_ <= 0)
		{
			owner_ = nullptr;
		}
	}

	Pickup::PickupI::update(milliseconds);
}
