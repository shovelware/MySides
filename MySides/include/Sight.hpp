#ifndef MS_PICKSIGHT_HPP
#define MS_PICKSIGHT_HPP

#include "Pickup.hpp"

namespace Pickup {
	class Sight : public PickupI {
	public:
		Sight(b2Body* body, int time);

		void update(int milliseconds);

		b2Vec2 getEnd();

		bool collide(Entity* other, b2Contact& contact);
	private:
		void onCollect();

		float size_;
		b2Vec2 end_;
	};
}

#endif