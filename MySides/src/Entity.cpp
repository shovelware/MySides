#include "Entity.hpp"

//Construct an entity, which MUST have a body
//Entities are alive and active at construction
Entity::Entity(b2Body * body) : body_(body), alive_(true), active_(true)
{
	body->SetUserData(this);
}

//Sets the body's position to passed value
void Entity::setPosition(b2Vec2 position)
{
	body_->SetTransform(position, body_->GetTransform().q.GetAngle());
}

//Sets the body's rotation to passed value
void Entity::setRotation(float rotation)
{
	body_->SetTransform(body_->GetTransform().p, rotation);
}

//Returns position
b2Vec2 Entity::getPosition() const
{
	return body_->GetPosition();
}

//Returns rotation (unbounded)
float Entity::getRotation() const
{
	return body_->GetAngle();
}

//Returns a pointer to the Entity's body
b2Body * Entity::getBody()
{
	return body_;
}

//Returns alive
bool Entity::getAlive() const
{
	return alive_;
}

//Returns active
bool Entity::getActive() const
{
	return active_;
}

//Set alive status of the entity
void Entity::setAlive(bool a)
{
	alive_ = a;
}

//Set active status of the entity
void Entity::setActive(bool a)
{
	active_ = a;
}

//Straight-up pull the plug. Can be overridden
void Entity::kill()
{
	alive_ = false;
}

bool Entity::collide(Entity * other, bool & physicsCollision)
{
	return false;
}

////Gets points of body's first fixture, assumes polygon. Override if necessary
//b2Shape* Entity::getVertices()
//{
//	b2PolygonShape* poly = (b2PolygonShape*)(body_->GetFixtureList()->GetShape());
//	b2Shape* b = body_->GetFixtureList()->GetShape();
//	
//	return poly;
//}