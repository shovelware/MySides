#include "..\include\LevelWaveQueue.hpp"
namespace Level {
	WaveQueue::WaveQueue(std::string id, ShapeDef player) :
		LevelI(id, player),
		waves_(0),
		curWaveCountFull_(waveLimitMAX_),
		curWaveCount_(waveLimit_),
		wavesMAX_(limitMAX_),
		wavesComplete_(limit_),
		respiteTime_(0),
		respiteTimeMAX_(5000)
	{
		limit_ = 0;
		limitMAX_ = 0;
		waveLimit_ = 0;
		waveLimitMAX_ = 0;
		waveGen_ = new WaveGen::WaveGeneratorQueue();
	}

	WaveQueue::WaveQueue(const WaveQueue& other) :
		LevelI(other),
		waves_(other.waves_),
		curWaveCountFull_(waveLimitMAX_),
		curWaveCount_(waveLimit_),
		wavesMAX_(limitMAX_),
		wavesComplete_(limit_),
		respiteTimeMAX_(other.respiteTimeMAX_),
		respiteTime_(other.respiteTime_),
		minEnemies_(other.minEnemies_),
		waveGen_(new WaveGen::WaveGeneratorQueue(*other.waveGen_))
	{
		waveLimitMAX_ = other.curWaveCountFull_;
		waveLimit_ = other.curWaveCount_;
		limitMAX_ = other.wavesMAX_;
		limit_ = other.wavesComplete_;
	}

	//inherited virtual clone function
	LevelI* WaveQueue::clone()
	{
		return static_cast<LevelI*>(new WaveQueue(*this));
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
		curWaveCount_ = count;
		
		//Add to our waves cleared, record our complete time, start respite
		if (curWaveCountFull_ != 0 && wavesComplete_ != wavesMAX_ && curWaveCount_ == 0)
		{
			wavesComplete_ = (wavesComplete_ + 1 < wavesMAX_ ? wavesComplete_ + 1 : wavesMAX_);

			curWaveCountFull_ = 0;
			respiteTime_ = respiteTimeMAX_;

			if (wavesComplete_ == wavesMAX_)
			{
				timeComplete_ = time_;
			}
		}
	}

	void WaveQueue::setRespiteTime(int time)
	{
		respiteTimeMAX_ = time > 0 ? time : respiteTimeMAX_;
	}

	bool WaveQueue::getWaveReady() const
	{
		return (curWaveCount_ == 0 && respiteTime_ == 0 && !waveGen_->isEmpty());
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

		if (player)
		{
			//Time down our respite between waves
			if (respiteTime_ != 0)
			{
				respiteTime_ = (respiteTime_ - milliseconds > 0 ? respiteTime_ - milliseconds : 0);
			}
			
			//Do wave management stuff
		}
	}
}