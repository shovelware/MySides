#include "Wave.hpp"

Wave::Wave()
{
}

void Wave::addEnemy(ShapeDef shape, std::string weapon, int weaponLevel)
{
	ArmedShapeDef newDef;
	newDef.shape = shape;
	newDef.weapon = weapon;
	newDef.weaponLevel = weaponLevel;

	vertsMaxNum_ += shape.verticesMax;
	vertsMinNum_ += shape.verticesMin;
	vertsNum_ += shape.vertices;

	wave_.push_back(newDef);

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

		for (std::vector<ArmedShapeDef>::const_iterator iter = wave_.begin(), end = wave_.end(); iter != end; ++iter)
		{
			vertMaxNum += iter->shape.verticesMax;
			vertMinNum += iter->shape.verticesMin;
			vertNum += iter->shape.vertices;
		}

		avgVertsMax_ = std::ceil(vertMaxNum / cnt);
		avgVertsMin_ = std::ceil(vertMinNum / cnt);
		avgVerts_ = std::ceil(vertNum / cnt);
	}
}

int Wave::getAverageVertsMax() const { return avgVertsMax_; }
int Wave::getAverageVertsMin() const { return avgVertsMin_; }
int Wave::getAverageVerts() const {	return avgVerts_; }

std::vector<Wave::ArmedShapeDef>& const Wave::getWave()
{
	return wave_;
}
