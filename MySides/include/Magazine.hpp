#ifndef MS_WMAGAZINE_HPP
#define MS_WMAGAZINE_HPP

class Magazine {
private:
	int count_;
	int countMAX_;

public:
	Magazine(int maxRounds) :
		countMAX_(maxRounds),
		count_(maxRounds)
	{

	}

	void add()
	{
		if (count_ + 1 <= countMAX_)
		{
			count_ += 1;
		}

		else
		{
			count_ = countMAX_;
		}
	}

	void remove()
	{
		if (count_ - 1 >= 0)
		{
			count_ -= 1;
		}

		else
		{
			count_ = 0;
		}
	}

	void reload()
	{
		count_ = countMAX_;
	}
	
	void empty()
	{
		count_ = 0;
	}

	void resize(int newCount, bool reload = false)
	{
		if (newCount > 0)
		{
			countMAX_ = newCount;

			if (count_ > newCount)
			{
				count_ = newCount;
			}

			if (reload)
			{
				count_ = countMAX_;
			}
		}
	}

	int getCount() const { return count_; }
	int getCountMAX() const { return countMAX_; }
	int getPercent() const { return ((float)count_ / (float)countMAX_) * 100.f; }
	bool checkFull() const { return count_ == countMAX_; }
	bool checkEmpty() const {	return (count_ <= 0); }
};
#endif