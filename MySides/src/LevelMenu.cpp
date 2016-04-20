#include "LevelMenu.hpp"

namespace Level {
	Menu::Menu(std::string id, const PlayerDef & player, int levelCount) :
		Level::LevelI(id, player),
		levels_(Wave()),
		selection_(-1),
		spawned_(false),
		fullWaveCount_(limitMAX_),
		currentWaveCount_(0)
	{
		timeMAX_ = 888888;
		respiteTimeMAX_ = 0;
		fullWaveCount_ = levelCount;
	}

	Menu::Menu(const Menu & other) :
		Level::LevelI(other),
		levels_(other.levels_),
		selection_(other.selection_),
		spawned_(other.spawned_),
		fullWaveCount_(limitMAX_),
		currentWaveCount_(other.currentWaveCount_)		
	{
		fullWaveCount_ = other.fullWaveCount_;
	}

	LevelI* Level::Menu::clone()
	{
		return static_cast<LevelI*>(new Menu(*this));
	}

	void Menu::addLevel(const Level::LevelI& level)
	{
		int count = levels_.getCount();
		int rad = boundsRadius_ * fmaxf(0.15f, fminf(0.05 * (fullWaveCount_), 0.75f));

		EnemyDef enemy;
		enemy.ai = 2;
		enemy.damageScale = 0;
		enemy.spawnProtection = 0.1f;
		enemy.colPrim = level.getPrimary();
		enemy.colSecn = level.getSecondary();
		enemy.colTert = level.getTertiary();

		enemy.size = fmaxf(10.f / fmaxf(fullWaveCount_ * 1.f, 1.f), 2.f);
		int verts = fmax(3, fminf(std::ceil(randFloat(3, 8)),8));
		enemy.verticesMin = 3;
		enemy.verticesMax = 3;
		enemy.vertices = 3;
		enemy.hpScale = 1;
		enemy.dropSides = false;

		float increm = ((b2_pi * 2.f / fullWaveCount_) * count);
		b2Vec2 pos;
		pos.y = rad * (-cos(increm));
		pos.x = rad * (sin(increm));

		enemy.position = pos;
		enemy.heading = -pos;

		enemy.faction = count + 4;
		
		levels_.addEnemy(enemy);
	}

	void Menu::updateStatus(int sides, int enemies, bool player)
	{
		LevelI::updateStatus(sides, enemies, player);

		currentWaveCount_ = enemies;
	}

	void Menu::updateSelection(int faction)
	{
		selection_ = faction - 4;
	}

	void Menu::complete()
	{
		limit_ = limitMAX_;
	}

	void Menu::reset()
	{
		LevelI::isComplete();
		timeComplete_ = -1;
		selection_ = -1;
		started_ = false;
		spawned_ = false;
		limit_ = 0;
	}

	int Menu::getSelection() const
	{
		return selection_;
	}

	bool Menu::getWaveReady() const
	{
		return (!spawned_ || currentWaveCount_ < fullWaveCount_ && limit_ != limitMAX_);
	}

	Wave Menu::getWave()
	{
		if (!spawned_) spawned_ = true;
		return levels_;
	}

	void Menu::update(int milliseconds)
	{
		LevelI::update(milliseconds);
	}
}

