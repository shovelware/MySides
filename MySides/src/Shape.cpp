#include "Shape.hpp"

Shape::Shape(b2Body * body) : Entity(body)
{
	accel_ = 0.1;
	topSpeed_ = 0.5;
}

void Shape::move(b2Vec2 direction)
{
	//Make direction an acceleration vector
	direction.Normalize();
	direction *= accel_;

	//Apply the force
	body_->ApplyForceToCenter(direction, true);

	//If we're faster than top speed, reset to top speed
	if (body_->GetLinearVelocity().Length() > topSpeed_)
	{
		b2Vec2 velocity = body_->GetLinearVelocity();
		velocity.Normalize();
		velocity *= topSpeed_;

		body_->SetLinearVelocity(velocity);
	}
}

void Shape::rotate(float amount)
{

}