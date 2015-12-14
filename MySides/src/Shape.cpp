#include "Shape.hpp"

///
#include <sstream>
#include "Log.hpp"
extern Log l;
///

//Creates a shape using passed body //int sides, float radius
Shape::Shape(b2Body* body, int vertices, float radius) : Entity(body)
{
	//Create a shape, the outline
	b2PolygonShape shap;
	
	//Was experimenting with poles and orientation, DON'T FORGET
	setTriangleEqu(shap, 1.f);
	
	//shap.SetAsBox(radius, radius);

	//Create a fixture, the link for body -> shape
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shap;

	//Add material properties to the fixture
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 1.f;

	//Add userdata to fixture for contacts
	fixtureDef.userData = "shape";

	//Create and add fixture using body's factory
	body_->CreateFixture(&fixtureDef);

	//End box2d setup

	maxVel_ = 0.05f * radius; // max velocity in m/s // * radius provisionally til we make an actual calculation
	maxRot_ = 0.0001f;

	//
	refireTime_ = 300;
	coolDown_ = 0;
	maxHP_ = 4;
	hp_ = maxHP_;
	sides_ = 0;
	controlled_ = false;
	ai_ = false;
}

//Sets shape to be an equilateral triangle with sides of 1*scale
void Shape::setTriangleEqu(b2PolygonShape& s, float scale)
{
	b2Vec2 verts[3];
	
	verts[0].Set(0, -1.f);
	verts[1].Set(-.5f, 0);
	verts[2].Set(.5f, 0);

	verts[0] *= scale;
	verts[1] *= scale;
	verts[2] *= scale;

	pole_ = verts[0];

	s.Set(verts, 3);
}

void Shape::setTriangleIso(b2PolygonShape& s, float scale)
{
	b2Vec2 verts[3];

	verts[0].Set(0, -1.5f);
	verts[1].Set(-.25f, 0);
	verts[2].Set(.25f, 0);

	verts[0] *= scale;
	verts[1] *= scale;
	verts[2] *= scale;

	pole_ = verts[0];

	s.Set(verts, 3);
}

void Shape::altSetTriangleEq(b2PolygonShape & s, float scale)
{
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

void Shape::takeDamage(int damage)
{
	hp_ -= damage;
}

void Shape::collect(int value)
{
	sides_ += value;
}

bool Shape::getControlled() const
{
	return controlled_;
}

bool Shape::getAI() const
{
	return ai_;
}

void Shape::setControlled(bool con)
{
	controlled_ = con;
}

void Shape::setAI(bool ai)
{
	ai_ = ai;

	//If we're an ai, dumb us down a bit //REMOVE
	if (ai)
	{
		maxVel_ /= 2;
		refireTime_ = 1000U;
	}
}

int Shape::getHP() const
{
	return hp_;
}

unsigned int Shape::getMaxHP() const
{
	return maxHP_;
}

b2Vec2 Shape::getFirePoint(float x, float y)
{
	b2Vec2 p = body_->GetPosition();
	b2Vec2 d(x, y);
	d.Normalize();

	d *= 1; //MAGIC NUMBER FIX SOON

	coolDown_ = refireTime_;

	return p + d;
}

bool Shape::getArmed()
{
	return (coolDown_ == 0);
}

void Shape::update(int milliseconds)
{
	if (active_)
	{
		//Weapon cooldown
		if (coolDown_ > 0)
		{
			coolDown_ -= milliseconds;

			if (coolDown_ < 0)
			{
				coolDown_ = 0;
			}
		}

		//Death check
		if (alive_)
		{
			if (hp_ <= 0)
			{
				alive_ = false;
				active_ = false;
			}
		}
	}
}

//Only deals with the effects of this collision on this entity
bool Shape::collide(Entity * other, b2Contact& contact)
{
	bool handled = false;

	if (Shape* shape = dynamic_cast<Shape*>(other))
	{
		handled = true;
	}

	else if (Projectile* proj = dynamic_cast<Projectile*>(other))
	{
		if (proj->getOwner() != this)
		{
			takeDamage(proj->getDamage());
		}

		else contact.SetEnabled(false);

		handled = true;
	}

	else if (Side* side = dynamic_cast<Side*>(other))
	{
		char* tagA = static_cast<char*>(contact.GetFixtureA()->GetUserData());
		char* tagB = static_cast<char*>(contact.GetFixtureB()->GetUserData());

		if (tagA == "side" || tagB == "side")
		{
			collect(side->getValue());
		}

		handled = true;
	}

	else if (Bounds* bounds = dynamic_cast<Bounds*>(other))
	{
		handled = true;
	}

	return handled;
}

//
//void Shape::draw(GameDrawer d)
//{
//
//}


