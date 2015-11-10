#include "Shape.hpp"

///
#include <sstream>
#include "Log.hpp"
extern Log l;
///

Shape::Shape(b2Body * body) : Entity(body)
{
	//Create a shape, the outline
	b2PolygonShape shap;
	
	b2Vec2 verts[3];
	verts[0].Set(0, -1.5f);
	verts[1].Set(-.75f, 0);
	verts[2].Set(.75f, 0);

	shap.Set(verts, 3);

	//Create a fixture, the link for body -> shape
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shap;

	//Add material properties to the fixture
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.1f;

	//Create and add fixture using body's factory
	body_->CreateFixture(&fixtureDef);

	accel_ = 0.001;
	topSpeed_ = 0.5;
}

void Shape::move(b2Vec2 direction)
{
	if (!(direction == b2Vec2_zero))
	{
		//Make direction an acceleration vector
		//direction.Normalize();
		direction *= accel_;

		//Apply the force
		body_->ApplyForceToCenter(direction, true);

		//If we're faster than top speed, reset to top speed
		//if (body_->GetLinearVelocity().Length() > topSpeed_)
		//{
		//	b2Vec2 velocity = body_->GetLinearVelocity();
		//	velocity.Normalize();
		//	velocity *= topSpeed_;
		//
		//	body_->SetLinearVelocity(velocity);
		//}
	}

	std::ostringstream vel;
	vel << direction.x << " " << direction.y;
	
	l.out(l.message, 'P', vel.str().c_str());
}

void Shape::stopMove()
{
	body_->SetLinearVelocity(b2Vec2_zero);
}

void Shape::orient(b2Vec2 direction)
{

}

void Shape::rotate(float amount)
{
	if (amount != 0)
	{
		body_->ApplyTorque(DR * amount, true);
	}


	//std::ostringstream vel;
	//vel << body_->GetAngularVelocity();
	//
	//l.out(l.message, 'P', vel.str().c_str());
}

void Shape::stopRotate()
{
	body_->SetAngularVelocity(0);
}