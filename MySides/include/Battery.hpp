#ifndef MS_WBATTERY_HPP
#define MS_WBATTERY_HPP

class Battery {
private:
	int charge_;
	int chargeMAX_;

public:
	Battery(int capacity, bool charged = true) :
		chargeMAX_(capacity),
		charge_((charged ? capacity : 0))
	{}

	void recharge(int charge)
	{
		charge_ = charge_ + charge <= chargeMAX_ ? charge_ + charge : chargeMAX_;
	}
	
	bool discharge(int charge)
	{
		if (charge <= charge_)
		{
			charge_ -= charge;
			return true;
		}

		else
		{
			charge_ = 0;
			return false;
		}
	}

	void refill()
	{
		charge_ = chargeMAX_;
	}

	void empty() { charge_ = 0; }

	void resize(int size, bool reload)
	{
		if (size > 0)
		{
			chargeMAX_ = size;

			if (charge_ > size)
			{
				charge_ = size;
			}

			if (reload)
			{
				charge_ = chargeMAX_;
			}
		}
	}

	int getCharge() const { return charge_; }
	int getChargeMAX() const { return chargeMAX_; }
	float getPercent() const { return ((float)charge_ / (float)chargeMAX_) * 100.f; }
	bool checkFull() const { return charge_ == chargeMAX_; }
	bool checkEmpty() const { return charge_ == 0; }
};
#endif