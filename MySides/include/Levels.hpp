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
				enem.position *= radius * 0.4f;
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
			play.weapon = "pistol";
			play.weaponLevel = 5;
			play.bombRadius = 64;
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

			EnemyDef e(ShapeDef(b2Vec2_zero, b2Vec2_zero, 5));
			e.damageScale = 0;
			e.ai = 5;
			e.faction = 2;
			e.colPrim = b2Color(1.f, 0.f, 0.f);
			e.colSecn = b2Color(1.f, 1.f, 0.f);
			e.colTert = b2Color(0, 0, 0);
			e.hpScale = 5;
			e.speedScale = 0.6f;
			e.weapon = "pistol";
			e.weaponLevel = 0;
			e.brain = AIDef::testDef();

			wav.addEnemy(e);

			steerlvl->addPaletteWave(wav);
			return steerlvl;
		}
		
		//Survival Rifle
		static Level::LevelI* survRif()
		{
			PlayerDef play(basePlayer());
			play.weapon = "rifle";
			play.weaponLevel = 0;
			play.bombRadius = 24;
			play.bombTime = 15000;
			play.colPrim = b2Color(0.2f, 0.2f, 0);
			play.colSecn = b2Color(0.4f, 0.2f, 0);
			play.colTert = b2Color(0.8f, 0.2f, 0);

			Level::Survival* rifl = new Level::Survival("Rifle", play);

			float radius = 64;

			rifl->addAFX("../assets/wind.ogg", 0, 1, 2000, 2100);
			rifl->addAFX("../assets/spriterip.ogg", 1, 0, 820, 2100);
			rifl->setPrimary(b2Color(.75f, .75f, 0));
			rifl->setSecondary(b2Color(.5f, .5f, 0));
			rifl->setTertiary(b2Color(0, 0, 0));
			rifl->setBoundsRadius(radius);
			rifl->setBoundsPoints(12);

			rifl->setSurvivalTime(120);
			rifl->setRespiteTimeMAX(10000);

			rifl->setWaveSizeMod(1.f);
			rifl->setWaveSizeMAX(10.f);

			EnemyDef mod = EnemyDef::modDef();
			mod.vertices = 1;
			mod.verticesMax = 1;
			mod.verticesMin = 1;
			mod.size = 0.25f;

			rifl->setMod(mod);

			EnemyDef modMax = EnemyDef::maxDef();
			modMax.vertices = 5;
			modMax.verticesMax = 5;
			modMax.verticesMin = 5;
			modMax.size = 1.f;

			rifl->setModMAX(modMax);
			
			Wave wav = Wave();
			for (int i = 0; i < 5; ++i)
			{
				wav = Wave();
				b2Vec2 pos = b2Vec2(randFloat(-1, 1), randFloat(-1, 1));
				pos.Normalize();
				pos *= randFloat(32, 48);

				EnemyDef e(ShapeDef(pos, b2Vec2_zero, fmax(i + 3, 8)));
				e.damageScale = 0.5f;
				e.ai = 5;
				e.size = 0.75f;
				e.faction = 2;
				e.colPrim = b2Color(0.5f, 0.f, 0.f);
				e.colSecn = b2Color(0.f, 0.f, 0.f);
				e.colTert = b2Color(0.5f, 0.5f, 0);
				e.hpScale = 5;
				e.speedScale = 0.6f;
				e.brain = AIDef::testDef();
				e.brain.dancer = randFloat(0, 4);
				e.brain.CCW = randFloat(0, 10) > 9;


				switch (i)
				{
				case 0:	e.weapon = "pistol";	break;
				case 1: e.weapon = "rifle";		break;
				case 2: e.weapon = "shotgun";	break;
				case 3: e.weapon = "cannon";	break;
				case 4:	e.weapon = "thumper";	break;
				}

				e.weaponLevel = 0;

				wav.addEnemy(e, (i + 1) * 2);

				rifl->addPaletteWave(wav);
			}

			return rifl;
		}

		//Survival Shotgun
		static Level::LevelI* survShot()
		{
			PlayerDef play(basePlayer());
			play.weapon = "shotgun";
			play.weaponLevel = 0;
			play.bombRadius = 16;
			play.bombTime = 10000;
			play.colPrim = b2Color(0.8f, 0.8f, 0);
			play.colSecn = b2Color(1.f, 0.f, 0);
			play.colTert = b2Color(1.f, 1.f, 1.f);
			play.size = 0.75f;
			play.speedScale = 1.5f;

			Level::Survival* shot = new Level::Survival("Shotgun", play);

			float radius = 64;

			shot->addAFX("../assets/wind.ogg", 0, 1, 2000, 2100);
			shot->addAFX("../assets/spriterip.ogg", 1, 0, 820, 2100);
			shot->setPrimary(b2Color(.75f, .75f, .75f));
			shot->setSecondary(b2Color(.5f, .5f, .5f));
			shot->setTertiary(b2Color(.75f, .75f, .75f));
			shot->setBoundsRadius(radius);
			shot->setBoundsPoints(12);

			shot->setSurvivalTime(120);
			shot->setRespiteTimeMAX(10000);

			shot->setWaveSizeMod(1.f);
			shot->setWaveSizeMAX(15.f);

			EnemyDef mod = EnemyDef::modDef();
			mod.vertices = 1;
			mod.verticesMax = 1;
			mod.verticesMin = 1;
			mod.size = 0.25f;

			shot->setMod(mod);

			EnemyDef modMax = EnemyDef::maxDef();
			modMax.vertices = 5;
			modMax.verticesMax = 5;
			modMax.verticesMin = 5;
			modMax.size = 0.75f;

			shot->setModMAX(modMax);

			Wave wav = Wave();
			for (int i = 0; i < 5; ++i)
			{
				wav = Wave();
				b2Vec2 pos = b2Vec2(randFloat(-1, 1), randFloat(-1, 1));
				pos.Normalize();
				pos *= randFloat(32, 48);

				EnemyDef e(ShapeDef(pos, b2Vec2_zero, fmax(i + 3, 8)));
				e.damageScale = 0.5f;
				e.ai = 5;
				e.size = 0.5f;
				e.faction = 2;
				e.colPrim = b2Color(0.f, 0.f, 0.f);
				e.colSecn = b2Color(0.f, 0.1f, 0.f);
				e.colTert = b2Color(0, 0, 1.f);
				e.hpScale = 1;
				e.speedScale = 0.4f;
				e.brain = AIDef::testDef();
				e.brain.dancer = randFloat(0, 4);
				e.brain.CCW = randFloat(0, 10) > 9;


				switch (i)
				{
				case 0:	e.weapon = "pistol";	break;
				case 1: e.weapon = "rifle";		break;
				case 2: e.weapon = "thumper";	break;
				case 3: e.weapon = "cannon";	break;
				case 4:	e.weapon = "launcher";	break;
				}

				e.weaponLevel = 0;

				wav.addEnemy(e, (i + 1) * 4);

				shot->addPaletteWave(wav);
			}

			return shot;
		}

		//Survival Coilgun
		static Level::LevelI* survCoil()
		{
			PlayerDef play(basePlayer());
			play.weapon = "coilgun";
			play.weaponLevel = 0;
			play.bombRadius = 24;
			play.bombTime = 10000;
			play.colPrim = b2Color(0.1f, 0.8f, 0.8f);
			play.colSecn = b2Color(1.f, 1.f, 1.f);
			play.colTert = b2Color(1.f, 0.f, 0.f);

			Level::Survival* coil = new Level::Survival("Coilgun", play);

			float radius = 64;

			coil->addAFX("../assets/wind.ogg", 0, 1, 2000, 2100);
			coil->addAFX("../assets/spriterip.ogg", 1, 0, 820, 2100);
			coil->setPrimary(b2Color(0, 0, 1));
			coil->setSecondary(b2Color(0, 0, .4f));
			coil->setTertiary(b2Color(1, 1, 0));
			coil->setBoundsRadius(radius);
			coil->setBoundsPoints(12);

			coil->setSurvivalTime(120);
			coil->setRespiteTimeMAX(10000);

			coil->setWaveSizeMod(1.f);
			coil->setWaveSizeMAX(2.f);

			EnemyDef mod = EnemyDef::modDef();
			mod.vertices = 1;
			mod.verticesMax = 1;
			mod.verticesMin = 1;
			mod.size = 0.25f;

			coil->setMod(mod);

			EnemyDef modMax = EnemyDef::maxDef();
			modMax.vertices = 5;
			modMax.verticesMax = 5;
			modMax.verticesMin = 5;
			modMax.size = 1.75f;

			coil->setModMAX(modMax);

			Wave wav = Wave();
			for (int i = 0; i < 5; ++i)
			{
				wav = Wave();
				b2Vec2 pos = b2Vec2(randFloat(-1, 1), randFloat(-1, 1));
				pos.Normalize();
				pos *= randFloat(32, 48);

				EnemyDef e(ShapeDef(pos, b2Vec2_zero, fmax(i + 3, 8)));
				e.damageScale = 0.5f;
				e.ai = 5;
				e.faction = 2;
				e.size = 0.75f;
				e.colPrim = b2Color(0.5f, 0.6f, 0.f);
				e.colSecn = b2Color(1.f, 1.f, 0.f);
				e.colTert = b2Color(0, 0.1, 0);
				e.hpScale = 5;
				e.speedScale = 0.7f;
				e.brain = AIDef::testDef();
				e.brain.dancer = randFloat(0, 4);
				e.brain.CCW = randFloat(0, 10) > 9;

				switch (i)
				{
				case 0:	e.weapon = "pistol";	break;
				case 1: e.weapon = "cannon";	break;
				case 2: e.weapon = "rifle";		break;
				case 3: e.weapon = "thumper";	break;
				case 4:	e.weapon = "railgun";	break;
				}

				e.weaponLevel = 0;

				wav.addEnemy(e, (i + 1) * 4);

				coil->addPaletteWave(wav);
			}

			return coil;
		}

		//Survival Cannon
		static Level::LevelI* survCann() 
		{
			PlayerDef play(basePlayer());
			play.weapon = "cannon";
			play.weaponLevel = 0;
			play.bombRadius = 24;
			play.bombTime = 15000;
			play.colPrim = b2Color(0.3f, 0.3f, 0.3f);
			play.colSecn = b2Color(0.6f, 0.6f, 0.6f);
			play.colTert = b2Color(1.f, 0.5f, 0.f);
			play.speedScale = 0.75f;
			play.size = 1.25f;

			Level::Survival* cann = new Level::Survival("Cannon", play);

			float radius = 64;

			cann->addAFX("../assets/wind.ogg", 0, 1, 2000, 2100);
			cann->addAFX("../assets/spriterip.ogg", 1, 0, 820, 2100);
			cann->setPrimary(b2Color(0.2f, 0.2f, 0.2f));
			cann->setSecondary(b2Color(.1f, .1f, .1f));
			cann->setTertiary(b2Color(0.2f, 0.2f, 0.2f));
			cann->setBoundsRadius(radius);
			cann->setBoundsPoints(12);

			cann->setSurvivalTime(120);
			cann->setRespiteTimeMAX(10000);

			cann->setWaveSizeMod(1.f);
			cann->setWaveSizeMAX(50.f);

			EnemyDef mod = EnemyDef::modDef();
			mod.vertices = 1;
			mod.verticesMax = 1;
			mod.verticesMin = 1;
			mod.size = 0.25f;

			cann->setMod(mod);

			EnemyDef modMax = EnemyDef::maxDef();
			modMax.vertices = 5;
			modMax.verticesMax = 8;
			modMax.verticesMin = 5;
			modMax.size = 0.5f;

			cann->setModMAX(modMax);

			Wave wav = Wave();
			for (int i = 0; i < 5; ++i)
			{
				wav = Wave();
				b2Vec2 pos = b2Vec2(randFloat(-1, 1), randFloat(-1, 1));
				pos.Normalize();
				pos *= randFloat(32, 48);

				EnemyDef e(ShapeDef(pos, b2Vec2_zero, fmax(i + 3, 8)));
				e.damageScale = 0.25f;
				e.ai = 5;
				e.faction = 2;
				e.size = 0.25f;
				e.colPrim = b2Color(0.5f, 0.6f, 0.f);
				e.colSecn = b2Color(0.9f, 0.1f, 0.f);
				e.colTert = b2Color(1.f, 1.f, 0);
				e.hpScale = 1;
				e.speedScale = 0.6f;
				e.brain = AIDef::testDef();
				e.brain.dancer = randFloat(0, 4);
				e.brain.CCW = randFloat(0, 10) > 9;


				switch (i)
				{
				case 0:	e.weapon = "pistol";	break;
				case 1: e.weapon = "rifle";	break;
				case 2: e.weapon = "shotgun";	break;
				case 3: e.weapon = "rifle";	break;
				case 4:	e.weapon = "pistol";	break;
				}

				e.weaponLevel = 0;

				wav.addEnemy(e, (i + 1) * 10);

				cann->addPaletteWave(wav);
			}

			return cann;
		}

		//Survival Thumper
		static Level::LevelI* survThum()
		{
			PlayerDef play(basePlayer());
			play.weapon = "thumper";
			play.weaponLevel = 0;
			play.bombRadius = 24;
			play.bombTime = 15000;
			play.colPrim = b2Color(0.f, 0.2f, 0.f);
			play.colSecn = b2Color(1.f, 1.f, 1.f);
			play.colTert = b2Color(1.f, 1.f, 0.f);

			Level::Survival* thum = new Level::Survival("Thumper", play);

			float radius = 64;

			thum->addAFX("../assets/wind.ogg", 0, 1, 2000, 2100);
			thum->addAFX("../assets/spriterip.ogg", 1, 0, 820, 2100);
			thum->setPrimary(b2Color(0, 0.75f, 0));
			thum->setSecondary(b2Color(0, 0.5f, 0));
			thum->setTertiary(b2Color(0, 1.f, 1.f));
			thum->setBoundsRadius(radius);
			thum->setBoundsPoints(12);

			thum->setSurvivalTime(120);
			thum->setRespiteTimeMAX(10000);

			thum->setWaveSizeMod(1.f);
			thum->setWaveSizeMAX(20.f);

			EnemyDef mod = EnemyDef::modDef();
			mod.vertices = 1;
			mod.verticesMax = 1;
			mod.verticesMin = 1;
			mod.size = 0.1f;

			thum->setMod(mod);

			EnemyDef modMax = EnemyDef::maxDef();
			modMax.vertices = 5;
			modMax.verticesMax = 5;
			modMax.verticesMin = 5;
			modMax.size = 0.8f;

			thum->setModMAX(modMax);

			Wave wav = Wave();
			for (int i = 0; i < 5; ++i)
			{
				wav = Wave();
				b2Vec2 pos = b2Vec2(randFloat(-1, 1), randFloat(-1, 1));
				pos.Normalize();
				pos *= randFloat(32, 48);

				EnemyDef e(ShapeDef(pos, b2Vec2_zero, fmax(i + 3, 8)));
				e.damageScale = 0.25f;
				e.size = 0.4f;
				e.ai = 5;
				e.faction = 2;
				e.colPrim = b2Color(0.2f, 0.1f, 0.4f);
				e.colSecn = b2Color(0.1f, 0.2f, 0.f);
				e.colTert = b2Color(1.f, 0.f, 0.5f);
				e.hpScale = 5;
				e.speedScale = 0.6f;
				e.brain = AIDef::testDef();
				e.brain.dancer = randFloat(0, 4);
				e.brain.CCW = randFloat(0, 10) > 9;


				switch (i)
				{
				case 0:	e.weapon = "shotgun";	break;
				case 1: e.weapon = "coilgun";	break;
				case 2: e.weapon = "railgun";	break;
				case 3: e.weapon = "pistol";	break;
				case 4:	e.weapon = "werfer";	break;
				}

				e.weaponLevel = 2;

				wav.addEnemy(e, (i + 1) * 2);

				thum->addPaletteWave(wav);
			}

			return thum;
		}

		//Survival Launcher
		static Level::LevelI* survLaun()
		{
			PlayerDef play(basePlayer());
			play.weapon = "launcher";
			play.weaponLevel = 0;
			play.bombRadius = 24;
			play.bombTime = 15000;
			play.colPrim = b2Color(1.f, 0.2f, 0.f);
			play.colSecn = b2Color(0.f, 0.f, 0.f);
			play.colTert = b2Color(0.9f, 0.6f, 0.12f);

			Level::Survival* laun = new Level::Survival("Launcher", play);

			float radius = 64;

			laun->addAFX("../assets/wind.ogg", 0, 1, 2000, 2100);
			laun->addAFX("../assets/spriterip.ogg", 1, 0, 820, 2100);
			laun->setPrimary(b2Color(0.7f, 0, 0));
			laun->setSecondary(b2Color(0.3f, 0, 0));
			laun->setTertiary(b2Color(0.9f, 0, 0));
			laun->setBoundsRadius(radius);
			laun->setBoundsPoints(12);

			laun->setSurvivalTime(120);
			laun->setRespiteTimeMAX(10000);

			laun->setWaveSizeMod(1.f);
			laun->setWaveSizeMAX(20.f);

			EnemyDef mod = EnemyDef::modDef();
			mod.vertices = 1;
			mod.verticesMax = 1;
			mod.verticesMin = 1;
			mod.size = 0.25f;

			laun->setMod(mod);

			EnemyDef modMax = EnemyDef::maxDef();
			modMax.vertices = 5;
			modMax.verticesMax = 5;
			modMax.verticesMin = 5;
			modMax.size = 1.5f;

			laun->setModMAX(modMax);

			Wave wav = Wave();
			for (int i = 0; i < 5; ++i)
			{
				wav = Wave();
				b2Vec2 pos = b2Vec2(randFloat(-1, 1), randFloat(-1, 1));
				pos.Normalize();
				pos *= randFloat(32, 48);

				EnemyDef e(ShapeDef(pos, b2Vec2_zero, fmax(i + 3, 8)));
				e.damageScale = 0.5f;
				e.size = 0.75f;
				e.ai = 5;
				e.faction = 2;
				e.colPrim = b2Color(0.46f, 0.9f, 0.1f);
				e.colSecn = b2Color(0.f, 1.f, 0.f);
				e.colTert = b2Color(1.f, 0.4f, 1.f);
				e.hpScale = 5;
				e.speedScale = 0.6f;
				e.brain = AIDef::testDef();
				e.brain.dancer = randFloat(0, 4);
				e.brain.CCW = randFloat(0, 10) > 9;


				switch (i)
				{
				case 0:	e.weapon = "rifle";	break;
				case 1: e.weapon = "werfer";	break;
				case 2: e.weapon = "railgun";	break;
				case 3: e.weapon = "cannon";	break;
				case 4:	e.weapon = "shotgun";	break;
				}

				e.weaponLevel = 2;

				wav.addEnemy(e, i + 1);

				laun->addPaletteWave(wav);
			}

			return laun;
		}

		//Survival Launcher
		static Level::LevelI* survWerf()
		{
			PlayerDef play(basePlayer());
			play.weapon = "werfer";
			play.weaponLevel = 0;
			play.bombRadius = 8;
			play.bombTime = 10000;
			play.colPrim = b2Color(0.3f, 0.2f, 0.f);
			play.colSecn = b2Color(0.1f, 0.f, 0.3f);
			play.colTert = b2Color(0.25f, 1.f, 0.12f);
			play.speedScale = 0.75f;
			play.size = 1.5f;

			Level::Survival* laun = new Level::Survival("Werfer", play);

			float radius = 64;

			laun->addAFX("../assets/wind.ogg", 0, 1, 2000, 2100);
			laun->addAFX("../assets/spriterip.ogg", 1, 0, 820, 2100);
			laun->setPrimary(b2Color(0.8f, 0.5f, 0.1f));
			laun->setSecondary(b2Color(0.7f, 0.8f, 0.1f));
			laun->setTertiary(b2Color(0.2f, 0, 0));
			laun->setBoundsRadius(radius);
			laun->setBoundsPoints(12);

			laun->setSurvivalTime(120);
			laun->setRespiteTimeMAX(10000);

			laun->setWaveSizeMod(1.f);
			laun->setWaveSizeMAX(20.f);

			EnemyDef mod = EnemyDef::modDef();
			mod.vertices = 1;
			mod.verticesMax = 1;
			mod.verticesMin = 1;
			mod.size = 0.25f;

			laun->setMod(mod);

			EnemyDef modMax = EnemyDef::maxDef();
			modMax.vertices = 5;
			modMax.verticesMax = 5;
			modMax.verticesMin = 5;
			modMax.size = 1.5f;

			laun->setModMAX(modMax);

			Wave wav = Wave();
			for (int i = 0; i < 5; ++i)
			{
				wav = Wave();
				b2Vec2 pos = b2Vec2(randFloat(-1, 1), randFloat(-1, 1));
				pos.Normalize();
				pos *= randFloat(32, 48);

				EnemyDef e(ShapeDef(pos, b2Vec2_zero, randFloat(3, 8)));
				e.damageScale = 0.5f;
				e.ai = 5;
				e.faction = 2;
				e.colPrim = b2Color(0.2f, 0.2f, 0.2f);
				e.colSecn = b2Color(0.4f, 0.4f, 0.4f);
				e.colTert = b2Color(0.7f, 0.5f, 0.5f);
				e.hpScale = 5;
				e.speedScale = 0.6f;
				e.brain = AIDef::testDef();
				e.brain.dancer = randFloat(0, 4);
				e.brain.CCW = randFloat(0, 10) > 9;


				switch (i)
				{
				case 0:	e.weapon = "coilgun";	break;
				case 1: e.weapon = "railgun";	break;
				case 2: e.weapon = "shotgun";	break;
				case 3: e.weapon = "cannon";	break;
				case 4:	e.weapon = "rifle";		break;
				}

				e.weaponLevel = 2;

				wav.addEnemy(e, (i + 1) * 3);

				laun->addPaletteWave(wav);
			}

			return laun;
		}

	}//end atlas
}//end level
#endif
