#include "Shape.hpp"
#include "Projectile.hpp"
#include "Side.hpp"

Shape::Shape(b2Body* body, const ShapeDef &def, std::function<void(SideDef&)>& callback) : 
	Entity(body),
	weapon_(nullptr),
	lastDamage_(b2Vec2_zero),
	size_(def.size),
	drop_(def.drop),
	shapeVertices_(def.vertices),
	verticesMIN_(def.verticesMin - 1),
	verticesMAX_(def.verticesMax),
	sideCallback_(callback),
	hpScale_(def.hpScale),
	damageScale_(def.damageScale),
	collector_(false),
	sides_(0)
{	
	//Collision
	shapeFixDef_.userData = "shape";
	shapeFixDef_.density = 100.0f;
	shapeFixDef_.friction = 0.8f;
	shapeFixDef_.restitution = 0.3f;
	faction_ = def.faction;

	//Spawning
	alive_ = false;
	spawnTimeMAX_ = fmin(2500, fmax(def.size * 500, 500));
	spawnTime_ = spawnTimeMAX_;

	//Corrections
	shapeVertices_ = fmax(3, fmin(shapeVertices_, 8));
	vertices_ = shapeVertices_;

	setPoly(vertices_, size_);

	snapOrient(def.heading); 
	
	//Color data
	colPrim_ = def.colPrim;
	colSecn_ = def.colSecn;
	colTert_ = def.colTert;

	maxVel_ = 0.025f * (def.speedScale >= 0.5f ? def.speedScale : 0.5f); // max velocity in m/s // * size provisionally til we make an actual calculation
	maxRot_ = 0.001f;

	hpMAX_ = vertices_ * hpScale_;
	hp_ = hpMAX_;
	uhpScale_ = (def.upgrade ? hpScale_ : 0);
	uhpMAX_ = ((vertices_ + 1) * uhpScale_);
	uhp_ = 0;
}

//Destroys weapon if we have one
Shape::~Shape()
{
	if (weapon_ != nullptr)
	{
		weapon_->setOwner(nullptr);
	}
}

//Should only be called by set[shape] methods
void Shape::setPoly(int vertices, float radius)
{
	//Sensor
	shapeFixDef_.isSensor = !alive_;

	//Shape data
	b2PolygonShape shap;

	if (radius > 0 && verticesMIN_ < vertices && vertices <= verticesMAX_)
	{
		b2Vec2* pnts = new b2Vec2[vertices];

		for (int i = 0; i < vertices; ++i)
		{
			pnts[i].y = radius * (cos(M_PI * 2 / vertices * i));
			pnts[i].x = radius * (-sin(M_PI * 2 / vertices * i));
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
void Shape::dropSide(b2Vec2 dir)
{
	if (!drop_) return;

	b2Vec2 offset = dir;
	offset.Normalize();
	offset *= getSize();

	float rhs = std::sqrt((size_ * size_) + (size_ * size_) - (2 * size_ * size_) * cos(2 * M_PI / shapeVertices_));

	SideDef newSide = SideDef(getPosition() + offset, dir, rhs, fmin(1000 + (1000.f * rhs), 10000));

	newSide.colPrim = colSecn_;
	newSide.colSecn = colPrim_;
	newSide.colTert = colTert_;
	newSide.faction = faction_;

	sideCallback_(newSide);
}

//Move in passed direction at constant speed
void Shape::move(b2Vec2 direction)
{
	//Don't move if we're dead
	if (!alive_) return;

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

void Shape::stopMove()
{
	body_->SetLinearVelocity(b2Vec2_zero);
}

void Shape::orient(b2Vec2 direction)
{
	//Don't turn if we're dead
	if (!alive_) return;

	float bodyAngle = body_->GetAngle();
	float desiredAngle = atan2f(-direction.x, direction.y);
	float nextAngle = body_->GetAngle() + body_->GetAngularVelocity() / _TICKTIME_;

	float totalRotation = desiredAngle - bodyAngle;

	while (totalRotation < M_PI) totalRotation += 2 * M_PI;
	while (totalRotation > M_PI) totalRotation -= 2 * M_PI;

	float change = 0.2f  * size_; //allow 20 degree rotation per time step
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

int Shape::getSpawnTime() const { return spawnTime_; }

int Shape::getSpawnTimeMax() const { return spawnTimeMAX_; }

void Shape::heal(int health)
{
	if (!alive_) return;

	for (int hlt = health, max = getHPMax(); hlt > 0 && (hp_ + uhp_ < max); --hlt) 
	{
		//Overhealing
		if (uhpMAX_ != 0 && hp_ == hpMAX_)
		{
			//If we can go up
			if (uhp_ + 1 <= uhpMAX_) uhp_++;

			syncHP();

			//At the top, break
			if (uhp_ == uhpMAX_)
			{
				break;
			}
		}

		//Just healing
		else
		{
			if (hp_ + 1 <= hpMAX_) hp_++;

			//If we can't add overheal
			else if (uhpMAX_ == 0) break;
		}
	}
}

void Shape::takeDamage(int damage, b2Vec2 direction)
{
	if (!alive_) return;
	lastDamage_ = direction;
	lastDamage_.Normalize();

	for (int dmg = damage; dmg > 0; --dmg)
	{
		//If we're overhealed
		if (uhp_ > 0)
		{
			uhp_ -= 1;
		}

		//If we're about to go down, go back a side
		else if (hp_ == 1)
		{
			hp_ -= 1;

			vertices_ = (vertices_ - 1 >= verticesMIN_ ? vertices_ - 1 : verticesMIN_);

			if (vertices_ > verticesMIN_)
			{
				//Reset health
				hpMAX_ = vertices_ * hpScale_;
				hp_ = hpMAX_;

				//Re-enable upgrades if we must
				uhpMAX_ = (vertices_ + 1) * uhpScale_;
			}
		}

		//Otherwise just take some hp
		else
		{
			if (vertices_ > verticesMIN_)
				hp_ -= 1;
		}

		//We're dead, stop hurting
		if (vertices_ <= verticesMIN_ && hp_ <= 0 && uhp_ <= 0)
		{
			syncHP();
			break;
		}
	}

	//HP should be refilled in loop, this means we're actually dead
	if (hp_ <= 0)
	{
		alive_ = false;
	}
}

void Shape::collect(int value)
{
	sides_ += value;
}

bool Shape::syncHP()
{
	if (uhpMAX_ != 0 && uhp_ >= uhpMAX_ && vertices_ < verticesMAX_)
	{
		vertices_++;
		hp_ = hpMAX_;
		hpMAX_ = vertices_ * hpScale_;
		uhp_ = 0;
		uhpMAX_ = (vertices_ + 1) * uhpScale_;
		return true;
	}

	else return false;
}

void Shape::syncPoly()
{
	//If our internal shape mismatches our displayed shape
	if (vertices_ > verticesMIN_ && vertices_ != shapeVertices_)
	{
		int diff = shapeVertices_ - vertices_;

		//This means we've moved down
		if (diff > 0)
		{
			//Drop the sides we lost
			for (int i = diff; i > 0; --i)
			{
				dropSide(lastDamage_);
			}
		}

		//Correct mismatch
		setPoly(vertices_, size_);
	}
}

void Shape::setPrimary(b2Color col)
{
	colPrim_ = col;
	if (weapon_ != nullptr)
	{
		weapon_->setPrimary(col);
	}
}

void Shape::setSecondary(b2Color col)
{
	colSecn_ = col;
	if (weapon_ != nullptr)
	{
		weapon_->setSecondary(col);
	}
}

void Shape::setTertiary(b2Color col)
{
	colTert_ = col;
	if (weapon_ != nullptr)
	{
		weapon_->setTertiary(col);
	}
}

int Shape::getHP() const 
{
	int hp = 0;

	if (spawnTime_ > 0 || alive_)
	{
		for (int vrts = vertices_ - 1; vrts > verticesMIN_; --vrts)
		{
			hp += vrts * hpScale_;
		}

		hp += hp_;
	}

	return hp; 
}

unsigned int Shape::getHPMax() const 
{
	int mhp = 0;
	int vertmax = vertices_;
	if (uhpScale_ != 0) vertmax = verticesMAX_;

	for (int vrts = vertmax; vrts > verticesMIN_; --vrts)
	{
		mhp += vrts * hpScale_;
	}

	return mhp;
}

int Shape::getUHP() const { return uhp_; }

unsigned int Shape::getUHPMax() const { return uhpMAX_; }

int Shape::getSidesCollected() const { return sides_; }
float Shape::getSize() const { return size_; }

float Shape::getDamageScale() const { return damageScale_; }

bool Shape::wasDamaged()
{
	return lastHealth_ > getHP();
}

void Shape::explode()
{
	if (vertices_ > 0)
	{
		b2PolygonShape* shape = static_cast<b2PolygonShape*>(body_->GetFixtureList()->GetShape());

		for (int i = 0, count = shapeVertices_; i < count; ++i)
		{
			int a = i;
			int b = (a + 1 != count ? a + 1 : 0);

			b2Vec2 vA = body_->GetWorldPoint(shape->GetVertex(a));
			b2Vec2 vB = body_->GetWorldPoint(shape->GetVertex(b));
			b2Vec2 mid = vA - vB;

			b2Vec2 norm = b2Vec2(-mid.y, mid.x);
			norm.x /= 2.f;
			norm.y /= 2.f;

			norm.Normalize();
			dropSide(norm);
		}

		body_->GetFixtureList()->SetSensor(true);
		kill();
		spawnTime_ = 0;
		vertices_ = 0;
	}
}

bool Shape::getArmed()
{
	return (weapon_ != nullptr);
}

bool Shape::getWeaponReady() const
{
	bool ready = false;
	
	if (weapon_ != nullptr)
	{
		ready = weapon_->canFire();
	}

	return ready;
}

bool Shape::getWeaponLoading() const
{
	bool ready = false;

	if (weapon_ != nullptr)
	{
		ready = weapon_->isUpping();
	}

	return ready;
}

int Shape::getWeaponLevel() const 
{
	int lvl = 0;

	if (weapon_ != nullptr)
	{
		lvl = weapon_->getLevel();
	}

	return lvl;
}

void Shape::arm(Weapon::WeaponI * weapon)
{
	weapon_ = weapon;
	if (weapon_ != nullptr)
	{
		weapon_->setPrimary(colTert_);
		weapon_->setSecondary(colSecn_);
		weapon_->setTertiary(colSecn_);
		weapon_->setOwner(this);
		weapon_->setFaction(faction_);
	}
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
	//Don't fire if we're dead
	if (!alive_) return;

	b2Vec2 dir = direction;

	//If there's no impulse, fire where we're looking
	if (dir.Length() < 0.1f)
	{
		dir = getOrientation();
		dir.Normalize();
		dir *= size_;
	}

	//Otherwise face towards new direction
	else orient(dir);

	//If we can fire, do so
	if (weapon_ != nullptr)
	{
		dir.Normalize();
		dir *= size_;
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
	//Don't reaload if we're dead
	if (!alive_) return;

	if (weapon_ != nullptr)
	{
		weapon_->reup();
	}
}

int Shape::getWeaponBar()
{
	int bar = 0;

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

bool Shape::collide(Entity* other, b2Contact& contact, std::string tag)
{
	bool handled = false;

	if (active_ == false) return true;


	if (tag == "projectile")
	{
		Projectile* proj = static_cast<Projectile*>(other);

		if (proj->getOwner() != this)
		{
			takeDamage(proj->getDamage(), proj->getDirection());
		}

		else contact.SetEnabled(false);

		handled = true;
	}

	else if (tag == "side" && collector_)
	{
		Side* side = static_cast<Side*>(other);

		collect(side->getValue());
		heal(std::ceil(side->getValue()) * fmaxf(1.f, (hpScale_ / 2)));
			
		side->collect();

		handled = true;
	}

	return handled;
}

void Shape::update(int milliseconds)
{
	if (active_)
	{
		//Death check
		if (alive_)
		{
			//Slow down to maxVel
			b2Vec2 vel = body_->GetLinearVelocity();
			if (vel.Length() > maxVel_)
			{
				vel *= 0.9f;
				body_->SetLinearVelocity(vel);
			}

			//Update weapon if we have one
			if (weapon_ != nullptr)
			{
				weapon_->update(milliseconds);
			}

			syncHP();
			syncPoly();
			lastHealth_ = getHP();
		}//End alive

		//Spawning
		else if (spawnTime_ > 0)
		{
			spawnTime_ -= milliseconds;

			if (spawnTime_ <= 0)
			{
				alive_ = true;
				body_->GetFixtureList()->SetSensor(false);
			}
		}


		//If dead
		else
		{
			explode();
			active_ = false;
		}
	}//end active
}

