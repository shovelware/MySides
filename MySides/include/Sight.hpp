#ifndef MS_PICKSIGHT_HPP
#define MS_PICKSIGHT_HPP

#include "Pickup.hpp"

namespace Pickup {
	class Sight : public PickupI {
	public:
		Sight(b2Body* body, int time, float length = 0.f);

		void update(int milliseconds);

		bool getContact() const;

		bool collide(Entity* other, b2Contact& contact, std::string tag);
	private:
		void onCollect();

		bool colourChange_;

		float length_;
		bool contact_;
		std::string enemyTag_;
	};
}

#endif