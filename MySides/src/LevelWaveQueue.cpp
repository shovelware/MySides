#include "..\include\LevelWaveQueue.hpp"
namespace Level {
	WaveQueue::WaveQueue(std::string id, const PlayerDef& player) :
		LevelI(id, player),
		waves_(0),
		curWaveCountFull_(waveLimitMAX_),
		curWaveCount_(waveLimit_),
		wavesMAX_(limitMAX_),
		wavesComplete_(limit_)
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

	void WaveQueue::updateStatus(int sides, int enemies, bool player)
	{
		Level::LevelI::updateStatus(sides, enemies, player);
		curWaveCount_ = enemies;
		
		//Add to our waves cleared, record our complete time, start respite
		if (curWaveCountFull_ != 0 && wavesComplete_ != wavesMAX_ && curWaveCount_ == 0)
		{
			wavesComplete_ = (wavesComplete_ + 1 < wavesMAX_ ? wavesComplete_ + 1 : wavesMAX_);

			curWaveCountFull_ = 0;

			//If we're finished
			if (wavesComplete_ == wavesMAX_)
			{
				timeComplete_ = time_;
			}
			
			//Else prepare next wave
			else respiteTime_ = respiteTimeMAX_;
		}
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

	void WaveQueue::update(int milliseconds)
	{
		LevelI::update(milliseconds);

		if (playerAlive_)
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