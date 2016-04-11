#ifndef MS_LEVEL_HPP
#define MS_LEVEL_HPP

#include <stdafx.h>
#include <Box2D\Box2D.h>
#include <queue>

#include "Armory.hpp"
#include "SideDef.hpp"
#include "ProjectileDef.hpp"

#include "WaveGenQueue.hpp"

namespace Level {

		struct AFXDef {
			std::string path = "";
			float nearFactor = 0;
			float farFactor = 0;
			float nearDistance = 0;
			float farDistance = 0;
		};

	class LevelI {
	public:
		LevelI();
		LevelI(ShapeDef player);

		~LevelI();

		int getLimit() const;
		int getLimitMAX() const;
		bool isComplete() const;

		int getTime() const;
		int getTimeMAX() const;

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

		virtual bool getWaveReady() const = 0;
		virtual Wave getWave() = 0;

		void update(int milliseconds, bool player); //override
	protected:
		std::queue<Level::AFXDef> afx_;

		int time_;
		int timeMAX_;

		int limit_;
		int limitMAX_;

		b2Color colPrim_;
		b2Color colSecn_;
		b2Color colTert_;

		float boundsRadius_;
		int boundsPoints_;

		ShapeDef player_;
		std::string playerWeapon_;
		int playerWeaponLevel_;
	};
}

#endif