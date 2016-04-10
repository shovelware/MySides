#ifndef MS_WAVE_HPP
#define MS_WAVE_HPP

#include <string>
#include <vector>

#include "ShapeDef.hpp"

class Wave {
public:
	struct ArmedShapeDef {
		ShapeDef shape = ShapeDef();
		//AIDef brain AIDef();
		std::string weapon = "";
		int weaponLevel = 0;
	};
	Wave();

	void addEnemy(ShapeDef shape, std::string weapon, int weaponLevel);	//void addEnemy(ShapeDef shape, AIDef brain, std::string weapon, int weaponLevel);

	void calculateStats();

	int getAverageVertsMax() const;
	int getAverageVertsMin() const;
	int getAverageVerts() const;

	std::vector<ArmedShapeDef>& const getWave();
private:
	std::vector<ArmedShapeDef> wave_;

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