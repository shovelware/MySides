#ifndef MS_WBATTERY_HPP
#define MS_WBATTERY_HPP

class Battery {
private:
	int charge_;
	int chargeMAX_;
	int chargeIncrement_;
	
public:
	Battery(int capacity) :
		chargeMAX_(capacity),
		charge_(capacity)
	{}

	bool discharge(int charge)
	{
		if (charge <= charge_)
		{
			charge_ -= charge;
			return true;
		}

		else return false;
	}

	void recharge(int charge)
	{
		charge_ = charge_ + charge <= chargeMAX_ ? charge_ + charge : chargeMAX_;
	}

	void refill()
	{
		charge_ = chargeMAX_;
	}

	int getCharge() const { return charge_; }
	int getPercent() const { return ((float)charge_ / (float)chargeMAX_) * 100.f; }
	bool checkFull() const { return charge_ == chargeMAX_; }
	bool checkEmpty() const { return charge_ == 0; }
};
#endif