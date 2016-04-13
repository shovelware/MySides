#ifndef MS_WAVE_HPP
#define MS_WAVE_HPP

#include <string>
#include <vector>

#include "EnemyDef.hpp"

class Wave {
public:
	Wave();

	void addEnemy(EnemyDef enemy);

	void calculateStats();

	int getAverageVertsMax() const;
	int getAverageVertsMin() const;
	int getAverageVerts() const;

	std::vector<EnemyDef>& const getWave();
private:
	std::vector<EnemyDef> wave_;

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