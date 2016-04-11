#ifndef MS_WAVEGENQ_HPP
#define MS_WAVEGENQ_HPP

#include <queue>

#include "WaveGenerator.hpp"

namespace WaveGen
{
	class WaveGeneratorQueue : public WaveGen::WaveGeneratorI {
	public:
		WaveGeneratorQueue();
		WaveGeneratorQueue(const WaveGeneratorQueue& other);

		Wave getWave();
		bool isEmpty() const;

		void pushWave(Wave& const wave);
		
		std::queue<Wave>const & const getWaveQueue() const;
	private:
		std::queue<Wave> waves_;
	};

}

#endif