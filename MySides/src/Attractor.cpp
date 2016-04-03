#include "Attractor.hpp"
#include "Projectile.hpp"
#include "Bounds.hpp"
#include "Pickup.hpp"

Pickup::Attractor::Attractor(b2Body* body, int time, float radius, float strengthScale) :
	Pickup::PickupI(body, time),
	radius_(radius > 0.f ? radius : 7.5f),
	strengthScale_(strengthScale >= -1.f ? strengthScale : 1.f)
{
	//Body is initially made by pickup base class
}

void Pickup::Attractor::onCollect()
{
	body_->DestroyFixture(body_->GetFixtureList());
	b2Body* ownerBody = owner_->getBody();
	float angle = atan2f(owner_->getOrientation().y,owner_->getOrientation().x);
	body_->SetTransform(ownerBody->GetPosition(), angle);

	b2FixtureDef def;
	def.userData = "attractor";

	def.density = 0.f;
	def.friction = 0.f;
	def.restitution = 0.f;
	def.isSensor = true;

	b2CircleShape shape;
	shape.m_radius = radius_;
	def.shape = &shape;

	b2RevoluteJointDef weld;
	weld.localAnchorA = b2Vec2(0, 0);
	weld.localAnchorB = b2Vec2(0, 0);
	weld.bodyA = body_;
	weld.bodyB = owner_->getBody();
	weld.collideConnected = false;

	body_->GetWorld()->CreateJoint(&weld);
	body_->CreateFixture(&def);

	collected_ = true;
}

bool Pickup::Attractor::collide(Entity* other, b2Contact& contact, std::string tag)
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

	return handled;
}

void Pickup::Attractor::update(int milliseconds)
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

			for (b2ContactEdge* ed = body_->GetContactList(); ed != nullptr; ed = ed->next)
			{
				b2Body* s = nullptr;

				if (ed->contact->GetFixtureA()->GetUserData() == "side")
					s = ed->contact->GetFixtureA()->GetBody();


				else if (ed->contact->GetFixtureB()->GetUserData() == "side")
					s = ed->contact->GetFixtureB()->GetBody();

				if (s != nullptr)
				{
					b2Vec2 dir = body_->GetPosition() - s->GetPosition();
					float dist = dir.Length();
					if (dist < radius_)
					{
						
						dir.Normalize();
						dir *= 0.05f * ((dist / radius_)) * strengthScale_;
						b2Vec2 vel = s->GetLinearVelocity();

						if (0.2 > vel.Length()  && vel.Length() < dir.Length() / 2)
							s->ApplyForceToCenter(dir, true);
					}
				}
			}
		}

		else time_ == 0;
	}

	//Deletion flag
	if (time_ == 0)
	{
		owner_ = nullptr;
		body_->GetWorld()->DestroyJoint(body_->GetJointList()->joint);
	}


}

float Pickup::Attractor::getRadius()
{
	return radius_;
}
