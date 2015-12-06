#include "..\include\Projectile.hpp"

Projectile::Projectile(b2Body* body, b2Vec2 heading) : Entity(body), impact_(false)
{
	//Create a shape, the outline
	b2CircleShape shap;

	shap.m_radius = 0.1f;

	//Create a fixture, the link for body -> shape
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shap;

	//Add material properties to the fixture
	fixtureDef.density = 0.5f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;

	//Add userdata to fixture for contacts
	fixtureDef.userData = "projectile";

	//Create and add fixture using body's factory
	body_->CreateFixture(&fixtureDef);

	speed_ = 0.0005f; // max velocity in m/s


	//Fire the bullet
	heading.Normalize();

	body_->ApplyLinearImpulse(speed_ * heading, body_->GetWorldCenter(), true);

	myBool = false;
	_ASSERT(!impact_);
}

void Projectile::hit()
{
	if (impact_ == false)
	{
		_ASSERT(!impact_);
		impact_ = true;
	}
	if (!myBool) {
		myBool = true;
	}

	alive_ = false;
	active_ = false;
}

void Projectile::update(int milliseconds)
{
}

