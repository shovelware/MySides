#include "..\include\Projectile.hpp"

Projectile::Projectile(b2Body* body, b2Vec2 heading) : Entity(body)
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

	//Create and add fixture using body's factory
	body_->CreateFixture(&fixtureDef);

	speed_ = 0.001f; // max velocity in m/s


	//Fire the bullet
	heading.Normalize();

	body_->ApplyLinearImpulse(speed_ * heading, body_->GetWorldCenter(), true);
}

//Need to tidy these up
void Projectile::move(b2Vec2) {}
void Projectile::rotate(float) {}