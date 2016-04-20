#ifndef MS_LEVELS_HPP
#define MS_LEVELS_HPP

#include "Level.hpp"
#include "LevelWaveQueue.hpp"
#include "LevelSurvival.hpp"

namespace Level {
	namespace Atlas {
		
		static PlayerDef basePlayer() {
			//Player
			PlayerDef bplay = PlayerDef();
			bplay.heading = b2Vec2(0, -1);
			bplay.vertices = 6;
			bplay.colPrim = b2Color(0.6f, 0.3f, 0.9f);
			bplay.colSecn = b2Color(0.f, 1.f, 1.f);
			bplay.colTert = b2Color(1.f, 0.f, 0.f);
			bplay.upgradeSides = true;
			bplay.faction = 1;
			bplay.hpScale = 10;
			return bplay;
		}
		//Test Queue level
		static Level::LevelI* testQueue()
		{
			PlayerDef play(basePlayer());
			play.weapon = "fungun";
			play.weaponLevel = 47;
			play.bombTime = 20000;
			play.bombRadius = 15.f;


			Level::WaveQueue* qlvl = new Level::WaveQueue("testqueue", play);
			float radius = 64.f;
			Wave wav = Wave();
			EnemyDef enem;

			//qlvl_->addAFX("../assets/spriterip.ogg", 1, 0.2, 400, 1024);
			qlvl->addAFX("../assets/wind.ogg", 0, 1, 1500, 2000);
			qlvl->setPrimary(b2Color(0.4f, 0.0f, 0.4f));
			qlvl->setSecondary(b2Color(0.1f, 0.1f, 0.1f));
			qlvl->setTertiary(b2Color(0.1f, 0.8f, 0.1f));
			qlvl->setBoundsRadius(radius);
			qlvl->setBoundsPoints(32);
			qlvl->setMinEnemyRatio(0.1f);

			for (int s = 3; s <= 8; ++s)
			{
				wav = Wave();
				switch (s)
				{
				case 3:
					enem = EnemyDef::triDef();
					enem.weapon = "werfer";
					break;
				case 4:
					enem = EnemyDef::squDef();
					enem.weapon = "pistol";
					break;
				case 5:
					enem = EnemyDef::penDef();
					enem.weapon = "shotgun";
					break;
				case 6:
					enem = EnemyDef::hexDef();
					enem.weapon = "thumper";
					break;
				case 7:
					enem = EnemyDef::hepDef();
					enem.weapon = "railgun";
					break;
				case 8:
					enem = EnemyDef::octDef();
					enem.weapon = "cannon";
					break;
				}

				enem.position = b2Vec2(randFloat(-1, 1), randFloat(-1, 1));
				enem.position.Normalize();
				enem.position *= radius * 0.1f;
				enem.ai = 3;
				enem.speedScale = 0.5f;
				enem.faction = 2;

				wav.addEnemy(enem, s * 2);

				qlvl->addWaveToQueue(wav);
			}

			return qlvl;
		}
		
		//Test Weapons level
		static Level::LevelI* testWeapon()
		{
			PlayerDef play(basePlayer());
			play.weapon = "fungun";
			play.weaponLevel = 999;
			play.bombTime = 1000;
			play.bombRadius = 15.f;

			Level::WaveQueue* weaplvl = new Level::WaveQueue("testweap", play);
			float radius = 64.f;
			Wave wav = Wave();
			std::string weap = "";
			EnemyDef enem;
			float x, y;

			weaplvl->addAFX("../assets/wind.ogg", 0, 1, 1500, 2000);
			weaplvl->setPrimary(b2Color(0.6f, 0.6f, 0.6f));
			weaplvl->setSecondary(b2Color(0.1f, 0.1f, 0.1f));
			weaplvl->setTertiary(b2Color(0.9f, 0.9f, 0.1f));
			weaplvl->setRespiteTimeMAX(1000);
			weaplvl->setBoundsRadius(radius);
			weaplvl->setBoundsPoints(16);

			for (int l = 0; l < 9; ++l)
			{
				wav = Wave();
				for (int s = 0; s < 9; ++s)
				{
					enem = EnemyDef();
					enem.vertices = static_cast<int>(randFloat(3, 8) + 1);

					switch (s)
					{
					case 0:
						weap = "shotgun";
						enem.colPrim = b2Color(.75f, .75f, .75f);
						enem.colSecn = b2Color(.5f, .5f, .5f);
						enem.colTert = b2Color(.75f, .75f, .75f);
						break;

					case 1:
						weap = "pistol";
						enem.colPrim = b2Color(1, .5f, 0);
						enem.colSecn = b2Color(.75f, .25f, 0);
						enem.colTert = b2Color(1, .5f, 0);
						break;

					case 2:
						weap = "rifle";
						enem.colPrim = b2Color(1, 1, 0);
						enem.colSecn = b2Color(.75f, .75f, 0);
						enem.colTert = b2Color(1, 1, 0);
						break;

					case 3:
						weap = "cannon";
						enem.colPrim = b2Color(0.2f, 0.2f, 0.2f);
						enem.colSecn = b2Color(.1f, .1f, .1f);
						enem.colTert = b2Color(0.2f, 0.2f, 0.2f);
						break;

					case 4:
						weap = "thumper";
						enem.colPrim = b2Color(0, 0.75f, 0);
						enem.colSecn = b2Color(0, 0.5f, 0);
						enem.colTert = b2Color(0, 0.75f, 0);
						break;

					case 5:
						weap = "launcher";
						enem.colPrim = b2Color(0.7f, 0, 0);
						enem.colSecn = b2Color(0.5f, 0, 0);
						enem.colTert = b2Color(0.7f, 0, 0);
						break;

					case 6:
						weap = "coilgun";
						enem.colPrim = b2Color(0, 0, 1);
						enem.colSecn = b2Color(0, 0, .7f);
						enem.colTert = b2Color(0, 0, 1);
						break;

					case 7:
						weap = "railgun";
						enem.colPrim = b2Color(0, 1.f, 1.f);
						enem.colSecn = b2Color(0.7f, 0.7f, 0.7f);
						enem.colTert = b2Color(0.7f, 0.8f, 0.1f);
						break;

					case 8:
						weap = "werfer";
						enem.colPrim = b2Color(0.8f, 0.5f, 0.1f);
						enem.colSecn = b2Color(0.7f, 0.8f, 0.1f);
						enem.colTert = b2Color(0.2f, 0, 0);
					}

					y = -(cos((2.f * M_PI) * s / 9.f)) * 5.f;
					x = -(sin((2.f * M_PI) * s / 9.f)) * 5.f;

					enem.position = (b2Vec2(x, y));
					enem.heading = (enem.position);
					enem.ai = 1;
					enem.hpScale = l + 1;
					enem.damageScale = 0;
					enem.faction = 0;
					enem.weapon = weap;
					enem.weaponLevel = l;

					wav.addEnemy(enem);
				}

				weaplvl->addWaveToQueue(wav);
			}

			return weaplvl;
		}

		//Test Layer level
		static Level::LevelI* testLayer()
		{
			PlayerDef play(basePlayer());
			play.weapon = "fungun";
			play.weaponLevel = 999;
			play.bombTime = 15000;
			play.bombRadius = 15.f;

			Level::WaveQueue* laylvl = new Level::WaveQueue("testlayer", play);
			Wave wav = Wave();
			float radius = 32;
			b2Vec2 centre(0, 0);
			b2Vec2 pos(0, 0);
			float spawnradius = 20.f;
			EnemyDef def;
			int groups = 10;
			int shapes = 6;

			int baseHPScale = 1.25f;
			float baseSize = 0.25f;

			laylvl->addAFX("../assets/wind.ogg", 0, 1, 1500, 2000);
			laylvl->setPrimary(b2Color(0.9f, 0.6f, 0.4f));
			laylvl->setSecondary(b2Color(0.5f, 0.1f, 1.f));
			laylvl->setTertiary(b2Color(0.7f, 0.3f, 0.4f));
			laylvl->setRespiteTimeMAX(1000);
			laylvl->setBoundsRadius(radius);
			laylvl->setBoundsPoints(24);

			//for 4 sizes
			for (float i = 1; i <= groups; ++i)
			{
				centre.y = spawnradius * (cos((M_PI * 2.f) / groups * i));
				centre.x = spawnradius * (-sin((M_PI * 2.f) / groups * i));

				//for 5 shape sizes
				for (float j = 1.f, max = shapes; j <= max; ++j)
				{
					float ang = atan2f(centre.y, centre.x);
					pos.y = 1.f * (cos((M_PI * 2.f) / max * j));
					pos.x = 1.f * (-sin((M_PI * 2.f) / max * j));

					def.size = i * baseSize;
					def.heading = pos + centre;

					def.position = pos;
					def.position *= (def.size * 2);
					def.position += centre;

					def.vertices = 3;
					def.vertices += (((int)j - 1) % 6);

					def.hpScale = 5 * baseHPScale;
					def.colPrim = b2Color(0.2f * i, 0.1f * j, 1.f - j / 10);
					def.colSecn = b2Color(0.2f * j, 0.1f * (i / j), 0.05f * (i + j));
					def.colTert = b2Color(0.1f * i, 0.3f * i, 1.f * (j - i));
					def.ai = 2;
					def.faction = 2;

					wav.addEnemy(def);
				}
			}

			laylvl->addWaveToQueue(wav);

			return laylvl;
		}

		//Test size level
		static Level::LevelI* testSize()
		{
			PlayerDef play(basePlayer());
			play.weapon = "fungun";
			play.weaponLevel = 42;
			play.bombTime = 5000;
			play.bombRadius = 15.f;

			Level::WaveQueue* sizelvl = new Level::WaveQueue("testsize", play);
			Wave wav = Wave();
			float radius = 32;
			b2Vec2 pos(0, 0);
			EnemyDef def;

			sizelvl->addAFX("../assets/wind.ogg", 0, 1, 1500, 2000);
			sizelvl->setPrimary(b2Color(0.1f, 0.3f, 0.8f));
			sizelvl->setSecondary(b2Color(0.2f, 0.7f, 7.f));
			sizelvl->setTertiary(b2Color(0.7f, 0.5f, 0.1f));
			sizelvl->setRespiteTimeMAX(1000);
			sizelvl->setBoundsRadius(radius);
			sizelvl->setBoundsPoints(16);

			for (int i = 1; i < 7; ++i)
			{
				for (int j = 1; j < 7; ++j)
				{
					b2Vec2 pos(-17.5 + j * 5, 2 - i * pow(1.25, i));
					EnemyDef e(ShapeDef(pos, b2Vec2_zero + pos, i + 2));
					e.size = 0.5 * i;
					e.hpScale = 5 * (j - 1);
					e.colPrim = b2Color(0.1 * i, 0.05 * j, 0.4);
					e.colSecn = b2Color(0.7, 0.4 + (0.05 * i), 0.9 - (0.1 * j));
					e.colTert = b2Color(1 - (0.05 * (i + j)), 0.7, 0.025 * (i * j));
					e.ai = 2;
					e.faction = 2;

					wav.addEnemy(e);
				}

				sizelvl->addWaveToQueue(wav);
			}

			return sizelvl;
		}

		//Test sloop level
		static Level::LevelI* testSurv()
		{
			PlayerDef play(basePlayer());
			play.weapon = "shotgun";
			play.weaponLevel = 8;
			play.bombTime = 2500;
			play.bombRadius = 20.f;
			play.size = 2.f;

			Level::Survival* survlvl = new Level::Survival("testsurv", play);
			Wave wav = Wave();
			float radius = 32;
			b2Vec2 pos(0, 0);
			EnemyDef def;

			survlvl->addAFX("../assets/wind.ogg", 0, 1, 1500, 2000);
			survlvl->setPrimary(b2Color(0.01f, 0.02f, 0.1f));
			survlvl->setSecondary(b2Color(0.2f, 0.1f, 0.5f));
			survlvl->setTertiary(b2Color(0.7f, 0.7f, 0.1f));
			survlvl->setRespiteTimeMAX(5000);
			survlvl->setBoundsRadius(radius);
			survlvl->setBoundsPoints(32);

			survlvl->setSurvivalTime(120);
			survlvl->setWaveSizeMod(1.f);
			survlvl->setWaveSizeMAX(10.f);

			EnemyDef mod = EnemyDef::modDef();
			mod.vertices = 1;
			mod.verticesMax = 1;
			mod.verticesMin = 1;
			mod.size = 0.25f;

			survlvl->setMod(mod);

			EnemyDef modMax = EnemyDef::maxDef();
			modMax.vertices = 5;
			modMax.verticesMax = 5;
			modMax.verticesMin = 5;
			modMax.size = 1.5f;

			survlvl->setModMAX(modMax);

			for (int i = 1; i < 6; ++i)
			{
				wav = Wave();
				for (int j = i; j > 0; --j)
				{
					b2Vec2 pos(0, 2.5f * j);
					EnemyDef e(ShapeDef(pos, b2Vec2_zero + pos, j + 2));
					e.size = 0.5;
					e.hpScale = 1;
					e.colPrim = b2Color(0.1 * i, 0.05 * i * i, 0.4 * i);
					e.colSecn = b2Color(0.7, 0.4 + (0.15 * i), 0.9 - (0.1 * i));
					e.colTert = b2Color(1 - (0.05 * (i + i)), 0.7, 0.025 * (i * i));
					e.ai = 2;
					e.faction = 2;

					wav.addEnemy(e);
				}
				survlvl->addPaletteWave(wav);
			}

			return survlvl;
		}

		//Test AI level
		static Level::LevelI* testAI()
		{
			PlayerDef play(basePlayer());
			play.weapon = "shotgun";
			play.weaponLevel = 4;
			play.bombTime = 5000;
			play.bombRadius = 20.f;
			play.size = 1.f;

			Level::Survival* survlvl = new Level::Survival("testAI", play);
			Wave wav = Wave();
			float radius = 32;
			b2Vec2 pos(0, 0);
			EnemyDef def;

			survlvl->addAFX("../assets/wind.ogg", 0, 1, 1500, 2000);
			survlvl->setPrimary(b2Color(1.f, 0.3f, 0.f));
			survlvl->setSecondary(b2Color(0.7f, 0.1f, 0.5f));
			survlvl->setTertiary(b2Color(0.1f, 0.3f, 0.1f));
			survlvl->setRespiteTimeMAX(15000);
			survlvl->setBoundsRadius(radius);
			survlvl->setBoundsPoints(32);

			survlvl->setSurvivalTime(120);
			survlvl->setWaveSizeMod(1.f);
			survlvl->setWaveSizeMAX(10.f);

			EnemyDef mod = EnemyDef::modDef();
			mod.vertices = 1;
			mod.verticesMax = 1;
			mod.verticesMin = 1;
			mod.hpScale = 1;
			mod.size = 0.25f;
			mod.weaponLevel = 1;

			survlvl->setMod(mod);

			EnemyDef modMax = EnemyDef::maxDef();
			modMax.vertices = 5;
			modMax.verticesMax = 8;
			modMax.verticesMin = 8;
			modMax.hpScale = 15;
			modMax.size = 1.5f;
			modMax.weaponLevel = 4;

			survlvl->setModMAX(modMax);

			for (int i = 1; i < 6; ++i)
			{
				wav = Wave();
				pos = b2Vec2(0, radius * 0.75f);
				EnemyDef e(ShapeDef(pos, - pos, i + 2));
				e.size = 0.75;
				e.hpScale = 5;
				e.speedScale = 0.5f;
				e.colPrim = b2Color(0.1 * i, 0.05 * i * i, 0.4 * i);
				e.colSecn = b2Color(0.7, 0.4 + (0.15 * i), 0.9 - (0.1 * i));
				e.colTert = b2Color(1 - (0.05 * (i + i)), 0.7, 0.025 * (i * i));
				e.ai = 2;
				e.faction = 2;
				e.weaponLevel = 1;

				switch (i)
				{
				case 1:	e.weapon = "pistol";	break;
				case 2: e.weapon = "rifle";		break;
				case 3: e.weapon = "coilgun";	break;
				case 4: e.weapon = "cannon";	break;
				case 5:	e.weapon = "railgun";	break;
				}

				wav.addEnemy(e, 6 - i);
				survlvl->addPaletteWave(wav);
			}

			return survlvl;
		}

		//Test completion level
		static Level::LevelI* testComplete()
		{

			PlayerDef play(basePlayer());
			play.weapon = "rifle";
			play.weaponLevel = 4;
			play.bombTime = 2500;
			play.bombRadius = 20.f;

			Level::WaveQueue* testlvl = new Level::WaveQueue("testcomp", play);
			Wave wav = Wave();
			float radius = 32;
			b2Vec2 pos(0, 4.f);
			EnemyDef def;

			testlvl->addAFX("../assets/wind.ogg", 0, 1, 1500, 2000);
			testlvl->setPrimary(b2Color(1.0f, 0.25f, 0.5f));
			testlvl->setSecondary(b2Color(0.25f, 0.15f, 1.f));
			testlvl->setTertiary(b2Color(0.3f, 0.3f, 0.6f));
			testlvl->setRespiteTimeMAX(5000);
			testlvl->setBoundsRadius(radius);
			testlvl->setBoundsPoints(8);

			for (int i = 1; i < 3; ++i)
			{
				wav = Wave();
				EnemyDef e(ShapeDef(pos, b2Vec2_zero + pos, i + 2));
				e.size = 1.5f;
				e.hpScale = 5;
				e.colPrim = b2Color(0.1 * i, 0.05 * i * i, 0.4 * i);
				e.colSecn = b2Color(0.7, 0.4 + (0.15 * i), 0.9 - (0.1 * i));
				e.colTert = b2Color(1 - (0.05 * (i + i)), 0.7, 0.025 * (i * i));
				e.ai = 2;
				e.faction = 2;

				wav.addEnemy(e, 4);

				testlvl->addWaveToQueue(wav);
			}

			return testlvl;
		}

		//Test steering level
		static Level::LevelI* testSteer()
		{
			PlayerDef play(basePlayer());
			play.weapon = "coilgun";
			play.weaponLevel = 5;
			play.bombRadius = 32;
			play.bombTime = 1000;

			Level::Survival* steerlvl = new Level::Survival("teststeer", play);
			
			Wave wav = Wave();
			float radius = 32;

			steerlvl->addAFX("../assets/wind.ogg", 0, 1, 1500, 2000);
			steerlvl->setPrimary(b2Color(0.5f, 0.05f, 0.1f));
			steerlvl->setSecondary(b2Color(0.5f, 0.25f, 0.35f));
			steerlvl->setTertiary(b2Color(0.0f, 0.0f, 0.0f));
			steerlvl->setBoundsRadius(radius);
			steerlvl->setBoundsPoints(12);

			steerlvl->setSurvivalTime(-1);
			steerlvl->setRespiteTimeMAX(-1);

			EnemyDef e(ShapeDef(b2Vec2_zero, b2Vec2_zero, 3));
			e.damageScale = 0;
			e.ai = 3;
			e.faction = 2;
			e.colPrim = b2Color(1.f, 0.f, 0.f);
			e.colSecn = b2Color(1.f, 1.f, 0.f);
			e.colTert = b2Color(0, 0, 0);
			e.hpScale = 5;
			e.speedScale = 0.5;
			e.weapon = "pistol";
			e.weaponLevel = 0;

			wav.addEnemy(e);

			steerlvl->addPaletteWave(wav);
			return steerlvl;
		}
	}//end atlas
}//end level
#endif
