#ifndef MS_PICKUP_HPP
#define MS_PICKUP_HPP

#include "stdafx.h"

#include "Shape.hpp"

//! @class PickupI
//! @brief Base Pickup class
//!

//!

namespace Pickup
{
	class PickupI : public Entity {
	public:
		virtual void update(int milliseconds);

		void setOwner(Shape* o);

		Shape* getOwner() const;
		bool getCollected() const;


	protected:
		PickupI(b2Body* body, int time);
		virtual void onCollect() = 0;

		Shape* owner_;

		bool collected_;

		int time_;
	};
}
	
#endif