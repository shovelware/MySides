#include "Entity.hpp"

//Construct an entity, which MUST have a body
Entity::Entity(b2Body * body) : body_(body)
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

b2Body * Entity::getBody()
{
	return body_;
}

////Gets points of body's first fixture, assumes polygon. Override if necessary
//b2Shape* Entity::getVertices()
//{
//	b2PolygonShape* poly = (b2PolygonShape*)(body_->GetFixtureList()->GetShape());
//	b2Shape* b = body_->GetFixtureList()->GetShape();
//	
//	return poly;
//}