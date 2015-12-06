#include "ContactListener.hpp"

ContactListener::ContactListener() : b2ContactListener()
{
}

void ContactListener::BeginContact(b2Contact * contact)
{
	//Get types from fixtures
	char* tagA = static_cast<char*>(contact->GetFixtureA()->GetUserData());
	char* tagB = static_cast<char*>(contact->GetFixtureB()->GetUserData());
	
	//If we got a pointer back for both
	if (tagA && tagB)
	{
		for (int i = 0; i < 2; ++i)
		{
			//Pull the fixtures
			b2Fixture* fixtureA = contact->GetFixtureA();
			b2Fixture* fixtureB = contact->GetFixtureB();

			//This branching if is messy and I hate it, how do I 
			//approach this in a nicer way? Ross, Oisin?
			if (tagA == "projectile")
			{
				void* p = fixtureA->GetBody()->GetUserData();
				Projectile* proj = static_cast<Projectile*>(p);

				proj->hit();
				
				if (tagB == "projectile")
				{
					void* op = fixtureB->GetBody()->GetUserData();
					Projectile* otherProj = static_cast<Projectile*>(op);

					handleCollision(proj, otherProj);
				}

				if (tagB == "shape")
				{
					void* s = fixtureB->GetBody()->GetUserData();
					Shape* shape = static_cast<Shape*>(s);

					handleCollision(proj, shape);
				}

				else if (tagB == "bounds")
				{
					void* b = fixtureB->GetBody()->GetUserData();
					Bounds* bounds = static_cast<Bounds*>(b);

					handleCollision(proj, bounds);
				}
			}

			else if (tagA == "shape")
			{
				void* s = fixtureA->GetBody()->GetUserData();
				Shape* shape = static_cast<Shape*>(s);

				if (tagB == "shape")
				{
					void* os = fixtureB->GetBody()->GetUserData();
					Shape* otherShape = static_cast<Shape*>(os);

					handleCollision(shape, otherShape);
				}

				if (tagB == "bounds")
				{
					void* b = fixtureB->GetBody()->GetUserData();
					Bounds* bounds = static_cast<Bounds*>(b);

					handleCollision(shape, bounds);
				}
			}

			//This should work, right?
			std::swap(tagA, tagB);
			std::swap(fixtureA, fixtureB);
		}
		//if (fixA == "projectile" && fixB == "shape" ||
		//	fixA == "shape" && fixB == "projectile")
		//{
		//
		//}
		
		//Entity* entityA = static_cast<Entity*>(udA);
		//Entity* entityB = static_cast<Entity*>(udB);
	}
}

void ContactListener::EndContact(b2Contact * contact)
{
}

void ContactListener::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
}

void ContactListener::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{
}

void ContactListener::handleCollision(Projectile* proj, Projectile* otherProj)
{
	proj->hit();
	otherProj->hit();
}

void ContactListener::handleCollision(Projectile* proj, Shape* shape)
{
	proj->hit();
	shape->kill();
}

void ContactListener::handleCollision(Projectile* proj, Bounds* bounds)
{
	proj->hit();
}

void ContactListener::handleCollision(Shape* shape, Shape* otherShape)
{

}

void ContactListener::handleCollision(Shape* shape, Bounds* bounds)
{
}

