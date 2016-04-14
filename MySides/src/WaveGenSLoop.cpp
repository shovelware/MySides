#include "WaveGenSLoop.hpp"

WaveGen::WaveGeneratorSLoop::WaveGeneratorSLoop() :
	waveCount_(0),
	currentWave_(0),
	paletteWaveCount_(0),
	loops_(0),
	loopsMAX_(-1),
	waveSizeMultMod_(0),
	waveSizeMultMAX_(1),
	base_(EnemyDef::baseDef()),
	mod_(EnemyDef::modDef()),
	modMAX_(EnemyDef::maxDef())
{
}

WaveGen::WaveGeneratorSLoop::WaveGeneratorSLoop(const WaveGeneratorSLoop & other) :
	palette_(other.palette_),
	waveCount_(other.waveCount_),
	currentWave_(other.currentWave_),
	paletteWaveCount_(other.paletteWaveCount_),
	base_(other.base_),
	mod_(other.mod_),
	modMAX_(other.modMAX_),
	loops_(other.loops_),
	loopsMAX_(other.loopsMAX_),
	waveSizeMultMod_(other.waveSizeMultMod_),
	waveSizeMultMAX_(other.waveSizeMultMAX_)
{}

Wave WaveGen::WaveGeneratorSLoop::getWave()
{
	std::vector<std::pair<EnemyDef, int>> palWave = palette_[currentWave_].getWave();
	Wave retWave = Wave();
	
	for (std::vector<std::pair<EnemyDef, int>>::iterator iter = palWave.begin(), end = palWave.end(); iter != end; ++iter)
	{
		EnemyDef e = iter->first;
		int count = iter->second;

		//Mod count: mult by fmax(maxMult, 1 + multMod * loops)
		count *= fmax(1.f, fminf(1.f + (waveSizeMultMod_ * loops_), waveSizeMultMAX_));

		//Mod verts: add to verts, respecting hard min and max
		int vertsMin = e.verticesMin + (mod_.verticesMin * loops_ < modMAX_.verticesMin ? mod_.verticesMin * loops_ : modMAX_.verticesMin);
		int verts  = e.vertices + (mod_.vertices * loops_ < modMAX_.vertices ? mod_.vertices * loops_ : modMAX_.vertices);
		int vertsMax = e.verticesMax + (mod_.verticesMax * loops_ < modMAX_.verticesMax ? mod_.verticesMax * loops_ : modMAX_.verticesMax);

		e.verticesMin = fmaxf(2, fminf(vertsMin, 8));
		e.vertices = fmaxf(2, fminf(verts, 8));
		e.verticesMax = fmaxf(2, fminf(vertsMax, 8));

		//Mod stats: mult by [fmax(1, Base Stat + (Stat Modifier * loops_)) until Stat Modifier reaches Modifier Max]
		e.size *= fmax(1.f, (base_.size + (mod_.size * loops_ < modMAX_.size ? mod_.size * loops_ : modMAX_.size)));
		e.hpScale *= fmax(1.f, (base_.hpScale + (mod_.hpScale * loops_ < modMAX_.hpScale ? mod_.hpScale * loops_ : modMAX_.hpScale)));
		e.speedScale *= fmax(1.f, (base_.speedScale + (mod_.speedScale * loops_ < modMAX_.speedScale ? mod_.speedScale * loops_ : modMAX_.speedScale)));
		e.damageScale *= fmax(1.f, (base_.damageScale + (mod_.damageScale * loops_ < modMAX_.damageScale ? mod_.damageScale * loops_ : modMAX_.damageScale)));

		retWave.addEnemy(e, count);
	}

	//Move to next wave
	currentWave_++;

	//If we've reached the end of the palette
	if (currentWave_ == paletteWaveCount_)
	{
		//If we can still loop, do so
		if (loops_ <= loopsMAX_ || loopsMAX_ == -1)
		{
			loops_++;

			//If we're not done now, roll around to the start
			if (loops_ != loopsMAX_ || loopsMAX_ == -1)
				currentWave_ = 0;
		}
	}

	return retWave;
}

bool WaveGen::WaveGeneratorSLoop::isEmpty() const
{
	return (currentWave_ == paletteWaveCount_);
}

void WaveGen::WaveGeneratorSLoop::pushPalette(const Wave & wave)
{
	palette_.push_back(wave);
	paletteWaveCount_++;

	if (loopsMAX_ != -1)
		waveCount_ += loopsMAX_;

	else waveCount_ = 999;
}

void WaveGen::WaveGeneratorSLoop::setBase(const EnemyDef & base)
{
	base_ = base;
}

void WaveGen::WaveGeneratorSLoop::setMod(const EnemyDef & mod)
{
	mod_ = mod;
}

void WaveGen::WaveGeneratorSLoop::setModMAX(const EnemyDef & modMax)
{
	modMAX_ = modMax;
}

void WaveGen::WaveGeneratorSLoop::setWaveSizeMultMod(int mod)
{
	waveSizeMultMod_ = mod >= 0 ? mod : waveSizeMultMod_;
}

void WaveGen::WaveGeneratorSLoop::setWaveSizeMultMAX(int max)
{
	waveSizeMultMAX_ = max > 0 ? max : waveSizeMultMAX_;
}

void WaveGen::WaveGeneratorSLoop::setLoopsMAX(int loops)
{
	if (loops > -2)
	{
		//Get our current count, respecting infinite
		if (loopsMAX_ != -1)
		{
			int cnt = waveCount_ / loopsMAX_;
			waveCount_ = cnt * loops;
		}

		loopsMAX_ = loops;

		//Max waves if we're infinite
		if (loopsMAX_ == -1)
			waveCount_ = 999;
	}
}

int WaveGen::WaveGeneratorSLoop::getLoops() const
{
	return loops_;
}

int WaveGen::WaveGeneratorSLoop::getLoopsMAX() const
{
	return loopsMAX_;
}

int WaveGen::WaveGeneratorSLoop::getWaveCount() const
{
	return waveCount_;
}
