#include "WaveGenQueue.hpp"

WaveGen::WaveGeneratorQueue::WaveGeneratorQueue()
{

}

WaveGen::WaveGeneratorQueue::WaveGeneratorQueue(const WaveGeneratorQueue& other) :
	waves_(other.waves_)
{

}

Wave WaveGen::WaveGeneratorQueue::getWave()
{
	Wave top = waves_.front();
	waves_.pop();
	return top;
}

bool WaveGen::WaveGeneratorQueue::isEmpty() const
{
	return waves_.size() == 0;
}

void WaveGen::WaveGeneratorQueue::pushWave(Wave& const wave)
{
	waves_.push(wave);
}

std::queue<Wave>const & const WaveGen::WaveGeneratorQueue::getWaveQueue() const
{
	return waves_;
}
