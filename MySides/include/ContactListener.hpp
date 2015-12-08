#ifndef MS_CONTACTLISTENER_HPP
#define MS_CONTACTLISTENER_HPP

#include "stdafx.h"
#include <Box2D\Box2D.h>

#include "Entity.hpp"
#include "Shape.hpp"
#include "Projectile.hpp"
#include "Bounds.hpp"

class ContactListener : public b2ContactListener {
private:
public:
	ContactListener();
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PreSolve(b2Contact *contact, const b2Manifold *oldManifold);
	void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);

	bool handleContact(char* tagA, char* tagB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	void handleCollision(Projectile* proj, Projectile* otherProj);
	void handleCollision(Projectile* proj, Shape* shape);
	void handleCollision(Projectile* proj, Bounds* bounds);
	void handleCollision(Shape* shape, Shape* otherShape);
	void handleCollision(Shape* shape, Bounds* bounds);
};
#endif