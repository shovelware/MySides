#ifndef MS_PICKATTRACT_HPP
#define MS_PICKATTRACT_HPP

#include "Pickup.hpp"
#include "PickupDef.hpp"
#include "Side.hpp"

#include <queue>

namespace Pickup {
	class Attractor : public PickupI {
	public:
		Attractor(b2Body* body, const PickupDef& def);

		void update(int milliseconds);

		float getRadius();

		bool collide(Entity* other, b2Contact& contact);
	private:
		void onCollect();

		float radius_;
		float strength_;

		std::queue<Side*> sides_;

	};
}

#endif