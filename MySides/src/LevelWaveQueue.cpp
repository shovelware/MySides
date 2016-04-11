#include "..\include\LevelWaveQueue.hpp"
namespace Level {
	WaveQueue::WaveQueue(ShapeDef player) :
		LevelI(player),
		waves_(0),
		curWaveCountFull_(0),
		curWaveCount_(0)
	{
		limit_ = 0;
		limitMAX_ = 0;
		waveGen_ = new WaveGen::WaveGeneratorQueue();
	}

	WaveQueue::~WaveQueue()
	{
		delete waveGen_;
	}

	void WaveQueue::addWaveToQueue(Wave& const wave)
	{
		waveGen_->pushWave(wave);
		waves_++;
		limitMAX_++;
	}

	void WaveQueue::updateCurrentWaveCount(int count)
	{
		if (curWaveCount_ != 0 && count == 0)
		{
			limit_ += 1;
		}

		curWaveCount_ = count;
	}

	bool WaveQueue::getWaveReady() const
	{
		return (curWaveCount_ == 0 && !waveGen_->isEmpty());
	}

	Wave WaveQueue::getWave()
	{
		if (!waveGen_->isEmpty())
		{
			Wave w = waveGen_->getWave();
			curWaveCountFull_ = w.getWave().size();
			curWaveCount_ = curWaveCountFull_;
			return w;
		}

		else return Wave();
	}

	void WaveQueue::update(int milliseconds, bool player)
	{
		LevelI::update(milliseconds, player);

		if (curWaveCount_ == 0)
		{
			limit_++;
		}

		if (player)
		{
			//Do wave management stuff
		}
	}
}