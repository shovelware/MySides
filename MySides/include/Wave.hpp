#ifndef MS_WAVE_HPP
#define MS_WAVE_HPP

#include <string>
#include <vector>

#include "EnemyDef.hpp"

class Wave {
public:
	Wave();
	Wave(const Wave& other);

	void addEnemy(EnemyDef enemy, int amount = 1);

	void calculateStats();

	int getAverageVertsMax() const;
	int getAverageVertsMin() const;
	int getAverageVerts() const;

	int getCount() const;

	std::vector<std::pair<EnemyDef, int>>& getWave();
private:
	std::vector<std::pair<EnemyDef, int>> wave_;

	int count_;

	int vertsMaxNum_;
	int vertsMinNum_;
	int vertsNum_;

	int avgVertsMax_;
	int avgVertsMin_;
	int avgVerts_;

	int modeVertsMax_;
	int modeVertsMin_;
	int modeVerts_;
};
#endif