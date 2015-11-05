#include "Entity.hpp"

Entity::Entity(b2Body * body) : body_(body)
{
	body->SetUserData(this);
}

void Entity::setPosition(b2Vec2 position)
{
	body_->SetTransform(position, body_->GetTransform().q.GetAngle());
}

void Entity::setRotation(float rotation)
{
	body_->SetTransform(body_->GetTransform().p, rotation);
}