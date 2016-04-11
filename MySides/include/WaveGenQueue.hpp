#ifndef MS_WAVEGENQ_HPP
#define MS_WAVEGENQ_HPP

#include <queue>

#include "WaveGenerator.hpp"

namespace WaveGen
{
	class WaveGeneratorQueue : public WaveGen::WaveGeneratorI {
	public:
		WaveGeneratorQueue();
		Wave getWave();
		bool isEmpty() const;

		void pushWave(Wave& const wave);
		
	private:
		std::queue<Wave> waves_;
	};

}

#endif