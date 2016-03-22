#ifndef MS_PICKSHIELD_HPP
#define MS_PICKSHIELD_HPP

#include "Pickup.hpp"
#include "PickupDef.hpp"

namespace Pickup {
	class Shield : public PickupI {
	public:
		Shield(b2Body* body, const PickupDef& def);

		void update(int milliseconds);

		b2Vec2 getRadius();

		bool collide(Entity* other, b2Contact& contact);
	private:
		void onCollect();

		void takeDamage(int damage);

		float radius_;
		float hpMAX_;
		float hp_;
	};
}

#endif