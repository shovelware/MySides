#ifndef MS_LEVELWAVEQ_HPP
#define MS_LEVELWAVEQ_HPP

#include "Level.hpp"
#include "WaveGenQueue.hpp"

namespace Level {

	class WaveQueue : public Level::LevelI
	{
	public:
		WaveQueue(std::string id, const PlayerDef& player);
		WaveQueue(const WaveQueue& other);

		LevelI* clone();

		void addWaveToQueue(Wave& const wave);
		void updateStatus(int sides, int enemies, bool player);

		bool getWaveReady() const;
		Wave getWave();

		void setMinEnemyRatio(float ratio);

		void update(int milliseconds);

	private:
		WaveGen::WaveGeneratorQueue waveGen_;
		int waves_;
		
		float minEnemyRatio_;	//Minimum scalar of current wavesize to trigger next wave spawn
		int leftovers_;			//Leftover enemies from previous wave

		int& curWaveThreshold_;	//Calculated Threshold to spawn new wave
		int& curWaveCount_;		//Count of enemies alive in active wave
		int& curWaveCountFull_; //Total no. of enemies in active wave

		int& wavesMAX_;			//Max no. of waves in queue
		int& wavesComplete_;	//Completed waves from queue
	};

}

#endif
