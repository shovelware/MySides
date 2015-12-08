#include "ContactListener.hpp"

ContactListener::ContactListener() : b2ContactListener()
{
}

void ContactListener::BeginContact(b2Contact * contact)
{
}

void ContactListener::EndContact(b2Contact * contact)
{
}

void ContactListener::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
	//Should this be in presolve or begincontact
	//Get types from fixtures
	char* tagA = static_cast<char*>(contact->GetFixtureA()->GetUserData());
	char* tagB = static_cast<char*>(contact->GetFixtureB()->GetUserData());

	bool solved = false;

	//If we got a pointer back for both
	if (tagA && tagB)
	{
		//Pull the fixtures
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		//Handle the contact appropriately
		solved = handleContact(tagA, tagB, fixtureA, fixtureB);

		if (!solved)
		{
			//If we haven't solved it, flip the tags
			solved = handleContact(tagB, tagA, fixtureB, fixtureA);
		}

		if (!solved)
		{
			std::cout << "UNSOLVED COLLISION" << std::endl;
		}
	}
}

void ContactListener::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{
}

bool ContactListener::handleContact(char* tagA, char* tagB, b2Fixture* fixtureA, b2Fixture* fixtureB)
{
	//This branching if is messy and I hate it, how do I 
	//approach this in a nicer way? Ross, Oisin?
	bool solved = false;

	//Friction <-> X
	//Proj <-> Proj, Shape, Bounds
	//Shape <-> Shape, Bounds

	//No collision with friction
	if (tagA == "friction")
	{
		solved = true;
	}

	//Projectiles with everything
	if (tagA == "projectile")
	{
		void* p = fixtureA->GetBody()->GetUserData();
		Projectile* proj = static_cast<Projectile*>(p);

		if (tagB == "projectile")
		{
			void* op = fixtureB->GetBody()->GetUserData();
			Projectile* otherProj = static_cast<Projectile*>(op);

			handleCollision(proj, otherProj);
			solved = true;
		}

		if (tagB == "shape")
		{
			void* s = fixtureB->GetBody()->GetUserData();
			Shape* shape = static_cast<Shape*>(s);

			handleCollision(proj, shape);
			solved = true;
		}

		else if (tagB == "bounds")
		{
			void* b = fixtureB->GetBody()->GetUserData();
			Bounds* bounds = static_cast<Bounds*>(b);

			handleCollision(proj, bounds);
			solved = true;
		}
	}

	//Shapes with everything
	else if (tagA == "shape")
	{
		void* s = fixtureA->GetBody()->GetUserData();
		Shape* shape = static_cast<Shape*>(s);

		if (tagB == "shape")
		{
			void* os = fixtureB->GetBody()->GetUserData();
			Shape* otherShape = static_cast<Shape*>(os);

			handleCollision(shape, otherShape);
			solved = true;
		}

		if (tagB == "bounds")
		{
			void* b = fixtureB->GetBody()->GetUserData();
			Bounds* bounds = static_cast<Bounds*>(b);

			handleCollision(shape, bounds);
			solved = true;
		}
	}

	return solved;
}

void ContactListener::handleCollision(Projectile* proj, Projectile* otherProj)
{
	proj->hit();
	otherProj->hit();
}

void ContactListener::handleCollision(Projectile* proj, Shape* shape)
{
	proj->hit();
	shape->setActive(false);
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

