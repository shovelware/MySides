#ifndef MS_LEVELWAVEQ_HPP
#define MS_LEVELWAVEQ_HPP

#include "Level.hpp"

namespace Level {

	class WaveQueue : public Level::LevelI
	{
	public:
		WaveQueue(std::string id, ShapeDef player);
		WaveQueue(const WaveQueue& other);

		LevelI* clone();
		
		~WaveQueue();

		void addWaveToQueue(Wave& const wave);
		void updateCurrentWaveCount(int count);

		void setRespiteTime(int time);
		
		bool getWaveReady() const;
		Wave getWave();

		void update(int milliseconds, bool player);

	private:
		WaveGen::WaveGeneratorQueue* waveGen_;
		int waves_;

		int& curWaveCount_;	//Count of enemies alive in active wave
		int& curWaveCountFull_; //Total no. of enemies in active wave

		int& wavesMAX_;
		int& wavesComplete_;

		int respiteTimeMAX_; //Respite time between waves
		int respiteTime_; //Timer for respite

		int minEnemies_; //Minimum enemies to trigger another spawn
	};

}

#endif
