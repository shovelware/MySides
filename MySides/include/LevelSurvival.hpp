#ifndef MS_LEVELSURVIVAL_HPP
#define MS_LEVELSURVIVAL_HPP

#include "Level.hpp"
#include "WaveGenSLoop.hpp"

namespace Level {
	class Survival : public LevelI{
	public:
		Survival(std::string id, const PlayerDef& player);
		Survival(const Survival& other);

		LevelI* clone();

		void start();

		void addPaletteWave(const Wave& wave);
		void setBase(const EnemyDef& base);
		void setMod(const EnemyDef& mod);
		void setModMAX(const EnemyDef& modMAX);
		void setWaveSizeMod(float mod);
		void setWaveSizeMAX(float max);

		void setSurvivalTime(int seconds);

		void updateStatus(int sides, int enemies, bool player);

		bool getWaveReady() const;
		Wave getWave();

		void update(int milliseconds);
	private:
		WaveGen::WaveGeneratorSLoop waveGen_;

		int& timeSurvived_;
		int& timeSurvivedMAX_;

		int& waves_;
		int& curWaveCount_;
		int& maxWaveCount_;
	};
	
};

#endif