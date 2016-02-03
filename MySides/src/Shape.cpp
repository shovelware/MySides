#include "Shape.hpp"

#include "Bounds.hpp"
#include "Projectile.hpp"
#include "Side.hpp"

///
#include <sstream>
#include "Log.hpp"
extern Log l;
///

Shape::Shape(b2Body* body, ShapeDef &def, std::function<void(SideDef&)>& callback) : Entity(body), weapon_(nullptr)
{	
	//Collision
	shapeFixDef_.userData = "shape";
	addMaterial(shapeFixDef_);

	setPoly(def.vertices, def.size);

	//Local storing of def items
	size_ = def.size;
	shapeVertices_ = def.vertices;
	vertices_ = shapeVertices_;
	//ROTATE TO FACE HEADING HERE

	//Color data
	colPrim_ = def.colPrim;
	colSecn_ = def.colSecn;
	colTert_ = def.colTert;

	sideCallback_ = callback;

	maxVel_ = 0.025f * (def.speedScale >= 0.5f ? def.speedScale : 0.5f); // max velocity in m/s // * size provisionally til we make an actual calculation
	maxRot_ = 0.0001f;

	hpMAX_ = def.hpMAX;
	hp_ = hpMAX_;
	sides_ = 0;
}

//Destroys weapon if we have one
Shape::~Shape()
{
	if (weapon_ != nullptr)
	{
		delete weapon_;
	}
}

//Add material data to passed fixture def
void Shape::addMaterial(b2FixtureDef &def)
{
	def.density = 1.0f;
	def.friction = 1.0f;
	def.restitution = 1.f;
}

//Should only be called by set[shape] methods
void Shape::setPoly(int vertices, float radius)
{
	//Shape data
	b2PolygonShape shap;

	assert(2 < vertices && vertices < 9);
	assert(radius > 0);

	if (2 < vertices && vertices < 9)
	{
		b2Vec2* pnts = new b2Vec2[vertices];

		for (int i = 0; i < vertices; ++i)
		{
			pnts[i].x = radius * (cos(DR * 360 / vertices * i)); //MAKE THESE START WITH DOWN
			pnts[i].y = radius * (sin(DR * 360 / vertices * i)); //MAKE THESE START WITH DOWN
		}

		shapeVertices_ = vertices;

		shap.Set(pnts, vertices);

		delete[] pnts;
	}

	//Fixture shape
	shapeFixDef_.shape = &shap;

	//If there's something there, clear it
	if (body_->GetFixtureList()) 
	{
		clearb2();
	}

	//Bind fixture
	body_->CreateFixture(&shapeFixDef_);

	//End box2d setup
}

//Clears the box2d fixture list of the first fixture (We should only have one anyway)
void Shape::clearb2()
{
	body_->DestroyFixture(body_->GetFixtureList());
}

//Uses a callback to drop a side
void Shape::dropSide(b2Vec2 dir, float size)
{
	b2Vec2 offset = b2Vec2(randFloat(-5, 5), randFloat(-5, 5));
	SideDef newSide = SideDef(getPosition() + offset, dir, size);
	
	newSide.colPrim = colSecn_;
	newSide.colSecn = colPrim_;
	newSide.colTert = colTert_;

	sideCallback_(newSide);
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
	float bodyAngle = body_->GetAngle();
	b2Vec2 toTarget = direction;
	float desiredAngle = atan2f(-toTarget.x, toTarget.y);
	body_->SetTransform(body_->GetPosition(), desiredAngle);
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

float Shape::getSize() const
{
	return size_;
}

void Shape::explode()
{
	for (int i = vertices_; i > 0; --i)
	{
		dropSide(b2Vec2(randFloat(-5, 5), randFloat(-5, 5)), 1);
	}

	alive_ = false;
}

bool Shape::getArmed()
{
	return (weapon_ != nullptr);
}

void Shape::arm(Weapon::WeaponI * weapon)
{
	weapon_ = weapon;
	weapon_->setPrimary(colPrim_);
	weapon_->setSecondary(colSecn_);
	weapon_->setTertiary(colTert_);
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
	orient(direction);
	if (weapon_ != nullptr)
	{
		weapon_->trigger(direction);
	}
}

void Shape::update(int milliseconds)
{
	if (active_)
	{
		//Death check
		if (alive_)
		{
			//Update weapon if we have on
			if (weapon_ != nullptr)
			{
				weapon_->update(milliseconds);
			}

			//If our internal shape mismatches our displayed shape
			if (vertices_ > 2 && vertices_ != shapeVertices_)
			{
				int diff = shapeVertices_ - vertices_;

				//This means we've moved down
				if (diff > 0)
				{
					//Drop the sides we lost
					for (int i = diff; i > 0; --i)
					{
						dropSide(b2Vec2(randFloat(-5, 5), randFloat(-5, 5)), 1);
					}
				}

				//Correct mismatch
				setPoly(vertices_, size_);

				hpMAX_ = vertices_;
				hp_ = hpMAX_;
			}

			//If we have a bunch of sides and could go up
			else if (sides_ >= 16 && vertices_ < 8)
			{
				sides_ -= 16;
				vertices_ += 1;
			}

			//If we're a  0 health triangle, die
			if (hp_ <= 0 && vertices_ <= 3)
			{
				explode();

				alive_ = false;
			}
		}//End alive

	else active_ = false;
	//Do death stuff in this else, then set active to false

	}//end active
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