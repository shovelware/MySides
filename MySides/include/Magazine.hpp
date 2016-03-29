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

	bool checkEmpty()
	{
		return (count_ <= 0);
	}

	void resize(int newCount, bool reload = false)
	{
		if (newCount > 0)
		{
			countMAX_ = newCount;

			if (reload)
			{
				count_ = countMAX_;
			}
		}
	}
};
#endif