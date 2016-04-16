#ifndef MS_LEVEL_HPP
#define MS_LEVEL_HPP

#include <stdafx.h>
#include <Box2D\Box2D.h>
#include <queue>

#include "EnemyDef.hpp"
#include "PlayerDef.hpp"
#include "SideDef.hpp"
#include "ProjectileDef.hpp"
#include "WaveGenerator.hpp"

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
		LevelI(std::string id);
		LevelI(std::string id, const PlayerDef& player);

		LevelI(const LevelI& other);
		virtual LevelI* clone() = 0;

		std::string getID() const;

		virtual void start();
		virtual void stop();
		bool getStarted() const;

		//Limit for completion
		int getLimit() const;
		int getLimitMAX() const;
		bool isComplete() const;

		//Wave information
		int getInfoAlpha() const;
		int getInfoBeta() const;
		int getInfoGamma() const;

		int getTime() const;
		int getTimeMAX() const;
		int getTimeComplete() const;
		bool getComplete() const;
		bool getTimeStandard() const;

		void setBoundsRadius(float radius);
		float getBoundsRadius() const;

		void setBoundsPoints(int points);
		int getBoundsPoints() const;

		void setRespiteTimeMAX(int time);
		int getRespiteTimeMAX() const;
		int getRespiteTime() const;
		void forceWave();

		PlayerDef const & const getPlayer() const;

		void addAFX(std::string path, float nearFactor, float farFactor, float nearDistance, float farDistance);
		std::queue<AFXDef>const &const getAFX() const;

		b2Color getPrimary() const;
		b2Color getSecondary() const;
		b2Color getTertiary() const;

		void setPrimary(b2Color col);
		void setSecondary(b2Color col);
		void setTertiary(b2Color col);

		virtual bool getWaveReady() const = 0;
		virtual Wave getWave() = 0;

		virtual void updateStatus(int sides, int enemies, bool player) = 0;

		virtual void update(int milliseconds);
	protected:
		std::queue<Level::AFXDef> afx_;

		bool started_;

		int time_;
		int timeMAX_;
		int timeComplete_;
		
		//For use as ref in derived, what marks the level as "complete"
		int limit_;
		int limitMAX_;

		//For use as ref in derived, level info
		int levelInfoAlpha_;
		int levelInfoBeta_;
		int levelInfoGamma_;

		b2Color colPrim_;
		b2Color colSecn_;
		b2Color colTert_;

		//Time between waves
		int respiteTimeMAX_;
		int respiteTime_;

		float boundsRadius_;
		int boundsPoints_;
		
		bool playerAlive_;
		PlayerDef player_;

		std::string id_;
	};
}

#endif