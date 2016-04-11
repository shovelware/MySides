#ifndef MS_LEVELWAVEQ_HPP
#define MS_LEVELWAVEQ_HPP

#include "Level.hpp"

namespace Level {

	class WaveQueue : public Level::LevelI
	{
	public:
		WaveQueue(ShapeDef player);
		~WaveQueue();

		void addWaveToQueue(Wave& const wave);
		void updateCurrentWaveCount(int count);

		bool getWaveReady() const;
		Wave getWave();

		void update(int milliseconds, bool player);

	private:
		WaveGen::WaveGeneratorQueue* waveGen_;
		int waves_;

		int curWaveCount_;
		int curWaveCountFull_;
	};

}

#endif
