#include "Shape.hpp"

Shape::Shape(b2Body* body, const ShapeDef &def, std::function<void(SideDef&)>& callback) : 
	Entity(body),
	weapon_(nullptr),
	lastDamage_(b2Vec2_zero)
{	
	//Collision
	shapeFixDef_.userData = "shape";
	addMaterial(shapeFixDef_);

	//Local storing of def items
	size_ = def.size;
	shapeVertices_ = def.vertices;
	
	//Corrections
	shapeVertices_ = fmax(3, fmin(shapeVertices_, 8));
	vertices_ = shapeVertices_;

	setPoly(vertices_, size_);

	snapOrient(def.heading); 

	//Color data
	colPrim_ = def.colPrim;
	colSecn_ = def.colSecn;
	colTert_ = def.colTert;

	sideCallback_ = callback;

	maxVel_ = 0.025f * (def.speedScale >= 0.5f ? def.speedScale : 0.5f); // max velocity in m/s // * size provisionally til we make an actual calculation
	maxRot_ = 0.0001f;

	hpScale_ = def.hpScale;
	hpMAX_ = def.hpMAX * hpScale_;
	hp_ = hpMAX_;
	sides_ = 0;
}

//Destroys weapon if we have one
Shape::~Shape()
{
	if (weapon_ != nullptr)
	{
		weapon_->setOwner(nullptr);
	}
}

void Shape::testBed()
{
	setPrimary(b2Color(randFloat(0.f, 1.f), randFloat(0.f, 1.f), randFloat(0.f, 1.f)));
	setSecondary(b2Color(randFloat(0.f, 1.f), randFloat(0.f, 1.f), randFloat(0.f, 1.f)));
	setTertiary(b2Color(randFloat(0.f, 1.f), randFloat(0.f, 1.f), randFloat(0.f, 1.f)));

	if (weapon_ != nullptr)
	{
		weapon_->setPrimary(colPrim_);
		weapon_->setSecondary(colSecn_);
		weapon_->setTertiary(colTert_);
	}
}

//Add material data to passed fixture def
void Shape::addMaterial(b2FixtureDef &def)
{
	def.density = 1.0f;
	def.friction = 0.8f;
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
			pnts[i].y = radius * (cos(DR * 360 / vertices * i)); //MAKE THESE START WITH DOWN
			pnts[i].x = radius * (-sin(DR * 360 / vertices * i)); //MAKE THESE START WITH DOWN
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
	b2Vec2 offset = dir;
	offset.Normalize();
	offset *= getSize();
	SideDef newSide = SideDef(getPosition() + offset, dir, size);
	
	newSide.colPrim = colSecn_;
	newSide.colSecn = colPrim_;
	newSide.colTert = colTert_;

	float rhs = (size_ * size_) + (size_ * size_) - (2 * size_ * size_) * cos(2 * M_PI / shapeVertices_);

	newSide.length = std::sqrt(rhs);

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

	//If we're going at a reasonable speed
	if (vel.Length() < maxVel_)
	{//Set desired velocity (respecting max velocity)
		b2Vec2 desiredVel = direction;
		desiredVel *= maxVel_;

		//Get the difference in current and desired
		b2Vec2 velChange = desiredVel - vel;

		//Apply appropriate impulse
		b2Vec2 impulse = body_->GetMass() * velChange;
		body_->ApplyLinearImpulse(impulse, body_->GetWorldCenter(), true);
	}

	//If we're getting knocked about, brake
	else
	{
		b2Vec2 reverse = body_->GetLinearVelocity();
		reverse.Normalize();
		reverse *= -maxVel_ * 0.5f;

		body_->ApplyLinearImpulse(reverse, body_->GetWorldCenter(), true);
	}

	//l.out(l.message, 'P', "Player move");
}

void Shape::orientedMove(b2Vec2 direction)
{
	orient(direction);
	move(direction);
}

void Shape::stopMove()
{
	body_->SetLinearVelocity(b2Vec2_zero);
}

void Shape::orient(b2Vec2 direction)
{
	float bodyAngle = body_->GetAngle();
	float desiredAngle = atan2f(-direction.x, direction.y);
	float nextAngle = body_->GetAngle() + body_->GetAngularVelocity() / _TICKTIME_;

	float totalRotation = desiredAngle - bodyAngle;

	while (totalRotation < -180 * DR) totalRotation += 360 * DR;
	while (totalRotation >  180 * DR) totalRotation -= 360 * DR;

	float change = 20 * DR; //allow 20 degree rotation per time step
	float newAngle = bodyAngle + std::min(change, std::max(-change, totalRotation));
	body_->SetTransform(body_->GetPosition(), newAngle);
}

void Shape::snapOrient(b2Vec2 direction)
{
	float bodyAngle = body_->GetAngle();
	float desiredAngle = atan2f(-direction.x, direction.y);

	body_->SetTransform(body_->GetPosition(), desiredAngle);
	body_->SetAngularVelocity(0);
}

void Shape::stopRotate()
{
	body_->SetAngularVelocity(0);
}

b2Vec2 Shape::getOrientation() const
{
	float r = body_->GetAngle();
	return b2Vec2(-sin(r), cos(r));
}

void Shape::takeDamage(int damage, b2Vec2 direction)
{
	lastDamage_ = direction;
	lastDamage_.Normalize();

	for (int dmg = damage; dmg > 0; --dmg)
	{
		//If we're about to go down, go back a side
		if (hp_ == 1)
		{
			hp_ -= 1;

			vertices_ = (vertices_ - 1 >= 2 ? vertices_ - 1 : 2);

			if (vertices_ > 2)
			{
				hpMAX_ = vertices_ * hpScale_;
				hp_ = hpMAX_;
			}
		}

		//Otherwise just take some hp
		else
		{
			if (vertices_ > 2)
			{
				hp_ -= 1;
			}
		}
		
		//We're dead, stop hurting
		if (vertices_ < 3 && hp_ <= 0)
			break;
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
	b2PolygonShape* shape = static_cast<b2PolygonShape*>(body_->GetFixtureList()->GetShape());

	for (int i = 0, count = shapeVertices_; i < count; ++i)
	{

		int a = i;
		int b = (a + 1 != count ? a + 1 : 0);

		b2Vec2 vA = shape->GetVertex(a);
		b2Vec2 vB = shape->GetVertex(b);
		
		b2Vec2 mid = vA + vB;
		mid.x /= 2.f;
		mid.y /= 2.f;

		mid.Normalize();
		dropSide(mid, 1);
	}

	alive_ = false;
}

bool Shape::getArmed()
{
	return (weapon_ != nullptr);
}

bool Shape::getWeaponReady()
{
	bool ready = false;
	
	if (weapon_ != nullptr)
	{
		ready = weapon_->canFire();
	}

	return ready;
}

void Shape::arm(Weapon::WeaponI * weapon)
{
	weapon_ = weapon;
	weapon_->setPrimary(colTert_);
	weapon_->setSecondary(colSecn_);
	weapon_->setTertiary(colSecn_);
	weapon_->setOwner(this);
}

Weapon::WeaponI * Shape::getWeapon() const
{
	return weapon_;
}

void Shape::disarm()
{
	if (weapon_ != nullptr)
	{
		weapon_->setOwner(nullptr);
	}

	weapon_ = nullptr;
}

void Shape::trigger(b2Vec2& direction)
{
	b2Vec2 dir = direction;

	if (dir.Length() < 0.1f)
	{
		dir = getOrientation();
	}

	orient(dir);
	if (weapon_ != nullptr)
	{
		weapon_->trigger(dir);
	}
}

void Shape::release()
{
	if (weapon_ != nullptr)
	{
		weapon_->release();
	}
}

void Shape::reup()
{
	if (weapon_ != nullptr)
	{
		weapon_->reup();
	}
}

int Shape::getWeaponBar()
{
	int bar = -1;

	if (weapon_ != nullptr)
	{
		bar = weapon_->getBar();
	}

	return bar;
}

int Shape::getWeaponBarMAX()
{
	int max = -1;

	if (weapon_ != nullptr)
	{
		max = weapon_->getBarMAX();
	}

	return max;
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

			//If we're at 0 health and can't go back, die
			if (hp_ <= 0 && vertices_ <= 3)
			{
				explode();

				alive_ = false;
			}

			//If our internal shape mismatches our displayed shape
			else if (vertices_ > 2 && vertices_ != shapeVertices_)
			{
				int diff = shapeVertices_ - vertices_;

				//This means we've moved down
				if (diff > 0)
				{
					//Drop the sides we lost
					for (int i = diff; i > 0; --i)
					{
						dropSide(lastDamage_, 1);
					}
				}

				//Correct mismatch
				setPoly(vertices_, size_);

				hpMAX_ = vertices_ * hpScale_;
				hp_ = hpMAX_;
			}

			//If we have a bunch of sides and could go up
			if (sides_ >= 16 && vertices_ < 8)
			{
				sides_ -= 16;
				vertices_ += 1;
			}

		}//End alive

	else active_ = false;
	//Do death stuff in this else, then set active to false

	}//end active
}

