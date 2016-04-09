#include "Level.hpp"

Level::Level() :
boundsPoints_(32),
boundsRadius_(32),
time_(0)
{
}

Level::Level(int limit, ShapeDef player) :
	player_(player),
	timeMAX_(limit),
	playerWeapon_("rifle"),
	playerWeaponLevel_(4),
	boundsPoints_(32),
	boundsRadius_(32),
	time_(0)
{
	
}

int Level::getLimit() const
{
	return time_ / 1000;
}

int Level::getLimitMAX() const
{
	return timeMAX_ / 1000;
}

int Level::getTime() const
{
	return time_ / 1000;
}

ShapeDef& Level::getPlayer()
{
	return player_;
}

std::string Level::getPlayerWeapon() const
{
	return playerWeapon_;
}

int Level::getPlayerWeaponLevel() const
{
	return playerWeaponLevel_;
}

void Level::addAFX(std::string path, float nearFactor, float farFactor, float nearDistance, float farDistance)
{
	AFXDef a;
	a.path = path;
	a.nearFactor = nearFactor;
	a.farFactor = farFactor;
	a.nearDistance = nearDistance;
	a.farDistance = farDistance;

	afx_.push(a);
}

std::queue<Level::AFXDef>& Level::getAFX()
{
	return afx_;
}

b2Color Level::getPrimary() const { return colPrim_; }
b2Color Level::getSecondary() const { return colSecn_; }
b2Color Level::getTertiary() const { return colTert_; }

void Level::setPrimary(b2Color col) { colPrim_ = col; }
void Level::setSecondary(b2Color col) { colSecn_ = col; }
void Level::setTertiary(b2Color col) { colTert_ = col; }

void Level::update(int milliseconds)
{
	time_ += milliseconds;
}
