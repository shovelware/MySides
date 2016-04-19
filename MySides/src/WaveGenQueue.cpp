#include "WaveGenQueue.hpp"

namespace WaveGen {
	WaveGeneratorQueue::WaveGeneratorQueue()
	{

	}

	WaveGeneratorQueue::WaveGeneratorQueue(const WaveGeneratorQueue& other) :
		waves_(other.waves_)
	{

	}

	Wave WaveGeneratorQueue::getWave()
	{
		Wave top = waves_.front();
		waves_.pop();
		return top;
	}

	bool WaveGeneratorQueue::isEmpty() const
	{
		return waves_.size() == 0;
	}

	void WaveGeneratorQueue::pushWave(const Wave& wave)
	{
		waves_.push(wave);
	}
}