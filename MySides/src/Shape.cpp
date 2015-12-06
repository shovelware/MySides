#include "Shape.hpp"

///
#include <sstream>
#include "Log.hpp"
extern Log l;
///

//Creates a shape using passed body //int sides, float radius
Shape::Shape(b2Body* body) : Entity(body)
{
	//Create a shape, the outline
	b2PolygonShape shap;
	
	////Was experimenting with poles and orientation, DON'T FORGET
	//b2Vec2 verts[3];
	//verts[0].Set(0, -2.f);
	//verts[1].Set(-.75f, 0);
	//verts[2].Set(.75f, 0);
	//
	//pole_ = verts[0];
	//
	//shap.Set(verts, 3);
	
	shap.SetAsBox(.5f, .5f);

	//Create a fixture, the link for body -> shape
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shap;

	//Add material properties to the fixture
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.99f;

	//Add userdata to fixture for contacts
	fixtureDef.userData = "shape";

	//Angular damping in body???
	//Create and add fixture using body's factory
	body_->CreateFixture(&fixtureDef);

	maxVel_ = 0.025f; // max velocity in m/s
	maxRot_ = 0.0001f;
}

void Shape::move(b2Vec2 direction)
{
	//Normalise dir it if it's >1
	if (!(direction == b2Vec2_zero))
	{
		if (direction.Length() > 1.0f)
		{
			direction.Normalize();
		}
	}

	//Get the body's velocity
	b2Vec2 vel = body_->GetLinearVelocity();

	//Set desired velocity (respecting max velocity)
	b2Vec2 desiredVel = direction;
	desiredVel *= maxVel_;

	//Get the difference in current and desired
	b2Vec2 velChange = desiredVel - vel;

	//Apply appropriate impulse
	b2Vec2 impulse = body_->GetMass() * velChange;
	body_->ApplyLinearImpulse(impulse, body_->GetWorldCenter(), true);

	//l.out(l.message, 'P', "Player move");
}

void Shape::stopMove()
{
	body_->SetLinearVelocity(b2Vec2_zero);
}

void Shape::orient(b2Vec2 direction)
{
	//get angle between center->pole and 
	b2Vec2 orientation = body_->GetWorldPoint(pole_) - body_->GetWorldCenter();
	orientation.Normalize();

	std::ostringstream o;
	o << orientation.x << ", " << orientation.y;
	l.out(l.message, 'P', o.str().c_str());
}

void Shape::rotate(float amount)
{	
	//Make amount in range -1 <-> +1
	if (abs(amount) > 1)
	{
		if (amount < 0)
		{
			amount = -1;
		}

		else amount = 1;
	}

	//Get the body's rotation
	float rotation = body_->GetAngle();

	//Set desired rotation
	float desiredRot = amount;
	desiredRot *= maxRot_;

	//Get the difference in current and desired
	float rotChange = desiredRot - rotation;

	//Apply appropriate impulse
	float impulse = rotChange;
	body_->ApplyTorque(DR * amount, true);

	//std::ostringstream vel;
	//vel << body_->GetAngularVelocity();
	//
	//l.out(l.message, 'P', vel.str().c_str());
}

void Shape::stopRotate()
{
	body_->SetAngularVelocity(0);
}

b2Vec2 Shape::getFirePoint(float x, float y)
{
	b2Vec2 p = body_->GetPosition();
	b2Vec2 d(x, y);
	d.Normalize();

	d *= 2; //MAGIC NUMBER FIX SOON

	return p + d;
}

void Shape::update(int milliseconds)
{
}

//
//void Shape::draw(GameDrawer d)
//{
//
//}
