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
};
#endif