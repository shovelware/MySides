#include "Shape.hpp"

#include "Bounds.hpp"
#include "Projectile.hpp"
#include "Side.hpp"

///
#include <sstream>
#include "Log.hpp"
extern Log l;
///

//Creates a shape using passed body //int sides, float radius
Shape::Shape(b2Body* body, int vertices, float radius) : Entity(body), weapon_(nullptr)
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

	vertices_ = 0;
	shapeVertices_ = 0;

	hpMAX_ = 4;
	hp_ = hpMAX_;
	sides_ = 0;
	controlled_ = false;
	ai_ = false;
}

Shape::Shape(b2Body* body, ShapeDef &def) : Entity(body), weapon_(nullptr)
{
	switch (def.vertices)
	{
	case 3:
		setAsTriangle(def.size);
		break;
	case 4:
		setAsSquare(def.size);
		break;
	case 5:
		setAsPentagon(def.size);
		break;
	default:
		setAsTriangle(def.size * 2);
		break;
	}
	
	size_ = def.size;
	shapeVertices_ = def.vertices;
	vertices_ = shapeVertices_;

	//Color data
	colPrim_ = def.colPrim;
	colSecn_ = def.colSecn;
	colTert_ = def.colTert;

	maxVel_ = 0.025f * def.size; // max velocity in m/s // * size provisionally til we make an actual calculation
	maxRot_ = 0.0001f;

	hpMAX_ = def.hpMAX;
	hp_ = hpMAX_;
	sides_ = 0;
	controlled_ = false;
	ai_ = false;
}

//Destroys weapon if we have one
Shape::~Shape()
{
	if (weapon_ != nullptr)
	{
		delete weapon_;
	}
}

//Sets shape to be an equilateral triangle with sides of 1*scale
void Shape::setTriangleEqu(b2PolygonShape& s, float scale)
{

	//Magic number:  0.577351f
	b2Vec2 verts[3];
	
	verts[0].Set(0, -1.f);
	verts[1].Set(-.5f, 0);
	verts[2].Set(.5f, 0);

	verts[0] *= scale;
	verts[1] *= scale;
	verts[2] *= scale;

	pole_ = verts[0];

	s.Set(verts, 3);

	//type_ = traits::type::TRI_EQU;


	std::ostringstream o;
	
	b2Vec2 side = verts[2] - verts[0];
	o << side.Length() << endl;
	l.out(l.message, 'P', o.str().c_str());
}

//Sets shape to a regular tetrahedron with side length 1 * scale
void Shape::setSquare(b2PolygonShape& s, float scale)
{
	b2Vec2 verts[4];

	//This is the distance from the centre of the square
	//To each point that results in side length 1
	//Simple pythagoras trig math
	float dist = sqrt(2) / 2;

	verts[0] = b2Vec2(0 * scale, -dist * scale);
	verts[1] = b2Vec2(-dist * scale, 0 * scale);
	verts[2] = b2Vec2(0 * scale, dist * scale);
	verts[3] = b2Vec2(dist * scale, 0 * scale);

	s.Set(verts, 4);


//	type_ = traits::type::SQU_EQU;

	//std::ostringstream o;
	////o << orientation.x << ", " << orientation.y;
	//
	//b2Vec2 side = verts[3] - verts[2];
	//o << side.Length() << endl;
	//l.out(l.message, 'P', o.str().c_str());

}

void Shape::setAsTriangle(float size)
{
	//Shape data
	b2PolygonShape shap;
	setPoly(shap, 3, size);

	//Fixture data
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shap;

	//Collision
	fixtureDef.userData = "shape";
	addMaterial(fixtureDef);

	//Bind fixture
	body_->CreateFixture(&fixtureDef);

	//End box2d setup
}

void Shape::setAsSquare(float size)
{
	//Shape data
	b2PolygonShape shap;
	setPoly(shap, 4, size);

	//Fixture data
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shap;

	//Collision
	fixtureDef.userData = "shape";
	addMaterial(fixtureDef);

	//Bind fixture
	body_->CreateFixture(&fixtureDef);

	//End box2d setup
}

void Shape::setAsPentagon(float size)
{
	//Shape data
	b2PolygonShape shap;
	setPoly(shap, 5, size);

	//Fixture data
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shap;

	//Collision
	fixtureDef.userData = "shape";
	addMaterial(fixtureDef);

	//Bind fixture
	body_->CreateFixture(&fixtureDef);

	//End box2d setup
}

//Add material data to passed fixture def
void Shape::addMaterial(b2FixtureDef &def)
{
	def.density = 1.0f;
	def.friction = 1.0f;
	def.restitution = 1.f;
}

//Should only be called by set[shape] methods
void Shape::setPoly(b2PolygonShape & s, int vertices, float radius)
{
	assert(2 < vertices && vertices < 9);
	if (2 < vertices && vertices < 9)
	{
		b2Vec2* pnts = new b2Vec2[vertices];

		for (int i = 0; i < vertices; ++i)
		{
			pnts[i].x = radius * (cos(DR * 360 / vertices * i));
			pnts[i].y = radius * (sin(DR * 360 / vertices * i));
		}

		s.Set(pnts, vertices);

		delete[] pnts;
	}
}

void Shape::clearb2()
{
	body_->DestroyFixture(body_->GetFixtureList());
}

//Move in passed direction at constant speed
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
	//o << orientation.x << ", " << orientation.y;
	o << fmod(body_->GetAngle() * RD, 180);
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

	//Go back a shape if we can
	if (hp_ <= 0 && vertices_ > 3)
	{
		vertices_ = (vertices_ - 1 >= 2 ? vertices_ - 1 : 2);

		if (vertices_ > 2)
		{
			hpMAX_ = vertices_;
			hp_ = hpMAX_;
		}
	}
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
	}
}

int Shape::getHP() const
{
	return hp_;
}

unsigned int Shape::getHPMax() const
{
	return hpMAX_;
}

int Shape::getSidesCollected() const
{
	return sides_;
}

bool Shape::getArmed()
{
	return (weapon_ != nullptr);
}

void Shape::arm(Weapon::WeaponI * weapon)
{
	weapon_ = weapon;
}

void Shape::disarm()
{
	if (weapon_ != nullptr)
	{
		delete weapon_;
	}

	weapon_ = nullptr;
}

void Shape::fire(b2Vec2 direction)
{
	if (weapon_ != nullptr)
	{
		weapon_->trigger(direction);
	}
}

void Shape::update(int milliseconds)
{
	if (active_)
	{
		if (weapon_ != nullptr)
			weapon_->update(milliseconds);

		//Death check
		if (alive_)
		{
			if (vertices_ > 2 && vertices_ != shapeVertices_)
			{
				switch (vertices_)
				{
				case 3:
					clearb2();
					setAsTriangle(size_);
					break;
				case 4:
					clearb2();
					setAsSquare(size_);
					break;
				case 5:
					clearb2();
					setAsPentagon(size_);
					break;
				}
			}


			if (hp_ <= 0 && vertices_ <= 0)
			{
				//if (type_ == traits::type::SQU_EQU);//If we're a square drop a side and change to tri

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