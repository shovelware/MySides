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

	def.density = 0.f;
	def.friction = 0.8f;
	def.restitution = 1.f;
	def.isSensor = false;

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
				std::cout << "dmg" << std::endl;
			}

			else contact.SetEnabled(false);
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
			body_->GetFixtureList()->SetSensor(true);
			body_->GetWorld()->DestroyJoint(body_->GetJointList()->joint);
		}
	}

	Pickup::PickupI::update(milliseconds);
}
