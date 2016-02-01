#include "Projectile.hpp"

#include "Bounds.hpp"
#include "Shape.hpp"
#include "Projectile.hpp"
#include "Side.hpp"

Projectile::Projectile(b2Body* body, ProjectileDef& def) :
	Entity(body),
	fired_(false), impacted_(false),
	origin_(def.origin), heading_(def.heading),
	maxHP_(def.hpMAX), hp_(def.hpMAX),
	size_(def.size),
	lifeTime_(def.lifeTime),
	owner_(def.owner), 
	target_(def.target)
{
	setAsBullet(size_, def.damageScale);

	//Do maths to orient body here
	body_->SetTransform(def.origin, body_->GetAngle());

	//Set up colours
	colPrim_ = def.colPrim;
	colSecn_ = def.colSecn;
	colTert_ = def.colTert;

	fire(def.velScale);
}

void Projectile::setAsBullet(float size, float damageScale = 1.f)
{
	//Shape
	b2CircleShape bullet;
	bullet.m_radius = (0.1f * size);

	//Fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &bullet;

	//Collision
	fixtureDef.userData = "projectile";
	addMaterial(fixtureDef);

	//Bind fixture
	body_->CreateFixture(&fixtureDef);

	//End box2d setup

	speed_ = 0.000025f;
	damage_ = 1 * damageScale;
}

void Projectile::addMaterial(b2FixtureDef & def)
{
	def.density = 0.5f;
	def.friction = 0.0f;
	def.restitution = 0.0f;
}

void Projectile::fire(float mult)
{
	if (fired_ == false)
	{
		heading_.Normalize();
		b2Vec2 newVelocity(heading_);

		newVelocity *= speed_ * mult;

		//newVelocity.x = body_->GetMass() / (newVelocity.x > 0 ? newVelocity.x : 1);
				
		//(newVelocity.x != 0 ? newVelocity.x = body_->GetMass() / newVelocity.x : newVelocity.x);
		//(newVelocity.y != 0 ? newVelocity.y = body_->GetMass() / newVelocity.y : newVelocity.y);

		body_->ApplyForce(newVelocity, body_->GetWorldCenter(), true);
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

void Projectile::update(int milliseconds)
{
	lifeTime_ -= milliseconds;

	if ((impacted_ && hp_ <= 0) || lifeTime_ <= 0 )
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

	else if (Bounds* bounds = dynamic_cast<Bounds*>(other))
	{
		impact();
		takeDamage(4);
		handled = true;
	}

	return handled;
}


