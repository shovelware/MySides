#include "Shield.hpp"
#include "Projectile.hpp"
#include "Side.hpp"
#include "Bounds.hpp"
#include "Pickup.hpp"

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

	def.density = 0.f;
	def.friction = 0.f;
	def.restitution = 0.f;
	def.isSensor = false;

	body_->SetFixedRotation(false);
	
	b2RevoluteJointDef rev;
	rev.localAnchorA = b2Vec2(0, 0);
	rev.localAnchorB = b2Vec2(0, 0);
	

	rev.bodyA = body_;
	rev.bodyB = owner_->getBody();
	
	rev.collideConnected = false;

	body_->GetWorld()->CreateJoint(&rev);

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

			else {
				contact.SetEnabled(false);
			}
		}

		//Contacts for my projectiles, and all projectiles when not collected
		else contact.SetEnabled(false);
	
		handled = true;
	}
	
	else if (Side* side = dynamic_cast<Side*>(other))
	{
		handled = true;
	}

	else if (Pickup::PickupI* pickup = dynamic_cast<Pickup::PickupI*>(other))
	{
		handled = true;
	}

	else if (Bounds* bounds = dynamic_cast<Bounds*>(other))
	{
		handled = true;
	}
	
	return handled;
}

void Pickup::Shield::update(int milliseconds)
{
	Pickup::PickupI::update(milliseconds);

	//Do our collection stuff outside b2 step
	if (owner_ != nullptr && collected_ == false)
	{
		onCollect();
	}

	if (collected_ == true)
	{
		if (owner_ != nullptr)
		{
			setPosition(owner_->getPosition());
		}

		//Edge case where projectile is in shield radius on collect and kills you
		else {
			time_ == 0;
		}
	}

	if (hp_ <= 0 || time_ == 0)
	{
		owner_ = nullptr;
		body_->GetFixtureList()->SetSensor(true);
		body_->GetWorld()->DestroyJoint(body_->GetJointList()->joint);
	}
}
