#ifndef MS_MENULEVEL_HPP
#define MS_MENULEVEL_HPP

#include "Level.hpp"
#include "Wave.hpp"

namespace Level {

	class Menu : public Level::LevelI
	{
	public:
		Menu(std::string id, const PlayerDef& player, int levelCount);
		Menu(const Menu& other);

		LevelI* clone();

		void addLevel(const Level::LevelI& level);

		void updateStatus(int sides, int enemies, bool player);
		void updateSelection(int faction);

		void complete();
		void reset();
		int getSelection() const;

		bool getWaveReady() const;
		Wave getWave();

		void update(int milliseconds);
	private:
		Wave levels_;
		int selection_;
		bool spawned_;

		int& fullWaveCount_;
		int currentWaveCount_;
	};
}
#endif