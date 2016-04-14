#ifndef MS_WAVEGENSLOOP_HPP
#define MS_WAVEGENSLOOP_HPP

#include <vector>
#include "WaveGenerator.hpp"

namespace WaveGen {
	class WaveGeneratorSLoop : public WaveGen::WaveGeneratorI {
	public:
		WaveGeneratorSLoop();
		WaveGeneratorSLoop(const WaveGeneratorSLoop& other);

		Wave getWave();
		bool isEmpty() const;

		void pushPalette(const Wave& wave);
		void setBase(const EnemyDef& base);
		void setMod(const EnemyDef& mod);
		void setModMAX(const EnemyDef& modMax);

		void setWaveSizeMultMod(int mod);
		void setWaveSizeMultMAX(int max);

		void setLoopsMAX(int loops);
		int getLoops() const;
		int getLoopsMAX() const;
		int getWaveCount() const;
	private:
		std::vector<Wave> palette_; //!< Palette of waves for 1 loop
		int paletteWaveCount_;	//!< Count of waves in palette
		EnemyDef base_;		//!<Base values for shape modification
		EnemyDef mod_;		//!<Mod values for shape modification after 1 loop
		EnemyDef modMAX_;	//!<Max values for shape modification after max/mod loops

		int waveCount_;		//!< Count of waves in palette * loops
		int currentWave_;	 //!< Current wave in current loop
		int loops_;			//!< How many loops we've done
		int loopsMAX_;		//!< Maximum number of loops

		
		float waveSizeMultMod_; //!< Wave size multiplier increase after loop 1
		float waveSizeMultMAX_; //!< Max wave size multiplier after x loops

		//Mod : stat = stat += mod;
		//Mult : stat = stat * mult;

		//int vertMod_;		//!< Mod of verts after loop 1
		//int vertModMAX_;	//!< Max verts mod after x loops
		//
		//int weapMod_;	//!< Mod of weapon level after loop 1
		//int weapModMAX_; //!< Max weapon level mod after x loops
		//
		//
		//float hpScaleMult_; //!< HP Scale multiplier
		//float hpScaleMultMod_; //!< HP Scale multiplier increase after loop 1
		//float hpScaleMultMAX_; //!< HP Scale multiplier after x loops
	};
}
#endif