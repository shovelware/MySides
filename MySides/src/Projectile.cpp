#include "Projectile.hpp"

#include "Bounds.hpp"
#include "Shape.hpp"
#include "Projectile.hpp"
#include "Side.hpp"
#include "Pickup.hpp"
#include "Shield.hpp"

Projectile::Projectile(b2Body* body, const ProjectileDef& def) :
	Entity(body),
	fired_(false), impacted_(false),
	origin_(def.origin), heading_(def.heading),
	maxHP_(def.hpMAX), hp_(def.hpMAX),
	damage_(def.damage),
	size_(def.width, def.height),
	lifeTime_(def.lifeTime),
	owner_(def.owner), 
	target_(def.target),
	shrapnel_(def.shrapnel)
{
	if (def.height > 0)
	{
		setAsRect(size_, def.bounce, def.ghost);
	}

	else
	{
		setAsCircle(size_, def.bounce, def.ghost);
	}

	//Do maths to orient body
	body_->SetTransform(def.origin, body_->GetAngle());

	//Set up colours
	colPrim_ = def.colPrim;
	colSecn_ = def.colSecn;
	colTert_ = def.colTert;

	fire(def.velScale);
}

void Projectile::setAsCircle(b2Vec2 size, float bounce = 0.f, bool ghost = false)
{
	//Shape
	b2CircleShape bullet;
	bullet.m_radius = (0.1f * size.x);

	//Fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &bullet;

	//Collision
	fixtureDef.userData = "projectile";
	fixtureDef.isSensor = ghost;
	addMaterial(fixtureDef, bounce);

	//Bind fixture
	body_->CreateFixture(&fixtureDef);

	//End box2d setup

	speed_ = 0.000025f * size.x;
}

void Projectile::setAsRect(b2Vec2 size, float bounce = 0.f, bool ghost = false)
{
	//Shape
	b2PolygonShape box;
	box.SetAsBox(0.1f * size.x, 0.1f * size.y);

	//Fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;

	//Collision
	fixtureDef.userData = "projectile";
	fixtureDef.isSensor = ghost;
	addMaterial(fixtureDef, bounce);

	//Bind fixture
	body_->CreateFixture(&fixtureDef);

	//End box2d setup
	body_->SetTransform(body_->GetPosition(), atan2f(-heading_.x, heading_.y));
	speed_ = 0.00025f / (size.x * size.y);
}

void Projectile::addMaterial(b2FixtureDef & def, float bounce)
{
	def.density = 0.5f;
	def.friction = 0.0f;
	def.restitution = bounce;
}

void Projectile::fire(float mult)
{
	if (fired_ == false)
	{
		heading_.Normalize();

		heading_ *= speed_ * mult;
		
		body_->SetLinearVelocity(heading_);
		//newVelocity.x = body_->GetMass() / (newVelocity.x > 0 ? newVelocity.x : 1);
				
		//(newVelocity.x != 0 ? newVelocity.x = body_->GetMass() / newVelocity.x : newVelocity.x);
		//(newVelocity.y != 0 ? newVelocity.y = body_->GetMass() / newVelocity.y : newVelocity.y);
		body_->ApplyForce(heading_, body_->GetWorldCenter(), true);
		fired_ = true;
	}
}

void Projectile::impact()
{
	if (impacted_ == false)
	{
		impacted_ = true;
	}
}

void Projectile::takeDamage(unsigned int damage)
{
	hp_ -= damage;
}

int Projectile::getDamage() const
{
	return damage_;
}

int Projectile::getShrapnel() const { return shrapnel_; }

Entity * Projectile::getOwner()
{
	return owner_;
}

void Projectile::setOwner(Entity* o)
{
	owner_ = o;
}

Entity* Projectile::getTarget()
{
	return target_;
}

void Projectile::setTarget(Entity* s)
{
	target_ = s;
}

b2Vec2 Projectile::getDirection() const
{
	return heading_;
}

void Projectile::update(int milliseconds)
{
	lifeTime_ -= milliseconds;

	if (fired_)
	{
		//body_->ApplyForce(heading_, body_->GetWorldCenter(), true);
	}
	b2Vec2 vel = body_->GetLinearVelocity();

	body_->SetTransform(body_->GetPosition(), atan2f(-vel.x, vel.y));

	//std::cout << body_->GetLinearVelocity().Length();

	if (vel.Length() <= 0|| hp_ <= 0 || lifeTime_ <= 0)
	{
		alive_ = false;
		active_ = false;		
	}
}

bool Projectile::collide(Entity * other, b2Contact& contact)
{
	bool handled = false;

	if (Shape* shape = dynamic_cast<Shape*>(other))////
	{
		//Projectiles do not hurt their owner
		if (owner_ != shape)
		{
			impact();
			takeDamage(1);
		}

		else contact.SetEnabled(false);

		handled = true;
	}

	else if (Projectile* proj = dynamic_cast<Projectile*>(other))
	{
		//Only collide if we're not friends
		if (owner_ != proj->getOwner())
		{
			impact();
			takeDamage(proj->getDamage());
			//Maybe set contact to false so stronger proj continues?
		}

		//Projectiles with same owner do not interact
		else contact.SetEnabled(false);

		handled = true;
	}

	else if (Side* side = dynamic_cast<Side*>(other))
	{
		handled = true;
	}

	else if (Pickup::PickupI* pickup = dynamic_cast<Pickup::PickupI*>(other))
	{

		if (Pickup::Shield*  shield = dynamic_cast<Pickup::Shield*>(other))
		{
			if (shield->getOwner() != owner_)
			{
				takeDamage(shield->getStrength());
			}

			handled = true;
		}

		else handled = true;
	}

	else if (Bounds* bounds = dynamic_cast<Bounds*>(other))
	{
		impact();
		takeDamage(4);
		handled = true;
	}

	return handled;
}