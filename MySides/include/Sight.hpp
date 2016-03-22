#ifndef MS_PICKSIGHT_HPP
#define MS_PICKSIGHT_HPP

#include "Pickup.hpp"
#include "PickupDef.hpp"

namespace Pickup {
	class Sight : public PickupI {
	public:
		Sight(b2Body* body, const PickupDef& def);

		void update(int milliseconds);

		b2Vec2 getEnd();

		bool collide(Entity* other, b2Contact& contact);
	private:
		void onCollect();

		float size_;
		float strength_;
		b2Vec2 end_;
	};
}

#endif