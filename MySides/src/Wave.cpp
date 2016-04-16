#include "Wave.hpp"

Wave::Wave() : 
	count_(0), 
	vertsMaxNum_(0),
	vertsMinNum_(0),
	vertsNum_(0),
	avgVertsMax_(0),
	avgVertsMin_(0),
	avgVerts_(0),
	modeVertsMax_(0),
	modeVertsMin_(0),
	modeVerts_(0)
{}


Wave::Wave(const Wave& other) :
	count_(other.count_),
	vertsMaxNum_(other.vertsMaxNum_),
	vertsMinNum_(other.vertsMinNum_),
	vertsNum_(other.vertsNum_),
	avgVertsMax_(other.avgVertsMax_),
	avgVertsMin_(other.avgVertsMin_),
	avgVerts_(other.avgVerts_),
	modeVertsMax_(other.modeVertsMax_),
	modeVertsMin_(other.modeVertsMin_),
	modeVerts_(other.modeVerts_),
	wave_(other.wave_)
{}


void Wave::addEnemy(const EnemyDef& enemy, int amount)
{
	vertsMaxNum_ += enemy.verticesMax * amount;
	vertsMinNum_ += enemy.verticesMin * amount;
	vertsNum_ += enemy.vertices * amount;

	EnemyDef e(enemy);

	wave_.push_back(std::make_pair(e, amount));

	count_ += amount;

	if (count_ != 0)
	{
		avgVertsMax_ = vertsMaxNum_ / count_;
		avgVertsMin_ = vertsMinNum_ / count_;
		avgVerts_ = vertsNum_ / count_;
	}
}

void Wave::calculateStats()
{
	if (!wave_.empty())
	{
		int cnt = 0;
		int vertMinNum = 0;
		int vertMaxNum = 0;
		int vertNum = 0;

		for (std::vector<std::pair<EnemyDef, int>>::const_iterator iter = wave_.begin(), end = wave_.end(); iter != end; ++iter)
		{
			cnt += iter->second;
			vertMaxNum += iter->first.verticesMax * iter->second;
			vertMinNum += iter->first.verticesMin * iter->second;
			vertNum += iter->first.vertices * iter->second;
		}

		count_ = cnt;
		avgVertsMax_ = std::ceil(vertMaxNum / cnt);
		avgVertsMin_ = std::ceil(vertMinNum / cnt);
		avgVerts_ = std::ceil(vertNum / cnt);
	}
}

int Wave::getAverageVertsMax() const { return avgVertsMax_; }
int Wave::getAverageVertsMin() const { return avgVertsMin_; }
int Wave::getAverageVerts() const {	return avgVerts_; }

int Wave::getCount() const
{
	return count_;
}

std::vector<std::pair<EnemyDef, int>>& Wave::getWave()
{
	return wave_;
}
