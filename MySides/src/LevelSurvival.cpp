#include "LevelSurvival.hpp"
#include "..\include\LevelSurvival.hpp"

namespace Level {
	Survival::Survival(std::string id, const PlayerDef& player) :
		LevelI(id, player),
		waveGen_(WaveGen::WaveGeneratorSLoop()),
		timeSurvived_(limit_),
		timeSurvivedMAX_(limitMAX_),
		waves_(levelInfoAlpha_),
		curWaveCount_(levelInfoBeta_),
		maxWaveCount_(levelInfoGamma_)
	{
		timeSurvived_ = 0;
		timeSurvivedMAX_ = 60;
		respiteTimeMAX_ = 2500;
		respiteTime_ = 0;
		waves_ = 0;
		curWaveCount_ = 0;
	}

	Survival::Survival(const Survival& other) :
		LevelI(other),
		waveGen_(other.waveGen_),
		timeSurvived_(limit_),
		timeSurvivedMAX_(limitMAX_),
		waves_(levelInfoAlpha_),
		curWaveCount_(levelInfoBeta_),
		maxWaveCount_(levelInfoGamma_)
	{
		timeSurvived_ = other.timeSurvived_;
		timeSurvivedMAX_ = other.timeSurvivedMAX_;
		waves_ = other.waves_;
		curWaveCount_ = other.curWaveCount_;
	}

	//inherited virtual clone function
	LevelI* Survival::clone()
	{
		return static_cast<LevelI*>(new Survival(*this));
	}

	void Survival::start()
	{
		Level::LevelI::start();
		//respiteTime_ = respiteTimeMAX_ / 2;
	}

	void Survival::addPaletteWave(const Wave& wave)
	{
		waveGen_.pushPalette(wave);
	}

	void Survival::setBase(const EnemyDef& base)
	{
		waveGen_.setBase(base);
	}

	void Survival::setMod(const EnemyDef& mod)
	{
		waveGen_.setMod(mod);
	}

	void Survival::setModMAX(const EnemyDef& modMAX)
	{
		waveGen_.setModMAX(modMAX);
	}

	void Survival::setWaveSizeMod(float mod)
	{
		waveGen_.setWaveSizeMultMod(mod);
	}

	void Survival::setWaveSizeMAX(float max)
	{
		waveGen_.setWaveSizeMultMAX(max);
	}

	void Survival::setSurvivalTime(int seconds)
	{
		timeSurvivedMAX_ = seconds > 0 ? seconds : timeSurvivedMAX_;
	}

	void Survival::updateStatus(int sides, int enemies, bool player)
	{
		Level::LevelI::updateStatus(sides, enemies, player);
		curWaveCount_ = enemies;
	}

	bool Survival::getWaveReady() const
	{
		return (respiteTime_ == 0 && !waveGen_.isEmpty() && timeSurvived_ < timeSurvivedMAX_);
	}

	Wave Survival::getWave()
	{
		if (!waveGen_.isEmpty())
		{
			Wave w = waveGen_.getWave();
			waves_++;
			curWaveCount_ = w.getCount();
			respiteTime_ = respiteTimeMAX_;
			return w;
		}

		else return Wave();
	}

	void Survival::update(int milliseconds)
	{
		Level::LevelI::update(milliseconds);

		timeSurvived_ = time_ / 1000;
		maxWaveCount_ = waveGen_.getWaveCount();

		if (playerAlive_)
		{
			//Time down our respite between waves
			if (respiteTime_ != 0)
			{
				respiteTime_ = (respiteTime_ - milliseconds > 0 ? respiteTime_ - milliseconds : 0);
			}
		}
	}
}