#include "WaveGenQueue.hpp"

WaveGen::WaveGeneratorQueue::WaveGeneratorQueue()
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
