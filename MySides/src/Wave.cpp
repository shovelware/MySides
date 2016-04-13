#include "Wave.hpp"

Wave::Wave()
{}

void Wave::addEnemy(EnemyDef enemy)
{
	vertsMaxNum_ += enemy.verticesMax;
	vertsMinNum_ += enemy.verticesMin;
	vertsNum_ += enemy.vertices;

	wave_.push_back(enemy);

	int count = wave_.size();

	if (count != 0)
	{
		avgVertsMax_ = vertsMaxNum_ / count;
		avgVertsMin_ = vertsMinNum_ / count;
		avgVerts_ = vertsNum_ / count;
	}
}

void Wave::calculateStats()
{
	if (!wave_.empty())
	{
		int cnt = wave_.size();

		int vertMinNum = 0;
		int vertMaxNum = 0;
		int vertNum = 0;

		for (std::vector<EnemyDef>::const_iterator iter = wave_.begin(), end = wave_.end(); iter != end; ++iter)
		{
			vertMaxNum += iter->verticesMax;
			vertMinNum += iter->verticesMin;
			vertNum += iter->vertices;
		}

		avgVertsMax_ = std::ceil(vertMaxNum / cnt);
		avgVertsMin_ = std::ceil(vertMinNum / cnt);
		avgVerts_ = std::ceil(vertNum / cnt);
	}
}

int Wave::getAverageVertsMax() const { return avgVertsMax_; }
int Wave::getAverageVertsMin() const { return avgVertsMin_; }
int Wave::getAverageVerts() const {	return avgVerts_; }

std::vector<EnemyDef>& const Wave::getWave()
{
	return wave_;
}
