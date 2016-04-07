#include "Projectile.hpp"

#include "Shape.hpp"
#include "Projectile.hpp"
#include "Shield.hpp"

Projectile::Projectile(b2Body* body, const ProjectileDef& def, ForceFunc& forceCallback) :
	Entity(body),
	forceCallback_(forceCallback),
	fired_(false),
	oneHit_(def.oneHit),
	origin_(def.origin), heading_(def.heading),
	lastPen_ (0, 0, 0),
	maxHP_(def.hpMAX), hp_(def.hpMAX),
	damage_(def.damage),
	size_(def.width, def.height),
	lifeTime_(def.lifeTime),
	owner_(def.owner), 
	tracking_(def.tracking),
	penetration_(def.penetration),
	bounce_(def.bounce)
{
	if (def.height > 0)
	{
		setAsRect(size_, bounce_, penetration_);
	}

	else
	{
		setAsCircle(size_, bounce_, penetration_);
	}

	//Faction
	faction_ = def.faction;

	//Explosive properties
	force_ = def.detonation;
	shrapnel_ = def.shrapnel;

	//Tracking
	if (tracking_.radius > 0 && tracking_.speed > 0)
	{
		setTracking(tracking_.radius);
	}

	//Do maths to orient body
	body_->SetTransform(origin_, atan2f(-heading_.x, heading_.y));
	body_->SetLinearVelocity(def.inVelocity);

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
	fixtureDef.isSensor = (penetration_ > 0);
	fixtureDef.shape = &bullet;

	//Collision
	fixtureDef.userData = "projectile";
	fixtureDef.density = 25.f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = bounce;

	//Bind fixture
	body_->CreateFixture(&fixtureDef);

	//End box2d setup
	speed_ = 0.05f * (M_PI * (size.x * size.x));
}

void Projectile::setAsRect(b2Vec2 size, float bounce = 0.f, bool ghost = false)
{
	//Shape
	b2PolygonShape box;
	box.SetAsBox(0.1f * size.x, 0.1f * size.y);

//Fixture
b2FixtureDef fixtureDef;
fixtureDef.isSensor = (penetration_ > 0);
fixtureDef.shape = &box;

//Collision
fixtureDef.userData = "projectile";
fixtureDef.density = 25.f;
fixtureDef.friction = 1.0f;
fixtureDef.restitution = bounce;

//Bind fixture
body_->CreateFixture(&fixtureDef);

//End box2d setup
speed_ = 0.05f * (size.x * size.y);
}

void Projectile::setTracking(float radius)
{
	b2CircleShape track;
	track.m_radius = (radius);

	//Fixture
	b2FixtureDef fixtureDef;
	fixtureDef.isSensor = true;
	fixtureDef.shape = &track;

	//Collision
	fixtureDef.userData = "projtracking";

	//Bind fixture
	body_->CreateFixture(&fixtureDef);

	//Tags for b2 tracking
	switch (faction_)
	{
	case 0:
		enemyTag_ = "";
		break;
	case 1:
		enemyTag_ = "enemy";
		break;
	case 2:
		enemyTag_ = "player";
		break;
	case 3:
		enemyTag_ = "obstacle";
	}
}

void Projectile::fire(float mult)
{
	if (fired_ == false)
	{
		heading_.Normalize();


		b2Vec2 newVelocity = heading_;
		newVelocity *= speed_ * mult;

		//body_->SetLinearVelocity(body_->GetLinearVelocity() + heading_);
		//newVelocity.x = body_->GetMass() / (newVelocity.x > 0 ? newVelocity.x : 1);

		//(newVelocity.x != 0 ? newVelocity.x = body_->GetMass() / newVelocity.x : newVelocity.x);
		//(newVelocity.y != 0 ? newVelocity.y = body_->GetMass() / newVelocity.y : newVelocity.y);

		//std::cout << std::endl << "NEWVEL===" << std::endl << newVelocity.Length() << std::endl;

		body_->ApplyLinearImpulse(newVelocity, body_->GetWorldCenter(), true);
		fired_ = true;
	}
}

void Projectile::track()
{
	std::string shortTag;

	bool found = false;
	for (b2ContactEdge* ed = body_->GetContactList(); ed != nullptr; ed = ed->next)
	{
		Shape* s = nullptr;
		float dist = FLT_MAX;
		std::string tag;

		if (ed->contact->GetFixtureA()->GetUserData() == "projtracking")
		{
			tag = std::string(static_cast<char*>(ed->contact->GetFixtureB()->GetUserData()));

			if (tag == enemyTag_)
			{
				b2Vec2 pos = ed->contact->GetFixtureB()->GetBody()->GetPosition();
				if (pos.Length() < dist)
				{
					target_ = b2Vec3(pos.x, pos.y, 1);
					dist = pos.Length();
					shortTag = tag;
				}
			}
		}

		else if (ed->contact->GetFixtureB()->GetUserData() == "projtracking")
		{
			tag = std::string(static_cast<char*>(ed->contact->GetFixtureA()->GetUserData()));

			if (tag == enemyTag_)
			{
				b2Vec2 pos = ed->contact->GetFixtureA()->GetBody()->GetPosition();
				if (pos.Length() < dist)
				{
					found = true;
					target_ = b2Vec3(pos.x, pos.y, 1);
					dist = pos.Length();
					shortTag = tag;
				}
			}
		}
	}

	if (found == false)
		target_.z = 0;

	std::cout  << target_.x << "\t" << target_.y << "\t " << target_.z << "\t" << shortTag.c_str() << ::endl;
}

void Projectile::orient(b2Vec2 target)
{
	float bodyAngle = body_->GetAngle();
	float desiredAngle = atan2f(-target.x, target.y);
	float nextAngle = body_->GetAngle() + body_->GetAngularVelocity() / _TICKTIME_;

	float totalRotation = desiredAngle - bodyAngle;

	while (totalRotation < M_PI) totalRotation += 2 * M_PI;
	while (totalRotation > M_PI) totalRotation -= 2 * M_PI;

	float change = 10 * DR; //allow 20 degree rotation per time step
	float newAngle = bodyAngle + std::min(change, std::max(-change, totalRotation));
	
	float vel = body_->GetLinearVelocity().Length();
	
	b2Vec2 newVel(vel * cos(newAngle), vel * sin(newAngle));
	
	//body_->SetLinearVelocity(newVel);

	//body_->SetTransform(body_->GetPosition(), newAngle);
	//vx = magnitude * cos(angle)
	//vy = magnitude * sin(angle)
}

void Projectile::takeDamage(unsigned int damage)
{
	hp_ -= damage;
	if (hp_ <= 0)
	{
		alive_ = false;
	}
}

int Projectile::getDamage() const { return damage_; }
ProjectileDef::ProjShrapnel const& Projectile::getShrapnel() const { return shrapnel_; }
Entity * Projectile::getOwner() { return owner_; }
void Projectile::setOwner(Entity* o) {	owner_ = o; }
b2Vec2 Projectile::getDirection() const { return heading_; }

void Projectile::update(int milliseconds)
{
	lifeTime_ -= milliseconds;

	if (alive_)
	{
		//Turn off penetration
		if (penetration_ < 0)
		{
			body_->GetFixtureList()->SetSensor(false);
		}

		if (lastPen_.z != 0)
		{
			forceCallback_(b2Vec2(lastPen_.x, lastPen_.y), force_.force, force_.radius, force_.lifeTime);
			lastPen_.z = 0;
		}

		if (tracking_.radius != 0 && tracking_.speed != 0)
		{
			track();
		}

		if (target_.z != 0)
		{
			b2Vec2 dir = b2Vec2(body_->GetPosition().x - target_.x, body_->GetPosition().y - target_.y);
			orient(dir);
		}

		b2Vec2 vel = body_->GetLinearVelocity();
		body_->SetTransform(body_->GetPosition(), atan2f(-vel.x, vel.y));

		//std::cout << vel.Length() << std::endl;

		if ((bounce_ <= 0 && vel.Length() <= 0) || hp_ <= 0 || lifeTime_ <= 0)
		{
			alive_ = false;
		}
	}

	//Do our force once before we deactivate
	else if (alive_ == false && active_ == true)
	{
		forceCallback_(body_->GetPosition(), force_.force, force_.radius, force_.lifeTime);
		active_ = false;
	}

	else active_ = false;
}

bool Projectile::collide(Entity * other, b2Contact& contact, std::string tag)
{
	bool handled = false;
	
	//Get fixture string
	std::string tig = std::string(static_cast<char*>(contact.GetFixtureA()->GetUserData()));

	//If it's the other, get what we are in contact
	if (tig == tag)
		tig = std::string(static_cast<char*>(contact.GetFixtureB()->GetUserData()));

	//If it's our tracking, don't bother
	if (tig == "projtracking")
	{
		handled = true;
	}

	else if (tag == "player" || tag == "enemy" || tag == "shape")////
	{
		Shape* shape = static_cast<Shape*>(other);
		//Projectiles do not hurt their friends
		if (faction_ != shape->getFaction() && shape->getFaction() != GOD)
		{
			//Onehit projectiles die on hit, precedence over all
			if (oneHit_)
			{
				alive_ = false;
			}

			//If we can't penetrate
			if (penetration_-- <= 0)
			{
				takeDamage(1);
				if (size_.y > 0)
				{
					//b2Vec2 vel = body_->GetLinearVelocity();
					//vel *= (0.1* size_.x * size_.y);
					//
					//body_->SetLinearVelocity(vel);
				}
			}

			//Do our force on penetration
			else
			{
				lastPen_ = b2Vec3(body_->GetPosition().x, body_->GetPosition().y, 1);
			}

			if (alive_ == false)
				contact.SetEnabled(false);
		}

		else contact.SetEnabled(false);

		handled = true;
	}

	else if (tag == "projectile")
	{
		Projectile* proj = static_cast<Projectile*>(other);
		//If we're not friends
		if (faction_ != proj->getFaction())
		{
			//If we can't penetrate
			if (--penetration_ < 0)
			{
				takeDamage(proj->getDamage());

				if (alive_ == false)
					contact.SetEnabled(false);
			}

			//Do our force on penetration
			else
			{
				lastPen_ = b2Vec3(body_->GetPosition().x, body_->GetPosition().y, 1);
			}
			//Maybe set contact to false so stronger proj continues?
		}

		//Else we must have same owner, do not interact
		else contact.SetEnabled(false);

		handled = true;
	}

	else if (tag == "pickup")
	{
		contact.SetEnabled(false);
		handled = true;
	}

	else if (tag == "shield")
	{
		Pickup::Shield* shield = static_cast<Pickup::Shield*>(other);

		//If we don't own the shield
		if (shield->getOwner() != owner_)
		{
			//Penetrate if we can
			if (penetration_ > 0)
			{
				//Shields block more pen
				penetration_ -= shield->getStrength();
			}
		
			//if (contact.IsEnabled())
			//{
			//	b2Vec2 back = body_->GetLinearVelocity();
			//	back *= -0.5;
			//	body_->SetLinearVelocity(back);
			//}
		}
		
		handled = true;
	}

	else if (tag == "bounds")
	{
		takeDamage(1);
		handled = true;
	}

	return handled;
}