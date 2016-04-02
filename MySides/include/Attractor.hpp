#ifndef MS_PICKATTRACT_HPP
#define MS_PICKATTRACT_HPP

#include "Pickup.hpp"
#include "Side.hpp"

namespace Pickup {
	class Attractor : public PickupI {
	public:
		Attractor(b2Body* body, int time, float radius = 0);

		void update(int milliseconds);

		float getRadius();

		bool collide(Entity* other, b2Contact& contact, std::string tag);
	private:
		void onCollect();

		float radius_;
	};
}

#endif