#include "Level.hpp"

namespace Level
{
	LevelI::LevelI() :
		boundsPoints_(32),
		boundsRadius_(32),
		time_(0),
		timeMAX_(120 * 1000)
	{
	}

	LevelI::LevelI(ShapeDef player) :
		player_(player),
		boundsPoints_(32),
		boundsRadius_(32),
		timeMAX_(120 * 1000),
		time_(0)
	{

	}

	LevelI::~LevelI()
	{
	}
	
	int LevelI::getLimit() const
	{
		return limit_;
	}

	int LevelI::getLimitMAX() const
	{
		return limitMAX_;
	}

	bool LevelI::isComplete() const
	{
		return limit_ >= limitMAX_;
	}

	int LevelI::getTime() const
	{
		return time_ / 1000;
	}

	int LevelI::getTimeMAX() const
	{
		return timeMAX_ / 1000;
	}

	ShapeDef& LevelI::getPlayer()
	{
		return player_;
	}

	void LevelI::setPlayerWeapon(std::string weapon)
	{
		playerWeapon_ = weapon;
	}

	std::string LevelI::getPlayerWeapon() const
	{
		return playerWeapon_;
	}

	void LevelI::setPlayerWeaponLevel(int level)
	{
		playerWeaponLevel_ = level;
	}

	int LevelI::getPlayerWeaponLevel() const
	{
		return playerWeaponLevel_;
	}

	void LevelI::addAFX(std::string path, float nearFactor, float farFactor, float nearDistance, float farDistance)
	{
		AFXDef a;
		a.path = path;
		a.nearFactor = nearFactor;
		a.farFactor = farFactor;
		a.nearDistance = nearDistance;
		a.farDistance = farDistance;

		afx_.push(a);
	}

	std::queue<Level::AFXDef>& LevelI::getAFX()
	{
		return afx_;
	}

	b2Color LevelI::getPrimary() const { return colPrim_; }
	b2Color LevelI::getSecondary() const { return colSecn_; }
	b2Color LevelI::getTertiary() const { return colTert_; }

	void LevelI::setPrimary(b2Color col) { colPrim_ = col; }
	void LevelI::setSecondary(b2Color col) { colSecn_ = col; }
	void LevelI::setTertiary(b2Color col) { colTert_ = col; }

	void LevelI::update(int milliseconds, bool player)
	{
		time_ += milliseconds;
	}
}