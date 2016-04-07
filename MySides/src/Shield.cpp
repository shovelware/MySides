#include "Shield.hpp"
#include "Projectile.hpp"
#include "Side.hpp"
#include "Bounds.hpp"
#include "Pickup.hpp"

Pickup::Shield::Shield(b2Body* body, int time, float strength) :
	Pickup::PickupI(body, time),
	strength_(strength > 0.f ? strength : 4)
{
	//Body is initially made by pickup base class
}

void Pickup::Shield::onCollect()
{
	body_->DestroyFixture(body_->GetFixtureList());
	b2Body* ownerBody = owner_->getBody();
	body_->SetTransform(ownerBody->GetPosition(), ownerBody->GetAngle());
	
	//Usual box2d setup
	b2FixtureDef def;
	def.userData = "shield";
	def.density = 0.f;
	def.friction = 0.f;
	def.restitution = 0.f;
	def.isSensor = false;

	body_->SetFixedRotation(false);
	
	b2CircleShape shape;
	shape.m_radius = owner_->getSize() * 2.f;
	def.shape = &shape;

	b2RevoluteJointDef dis;
	dis.localAnchorA = b2Vec2(0, 0);
	dis.localAnchorB = b2Vec2(0, 0);
	dis.bodyA = body_;
	dis.bodyB = owner_->getBody();
	dis.collideConnected = false;

	body_->GetWorld()->CreateJoint(&dis);
	body_->CreateFixture(&def);

	collected_ = true;
}

bool Pickup::Shield::collide(Entity* other, b2Contact& contact, std::string tag)
{

	bool handled = false;

	if (tag == "player" || tag == "enemy")
	{
		if (!collected_)
		{
			Shape* shape = dynamic_cast<Shape*>(other);
			setOwner(shape);
		}

		handled = true;
	}

	else if (tag == "projectile")
	{
		if (collected_)
		{
			Projectile* proj = dynamic_cast<Projectile*>(other);
			//Enemy projectiles after collection
			if (proj->getFaction() != faction_)
			{
				//int x = 4;
			}

			else {
				contact.SetEnabled(false);
			}
		}

		//Contacts for my projectiles, and all projectiles when not collected
		else contact.SetEnabled(false);
	
		handled = true;
	}

	else if (tag == "pickup")
	{
		contact.SetEnabled(false);
	}

	else if (tag == "shield")
	{
		contact.SetEnabled(false);
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
			//setPosition(owner_->getPosition());
		}

		//Edge case where projectile is in shield radius on collect and kills you after pickup
		else {
			time_ = 0;
		}
	}

	if (time_ == 0)
	{
		owner_ = nullptr;
		body_->GetFixtureList()->SetSensor(true);
		body_->GetWorld()->DestroyJoint(body_->GetJointList()->joint);
	}
}

float Pickup::Shield::getStrength()
{
	return strength_;
}
