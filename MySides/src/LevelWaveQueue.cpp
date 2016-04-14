#include "LevelWaveQueue.hpp"

namespace Level {
	WaveQueue::WaveQueue(std::string id, const PlayerDef& player) :
		LevelI(id, player),
		waves_(0),
		curWaveCountFull_(levelInfoGamma_),
		curWaveCount_(levelInfoBeta_),
		curWaveThreshold_(levelInfoAlpha_),
		leftovers_(0),
		wavesMAX_(limitMAX_),
		wavesComplete_(limit_),
		waveGen_(WaveGen::WaveGeneratorQueue())
	{
		waves_ = 0;
		wavesMAX_ = 0;
		curWaveThreshold_ = 0;
		curWaveCount_ = 0;
		curWaveCountFull_ = 0;
	}

	WaveQueue::WaveQueue(const WaveQueue& other) :
		LevelI(other),
		waves_(other.waves_),
		curWaveCountFull_(levelInfoGamma_),
		curWaveCount_(levelInfoBeta_),
		curWaveThreshold_(levelInfoAlpha_),
		leftovers_(other.leftovers_),
		wavesMAX_(limitMAX_),
		wavesComplete_(limit_),
		minEnemyRatio_(other.minEnemyRatio_),
		waveGen_(WaveGen::WaveGeneratorQueue(other.waveGen_))
	{
		levelInfoGamma_ = other.curWaveCountFull_;
		levelInfoBeta_ = other.curWaveCount_;

		limitMAX_ = other.wavesMAX_;
		limit_ = other.wavesComplete_;
		levelInfoAlpha_ = other.levelInfoAlpha_;
	}

	//inherited virtual clone function
	LevelI* WaveQueue::clone()
	{
		return static_cast<LevelI*>(new WaveQueue(*this));
	}

	void WaveQueue::addWaveToQueue(Wave& const wave)
	{
		waveGen_.pushWave(wave);
		waves_++;
		wavesMAX_++;
	}

	void WaveQueue::updateStatus(int sides, int enemies, bool player)
	{
		Level::LevelI::updateStatus(sides, enemies, player);
		curWaveCount_ = enemies;
		
		//Add to our waves cleared, record our complete time, start respite
		if (curWaveCountFull_ != 0 && wavesComplete_ != wavesMAX_ && curWaveCount_ <= curWaveThreshold_)
		{
			//If we're moving up
			if (wavesComplete_ + 1 < wavesMAX_)
			{
				wavesComplete_ = wavesComplete_ + 1;

				curWaveCountFull_ = 0;
				leftovers_ = curWaveCount_;
				respiteTime_ = respiteTimeMAX_;
			}

			//If we're finishing
			else if (wavesComplete_ + 1 <= wavesMAX_)
			{
				//Only finish if the last wave is done
				if (curWaveCount_ == 0)
				{
					wavesComplete_ = wavesMAX_;
					timeComplete_ = time_;
				}
			}
			
			//Else prepare next wave
		}
	}

	bool WaveQueue::getWaveReady() const
	{
		return (curWaveCount_ <= leftovers_ && respiteTime_ == 0 && !waveGen_.isEmpty());
	}

	Wave WaveQueue::getWave()
	{
		if (!waveGen_.isEmpty())
		{
			Wave w = waveGen_.getWave();
			curWaveCountFull_ = w.getCount();
			curWaveCount_ = curWaveCountFull_;
			curWaveThreshold_ = std::ceil((curWaveCountFull_ * minEnemyRatio_));
			return w;
		}

		else return Wave();
	}

	void WaveQueue::setMinEnemyRatio(float ratio)
	{
		minEnemyRatio_ = ((0 <= ratio && ratio <= 1) ? ratio : minEnemyRatio_);
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