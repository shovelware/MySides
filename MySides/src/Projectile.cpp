#include "..\include\Projectile.hpp"

Projectile::Projectile(b2Body* body, b2Vec2 heading) :
	Entity(body),
	fired_(false), impacted_(false),
	origin_(body_->GetPosition()), heading_(heading)
{

	//Create a fixture, the link for body -> shape
	b2FixtureDef fixtureDef;
	b2CircleShape circ;
	circ.m_radius = 0.05f;

	//Add material properties to the fixture
	fixtureDef.density = 0.5f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;

	fixtureDef.shape = &circ;

	//Add userdata to fixture for contacts
	fixtureDef.userData = "projectile";

	//Create and add fixture using body's factory
	body_->CreateFixture(&fixtureDef);

	//End box2d setup

	speed_ = 0.00025f; // max velocity in m/s

	//Fire the bullet

	fire(1);

	lifeTime_ = 1000;
	damage_ = 1;
	maxHP_ = 1;
	hp_ = maxHP_;
}

Projectile::Projectile(ProjectileDef def) :
	Entity(def.body),
	fired_(false), impacted_(false),
	origin_(def.origin), heading_(def.heading),
	maxHP_(def.maxHP), hp_(def.maxHP),
	size_(def.size),
	damage_(def.damage), 
	lifeTime_(def.lifeTime),
	owner_(def.owner), 
	target_(def.target)
{
	setAsBullet(size_);
	fire(def.velScale);
}

void Projectile::setAsBullet(float size)
{
	//Shape data
	b2CircleShape bullet;
	bullet.m_radius = (0.1f * size);

	//Fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &bullet;

	//Collision data
	fixtureDef.userData = "projectile";
	addMaterial(fixtureDef);

	//Create and add fixture using body's factory
	body_->CreateFixture(&fixtureDef);

	speed_ = 0.000025f;
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
		if (owner_ != shape)
		{
			impact();
			takeDamage(1);
		}

		handled = true;
	}

	else if (Projectile* proj = dynamic_cast<Projectile*>(other))
	{
		impact();
		takeDamage(proj->getDamage());
		//Maybe set contact to false so stronger proj continues?
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


