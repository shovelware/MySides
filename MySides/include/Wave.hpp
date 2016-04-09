#ifndef MS_WAVE_HPP
#define MS_WAVE_HPP

#include <string>

#include "ShapeDef.hpp"

class Wave {
public:
	struct ArmedShapeDef {
		ShapeDef def = ShapeDef();
		std::string weapon = "";
		int weaponLevel = 0;
	};

	void addEnemy(ShapeDef def, std::string weapon, int weaponLevel);

	std::vector<ArmedShapeDef>& getWave() const;
private:
	std::vector<ArmedShapeDef> wave_;
};
#endif