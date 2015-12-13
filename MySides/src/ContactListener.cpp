#include "ContactListener.hpp"

ContactListener::ContactListener() : b2ContactListener()
{
}

void ContactListener::BeginContact(b2Contact * contact)
{
	////Should this be in presolve or begincontact
	////Get types from fixtures
	//char* tagA = static_cast<char*>(contact->GetFixtureA()->GetUserData());
	//char* tagB = static_cast<char*>(contact->GetFixtureB()->GetUserData());
	//
	//bool solved = false;
	//bool collide = contact->IsEnabled();
	//
	////If we got a pointer back for both
	//if (tagA && tagB)
	//{
	//	//Pull the fixtures
	//	b2Fixture* fixtureA = contact->GetFixtureA();
	//	b2Fixture* fixtureB = contact->GetFixtureB();
	//
	//	//Handle the contact appropriately
	//	solved = handleContact(tagA, tagB, fixtureA, fixtureB);
	//
	//	if (!solved)
	//	{
	//		//If we haven't solved it, flip the tags
	//		solved = handleContact(tagB, tagA, fixtureB, fixtureA);
	//	}
	//
	//	if (!solved)
	//	{
	//		std::cout << "UNSOLVED COLLISION: " << tagA << " " << tagB << std::endl;
	//	}
	//}

	bool handle = true;

	//Get types from fixtures
	char* tagA = static_cast<char*>(contact->GetFixtureA()->GetUserData());
	char* tagB = static_cast<char*>(contact->GetFixtureB()->GetUserData());

	if (tagA == "friction" || tagB == "friction")
	{
		handle = false;
	}


	if (handle)
	{
		Entity* entA = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
		Entity* entB = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

		//Contact enable bools
		bool collA = true, collB = true;
		//Return values
		bool handledA = false, handledB = false;

		//Each entity handles it's own collision
		handledA = entA->collide(entB, collA);
		handledB = entB->collide(entA, collB);

		//Entities must agree to collide physically
		contact->SetEnabled(collA && collB);

		if (!(handledA && handledB))
		{
			std::cout << 
				"UNSOLVED COLLISION: " << 
				(handledA ? "1 " : "0 ") << tagA << " " <<
				(handledB ? "1 " : "0 ") << tagB << std::endl;
		}
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

bool ContactListener::handleContact(char* tagA, char* tagB, b2Fixture* fixtureA, b2Fixture* fixtureB)
{
	//This branching if is messy and I hate it, how do I 
	//approach this in a nicer way? Ross, Oisin?
	//Also can't disable contacts from this method

	bool solved = false;

	//Friction <-> X
	//Proj <-> Proj, Shape, Bounds
	//Shape <-> Shape, Bounds

	//No collision with friction
	if (tagA == "friction" || tagA == "sidebox")
	{
		solved = true;
	}

	if (tagA == "side")
	{
		if (tagB == "shape")
		{
			void* s = fixtureA->GetBody()->GetUserData();
			Side* side = static_cast<Side*>(s);
			side->collect();
			solved = true;
		}

		if (tagB == "projectile")
		{
			solved = true;
		}

		if (tagB == "side")
		{
			solved = true;
		}
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
	proj->impact();
	otherProj->impact();
}

void ContactListener::handleCollision(Projectile* proj, Shape* shape)
{
	proj->impact();
	shape->takeDamage(proj->getDamage());
}

void ContactListener::handleCollision(Projectile* proj, Bounds* bounds)
{
	proj->impact();
}

void ContactListener::handleCollision(Shape* shape, Shape* otherShape)
{

}

void ContactListener::handleCollision(Shape* shape, Bounds* bounds)
{
}

