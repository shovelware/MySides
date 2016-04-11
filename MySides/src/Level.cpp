#include "Level.hpp"

namespace Level
{
	LevelI::LevelI(std::string id) :
		id_(id),
		boundsPoints_(32),
		boundsRadius_(32),
		time_(0),
		timeMAX_(0),
		limit_(0),
		limitMAX_(0),
		waveLimit_(0),
		waveLimitMAX_(0),
		started_(false)
	{
	}

	LevelI::LevelI(std::string id, ShapeDef player) :
		id_(id),
		player_(player),
		boundsPoints_(32),
		boundsRadius_(32),
		time_(0),
		timeMAX_(0),
		limit_(0),
		limitMAX_(0),
		waveLimit_(0),
		waveLimitMAX_(0),
		started_(false)
	{

	}

	LevelI::LevelI(const LevelI & other) :
		started_(other.started_),
		time_(other.time_),
		timeMAX_(other.timeMAX_),
		timeComplete_(other.timeComplete_),
		limit_(other.limit_),
		limitMAX_(other.limitMAX_),
		waveLimit_(other.waveLimit_),
		waveLimitMAX_(other.waveLimitMAX_),
		colPrim_(other.colPrim_),
		colSecn_(other.colSecn_),
		colTert_(other.colTert_),
		boundsRadius_(other.boundsRadius_),
		boundsPoints_(other.boundsPoints_),
		player_(other.player_),
		playerWeapon_(other.playerWeapon_),
		playerWeaponLevel_(other.playerWeaponLevel_),
		id_(other.id_)
	{
	}

	LevelI::~LevelI()
	{
	}

	std::string LevelI::getID() const
	{
		return id_;
	}

	void LevelI::start()
	{
		if (!started_) started_ = true;
	}

	bool LevelI::getStarted() const
	{
		return started_;
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

	int LevelI::getWaveLimit() const
	{
		return waveLimit_;
	}

	int LevelI::getWaveLimitMax() const
	{
		return waveLimitMAX_;
	}

	int LevelI::getTime() const
	{
		return time_ / 1000;
	}

	int LevelI::getTimeMAX() const
	{
		return timeMAX_ / 1000;
	}

	int LevelI::getTimeComplete() const
	{
		return timeComplete_ / 1000;
	}

	bool LevelI::getComplete() const
	{
		return (limit_ >= limitMAX_);
	}

	bool LevelI::getTimeStandard() const
	{
		if (limit_ >= limitMAX_ && timeComplete_ > -1)
		{
			if (timeMAX_ == 0) return true;
			else if (timeComplete_ < timeMAX_) return true;
		}

		else return false;
	}

	void LevelI::setBoundsRadius(float radius) { boundsRadius_ = (radius >= 4.f ? radius : boundsRadius_); }
	float LevelI::getBoundsRadius() const { return boundsRadius_; }

	void LevelI::setBoundsPoints(int points) { boundsPoints_ = (points > 2 ? points : boundsPoints_); }
	int LevelI::getBoundsPoints() const { return boundsPoints_; }

	ShapeDef const & const LevelI::getPlayer() const
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

	std::queue<Level::AFXDef>const & const LevelI::getAFX() const
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