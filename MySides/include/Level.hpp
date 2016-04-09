#ifndef MS_LEVEL_HPP
#define MS_LEVEL_HPP

#include <stdafx.h>
#include <Box2D\Box2D.h>
#include <queue>

#include "ShapeDef.hpp"
#include "Armory.hpp"
#include "SideDef.hpp"
#include "ProjectileDef.hpp"
#include "Wave.hpp"

class Level {
public:
	struct AFXDef {
		std::string path = "";
		float nearFactor = 0;
		float farFactor = 0;
		float nearDistance = 0;
		float farDistance = 0;
	};

	Level();
	Level(int limit, ShapeDef player);

	/**/int getLimit() const;
	/**/int getLimitMAX() const;

	int getTime() const;

	ShapeDef& getPlayer();

	void setPlayerWeapon(std::string weapon);
	std::string getPlayerWeapon() const;

	void setPlayerWeaponLevel(int level);
	int getPlayerWeaponLevel() const;

	void addAFX(std::string path, float nearFactor, float farFactor, float nearDistance, float farDistance);
	std::queue<AFXDef>& getAFX();

	b2Color getPrimary() const;
	b2Color getSecondary() const;
	b2Color getTertiary() const;

	void setPrimary(b2Color col);
	void setSecondary(b2Color col);
	void setTertiary(b2Color col);

	void addWave(Wave w);
	Wave getWave();

	bool getWaveReady() const;

	void update(int milliseconds);//override
private:
	std::queue<AFXDef> afx_;

	int time_;
	unsigned int timeMAX_ = 5 * 60 * 1000;

	b2Color colPrim_;
	b2Color colSecn_;
	b2Color colTert_;

	float boundsRadius_;
	int boundsPoints_;

	ShapeDef player_;
	std::string playerWeapon_;
	int playerWeaponLevel_;
	std::queue<Wave> waves_;
};

#endif