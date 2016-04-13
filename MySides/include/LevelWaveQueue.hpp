#ifndef MS_LEVELWAVEQ_HPP
#define MS_LEVELWAVEQ_HPP

#include "Level.hpp"

namespace Level {

	class WaveQueue : public Level::LevelI
	{
	public:
		WaveQueue(std::string id, const PlayerDef& player);
		WaveQueue(const WaveQueue& other);

		LevelI* clone();
		
		~WaveQueue();

		void addWaveToQueue(Wave& const wave);
		void updateStatus(int sides, int enemies, bool player);

		bool getWaveReady() const;
		Wave getWave();

		void update(int milliseconds);

	private:
		WaveGen::WaveGeneratorQueue* waveGen_;
		int waves_;

		int& curWaveCount_;	//Count of enemies alive in active wave
		int& curWaveCountFull_; //Total no. of enemies in active wave

		int& wavesMAX_;			//Max no. of waves in queue
		int& wavesComplete_;	//Completed waves from queue

		int minEnemies_; //Minimum enemies to trigger another spawn
	};

}

#endif
