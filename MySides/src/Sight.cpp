#include "Sight.hpp"
#include "Projectile.hpp"
#include "Side.hpp"
#include "Bounds.hpp"
#include "Pickup.hpp"

Pickup::Sight::Sight(b2Body* body, int time, float length) :
	Pickup::PickupI(body, time),
	length_(length > 0.f ? length : 10.f),
	contact_(false),
	colourChange_(true)
{
	//Body is initially made by pickup base class
}

void Pickup::Sight::onCollect()
{
	body_->DestroyFixture(body_->GetFixtureList());
	body_->SetFixedRotation(true);
	body_->SetTransform(owner_->getBody()->GetPosition(), body_->GetAngle());

	b2FixtureDef def;
	def.userData = "sight";
	def.density = 0.f;
	def.friction = 0.f;
	def.restitution = 0.f;
	def.isSensor = true;
	
	b2EdgeShape shape;
	shape.Set(b2Vec2(0, 0), b2Vec2(0, length_));
	def.shape = &shape;


	//b2RevoluteJointDef dis;
	//dis.localAnchorA = b2Vec2(0, 0);
	//dis.localAnchorB = b2Vec2(0, 0);
	//dis.bodyA = owner_->getBody();
	//dis.bodyB = body_;
	//dis.enableLimit = true;
	//dis.lowerAngle = 0;
	//dis.upperAngle = 0;
	//dis.collideConnected = false;
	//
	//body_->GetWorld()->CreateJoint(&dis);
	body_->CreateFixture(&def);

	float angle = owner_->getBody()->GetAngle();
	body_->SetTransform(owner_->getBody()->GetPosition(), angle);

	//Enable sighting
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

	collected_ = true;
}

bool Pickup::Sight::collide(Entity* other, b2Contact& contact, std::string tag)
{
	bool handled = false;

	if (tag == "player" || tag == "enemy")
	{
		Shape* shape = dynamic_cast<Shape*>(other);

		if (!collected_)
		{
			setOwner(shape);
		}


		handled = true;
	}

	return handled;
}
bool Pickup::Sight::getContact() const { return contact_; }

void Pickup::Sight::update(int milliseconds)
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
			float angle = owner_->getBody()->GetAngle();
			body_->SetTransform(owner_->getBody()->GetPosition(), angle);

			contact_ = false;

			if (colourChange_)
			{
				b2EdgeShape* shap = static_cast<b2EdgeShape*>(body_->GetFixtureList()->GetShape());

				for (b2ContactEdge* ed = body_->GetContactList(); ed != nullptr; ed = ed->next)
				{
					b2Body* s = nullptr;

					if (ed->contact->GetFixtureA()->GetUserData() == enemyTag_.c_str())
						s = ed->contact->GetFixtureA()->GetBody();

					else if (ed->contact->GetFixtureB()->GetUserData() == enemyTag_.c_str())
						s = ed->contact->GetFixtureB()->GetBody();

					if (s != nullptr)
					{
						b2Vec2 dist = body_->GetPosition() + s->GetPosition();
						// divide each vector by length
						b2Vec2 v1 = body_->GetPosition() + shap->m_vertex2;
						b2Vec2 v2 = dist;

						v1.Normalize();
						v2.Normalize();

						float ang = acos(b2Dot(v1, v2));

						if (ang < 1.2f && dist.Length() <= length_)
							contact_ = true;

					}

					if (contact_ == true)
						break;
				}
			}

			
		}

		else
		{
			time_ == 0;
		}

	}

	//Deletion flag
	if (time_ == 0)
	{
		owner_ = nullptr;
	}

}
