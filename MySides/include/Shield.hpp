#ifndef MS_PICKSHIELD_HPP
#define MS_PICKSHIELD_HPP

#include "Pickup.hpp"

namespace Pickup {
	class Shield : public PickupI {
	public:
		Shield(b2Body* body, int time);

		void update(int milliseconds);

		float getStrength();

		bool collide(Entity* other, b2Contact& contact, std::string tag);
	private:
		void onCollect();

		float radius_;
		float strength_;
	};
}

#endif