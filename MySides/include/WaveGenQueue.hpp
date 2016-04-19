#ifndef MS_WAVEGENQ_HPP
#define MS_WAVEGENQ_HPP

#include <queue>
#include "WaveGenerator.hpp"

namespace WaveGen
{
	class WaveGeneratorQueue : public WaveGeneratorI {
	public:
		WaveGeneratorQueue();
		WaveGeneratorQueue(const WaveGeneratorQueue& other);

		Wave getWave();
		bool isEmpty() const;

		void pushWave(const Wave& wave);
	private:
		std::queue<Wave> waves_;
	};

}

#endif