#include "ContactListener.hpp"

ContactListener::ContactListener() : b2ContactListener()
{
}

void ContactListener::BeginContact(b2Contact * contact)
{
	char* fixA = static_cast<char*>(contact->GetFixtureA()->GetUserData());
	char* fixB = static_cast<char*>(contact->GetFixtureB()->GetUserData());
	
	//If we got a pointer back for both
	_ASSERT(contact->GetFixtureA() != contact->GetFixtureB());

	if (fixA && fixB)
	{
		if (fixA == "projectile")
		{
			void* p = contact->GetFixtureA()->GetBody()->GetUserData();
			Projectile* proj = static_cast<Projectile*>(p);
			proj->hit();
		}
		
		if (fixB == "projectile") //Always counts???
		{
			void* p = contact->GetFixtureB()->GetBody()->GetUserData();
			Projectile* proj = static_cast<Projectile*>(p);
			proj->hit();
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
	int thing = 5;
}

void ContactListener::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
	int thing = 5;
}

void ContactListener::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{
	int thing = 5;
}