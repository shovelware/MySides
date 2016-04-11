#ifndef MS_WAVEGEN_HPP
#define MS_WAVEGEN_HPP

#include "Wave.hpp"

namespace WaveGen {
	class WaveGeneratorI {
	public:
		virtual Wave getWave() = 0;
		virtual bool isEmpty() const = 0;
	private:
	};
}
#endif