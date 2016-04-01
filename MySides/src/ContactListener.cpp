#include "ContactListener.hpp"

ContactListener::ContactListener() : b2ContactListener()
{
}

void ContactListener::BeginContact(b2Contact * contact)
{
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

		//Return values
		bool handledA = false, handledB = false;

		//Each entity handles it's own collision
		handledA = entA->collide(entB, *contact, tagB);
		handledB = entB->collide(entA, *contact, tagA);

		//if (!(handledA && handledB))
		//{
		//	std::cout << 
		//		"UNSOLVED COLLISION: " << 
		//		(handledA ? "1 " : "0 ") << tagA << " " <<
		//		(handledB ? "1 " : "0 ") << tagB << std::endl;
		//}
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