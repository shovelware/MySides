#include "Armory.hpp"

Weapon::Armory::Armory(std::function<void(std::vector<ProjectileDef>&defs, std::string id)> fireCallback) :
	fireCallback_(fireCallback)
{
}

Weapon::WeaponI* Weapon::Armory::requisition(std::string name, int code)
{
	//Messy, but functional :^)
	if (name == "pistol")			return getPistol(code);
	else if (name == "rifle")		return getRifle(code);
	else if (name == "cannon")		return getCannon(code);
	else if (name == "shotgun")		return getShotgun(code);
	else if (name == "werfer")		return getWerfer(code);
	else if (name == "coilgun")		return getCoilgun(code);
	else if (name == "railgun")		return getRailgun(code);
	else if (name == "thumper")		return getThumper(code);
	else if (name == "launcher")	return getLauncher(code);
	else if (name == "fungun")		return getFun(code);

	else return nullptr;
}

//Upgrade order: HARDER, BETTER, FASTER, STRONGER

//Magazine weapons
Weapon::WeaponI * Weapon::Armory::getPistol(int level, int projLevel)
{
	//Min level 0
	int weaplv = std::max(0, std::min(level, 8));
	//If -1, match weapon level (default behaviour)
	int projlv = (projLevel > -1 ? projLevel : level);

	Weapon::SemiMag* pistol = new Weapon::SemiMag(fireCallback_, getNinMil(projlv), "pistol");
	upgradePistol(pistol, weaplv, projlv);
	return static_cast<Weapon::WeaponI*>(pistol);
}

Weapon::WeaponI * Weapon::Armory::getRifle(int level, int projLevel)
{
	//Min level 0
	int weaplv = std::max(0, std::min(level, 8));
	//If -1, match weapon level (default behaviour)
	int projlv = (projLevel > -1 ? projLevel : level);

	Weapon::AutoMag* rifle = new Weapon::AutoMag(fireCallback_, getBullet(projlv), "rifle");
	upgradeRifle(rifle, weaplv, projlv);
	return static_cast<Weapon::WeaponI*>(rifle);
}

Weapon::WeaponI * Weapon::Armory::getCannon(int level, int projLevel)
{
	//Min level 0
	int weaplv = std::max(0, std::min(level, 8));
	//If -1, match weapon level (default behaviour)
	int projlv = (projLevel > -1 ? projLevel : level);

	Weapon::SemiMag* cannon = new Weapon::SemiMag(fireCallback_, getCannonball(projlv), "cannon");
	upgradeCannon(cannon, weaplv, projlv);
	return static_cast<Weapon::WeaponI*>(cannon);
}

Weapon::WeaponI* Weapon::Armory::getShotgun(int level, int projLevel)
{
	//Min level 0
	int weaplv = std::max(0, std::min(level, 8));
	//If -1, match weapon level (default behaviour)
	int projlv = (projLevel > -1 ? projLevel : level);

	Weapon::SpreadMag* shotgun = new Weapon::SpreadMag(fireCallback_, getPellet(projlv), "shotgun");
	upgradeShotgun(shotgun, weaplv, projlv);
	return static_cast<Weapon::WeaponI*>(shotgun);
}

//Battery Weapons
Weapon::WeaponI* Weapon::Armory::getWerfer(int level, int projLevel)
{
	//Min level 0
	int weaplv = std::max(0, std::min(level, 8));
	//If -1, match weapon level (default behaviour)
	int projlv = (projLevel > -1 ? projLevel : level);

	Weapon::SpreadBat* werfer = new Weapon::SpreadBat(fireCallback_, getFlammen(projlv), "werfer");
	upgradeWerfer(werfer, weaplv, projlv);
	return static_cast<Weapon::WeaponI*>(werfer);
}

Weapon::WeaponI* Weapon::Armory::getCoilgun(int level, int projLevel)
{
	//Min level 0
	int weaplv = std::max(0, std::min(level, 8));
	//If -1, match weapon level (default behaviour)
	int projlv = (projLevel > -1 ? projLevel : level);

	Weapon::AutoBat* coilgun = new Weapon::AutoBat(fireCallback_, getLaser(projlv), "coilgun");
	upgradeCoilgun(coilgun, weaplv, projlv);
	return static_cast<Weapon::WeaponI*>(coilgun);
}

Weapon::WeaponI* Weapon::Armory::getRailgun(int level, int projLevel)
{
	//Min level 0
	int weaplv = std::max(0, std::min(level, 8));
	//If -1, match weapon level (default behaviour)
	int projlv = (projLevel > -1 ? projLevel : level);

	Weapon::SemiBat* railgun = new Weapon::SemiBat(fireCallback_, getSlug(projlv), "railgun");
	upgradeRailgun(railgun, weaplv, projLevel);
	return static_cast<Weapon::WeaponI*>(railgun);
}

//Explosive Weapons
Weapon::WeaponI* Weapon::Armory::getThumper(int level, int projLevel)
{
	return nullptr;
}

Weapon::WeaponI* Weapon::Armory::getLauncher(int level, int projLevel)
{
	return nullptr;
}

//Fun weapons
Weapon::WeaponI* Weapon::Armory::getFun(int type)
{
	Weapon::WeaponI * funGun = nullptr;

	switch (type)
	{
		// Explosive Buckshot
	case 0:
	{
		ProjectileDef gren = ProjectileDef();
		gren.bounce = 0.f;
		gren.hpMAX = 1;
		gren.damage = 10;
		//gren.oneHit = true;
		gren.force.first = 0.25f;
		gren.force.second = 2.f;
		gren.shrapnel.first = 8;
		gren.shrapnel.second = 4;
		gren.width = 1.0f;
		gren.lifeTime = 500;

		funGun = new Weapon::SpreadMag(fireCallback_, gren, "thumper", 16, 50, 500, 8, 0.3f);
		break;
	}
	
		//Lance Beam
	case 42:
	{
		ProjectileDef laser = getLaser();
		break;
	}
		//Black Hole Gun
	case 8:
	{
		break;
	}
		//Broad Beam
	case 1:
	{
		break;
		}
		//Flak cannon
	case 47:
	{
		ProjectileDef shrap = getShrapnel(8);
		shrap.lifeTime = 150;
		shrap.oneHit = true;
		shrap.height = shrap.width * 4;
		shrap.width *= 2;
		shrap.shrapnel.first = 4;
		shrap.shrapnel.second = 8;
		shrap.force.first = -0.5f;
		shrap.force.second = 1.f;

		funGun = new Weapon::SpreadMag(fireCallback_, shrap, "shotgun", 4, 0, 300, 8, 0.5f);

		break;
	}

		//Chaingun
	case 88:
	{
		ProjectileDef bb = getShrapnel(8);
		bb.lifeTime = 200;
		bb.oneHit = true;
		bb.bounce = 0.f;
		bb.force.first = 0.025f;
		bb.force.second = 0.5f;

		funGun = new Weapon::SpreadBat(fireCallback_, bb, "pistol", 2000, 32, 1000, 10, 8, 0, 0.5, 1.1f);

		break;
	}

	}//End switch

	return funGun;
}

void Weapon::Armory::setWeaponLevel(WeaponI * weapon, int level, int projLevel)
{
	if (weapon != nullptr && weapon->getLevel() != level)
	{
		std::string id = weapon->getID();
		int lv = std::max(0, std::min(level, 8));
		weapon->setLevel(lv);
		weapon->reup(true);

		if (id == "pistol")			upgradePistol(static_cast<Weapon::SemiMag*>(weapon), lv, projLevel);
		else if (id == "rifle")		upgradeRifle(static_cast<Weapon::AutoMag*>(weapon), lv, projLevel);
		else if (id == "cannon")	upgradeCannon(static_cast<Weapon::SemiMag*>(weapon), lv, projLevel);
		else if (id == "shotgun")	upgradeShotgun(static_cast<Weapon::SpreadMag*>(weapon), lv, projLevel);
		else if (id == "werfer")	upgradeWerfer(static_cast<Weapon::SpreadBat*>(weapon), lv, projLevel);
		else if (id == "coilgun")	upgradeCoilgun(static_cast<Weapon::AutoBat*>(weapon), lv, projLevel);
		else if (id == "railgun")	upgradeRailgun(static_cast<Weapon::SemiBat*>(weapon), lv, projLevel);
		else if (id == "thumper")	upgradeThumper(static_cast<Weapon::SemiMag*>(weapon), lv, projLevel);
		else if (id == "launcher")	upgradeLauncher(static_cast<Weapon::SemiMag*>(weapon), lv, projLevel);
	}
}

void Weapon::Armory::upgradeWeapon(WeaponI * weapon)
{
	//Auto-correct mislevelled weapons
	int curLevel = std::max(0, weapon->getLevel());

	//If we have levels to go to
	if (curLevel + 1 < 9)
	{
		setWeaponLevel(weapon, curLevel + 1);
	}
}

ProjectileDef Weapon::Armory::getShrapnel(int level)
{
	//L0 : Default
	ProjectileDef shard = ProjectileDef();
	shard.velScale = 0.5f;
	shard.hpMAX = 1;
	shard.width = 0.15f;
	shard.damage = 1;
	shard.lifeTime = 25;

	//L1H : Damage+
	if (level > 0) { shard.damage = 2; }

	//L2B : Lifetime+
	if (level > 1) { shard.lifeTime = 40; }

	//L3F : Speed+
	if (level > 2) { shard.velScale = 0.75f; }

	//L4S : Speed++
	if (level > 3) { shard.velScale = 1.f; }

	//L5H : Damage++
	if (level > 4) { shard.damage = 3; }

	//L6B : Bounce+
	if (level > 5) { shard.bounce = 0.5f; }

	//L7F : Speed++
	if (level > 6) { shard.velScale = 1.25f; }

	//L8S : Lifetime++
	if (level > 7) { shard.lifeTime = 64; }

	return shard;
}

ProjectileDef Weapon::Armory::getPellet(int level)
{
	//L0 : Default
	ProjectileDef pellet = ProjectileDef();
	pellet.velScale = 1.f;
	pellet.hpMAX = 1;
	pellet.width = 0.25f;
	pellet.damage = 4;
	pellet.lifeTime = 150;

	//L1H : Damage+
	if (level > 0) { pellet.damage = 6; }

	//L2B : Life+
	if (level > 1) { pellet.lifeTime = 200; }

	//L3F : Speed+
	if (level > 2) { pellet.velScale = 1.25f; }

	//L4S : HP+
	if (level > 3) { pellet.hpMAX = 2; }

	//L5H : Damage++
	if (level > 4) { pellet.damage = 8; }

	//L6B : Bounce+
	if (level > 5) { pellet.bounce = 0.5f; }

	//L7F : Speed++
	if (level > 6) { pellet.velScale = 1.5f; }

	//L8S : HP++
	if (level > 7) { pellet.hpMAX = 3; }

	return pellet;
}

ProjectileDef Weapon::Armory::getNinMil(int level)
{
	//L0 : Default
	ProjectileDef ninmil = ProjectileDef();
	ninmil.velScale = 1.f;
	ninmil.hpMAX = 1;
	ninmil.width = 0.75f;
	ninmil.damage = 8;
	ninmil.lifeTime = 100;

	//L1H : Damage+
	if (level > 0) { ninmil.damage = 12; }

	//L2B : Lifetime+
	if (level > 1) { ninmil.lifeTime = 150; }

	//L3F : Speed+
	if (level > 2) { ninmil.velScale = 1.2f; }

	//L4S : Penetration+
	if (level > 3) { ninmil.penetration = 1; }

	//L5H : Damage++
	if (level > 4) { ninmil.damage = 24; }

	//L6B : Force+
	if (level > 5) { ninmil.force.first = 0.1f; ninmil.force.second = 1.f; }

	//L7F : Speed++
	if (level > 6) { ninmil.velScale = 1.6; }

	//L8S : Lifetime++
	if (level > 7) { ninmil.lifeTime = 200; }

	return ninmil;
}

ProjectileDef Weapon::Armory::getBullet(int level)
{
	//L0 : Default
	ProjectileDef bullet = ProjectileDef();
	bullet.velScale = 0.8f;
	bullet.hpMAX = 1;
	bullet.width = 1.f;
	bullet.damage = 8;
	bullet.lifeTime = 100;

	//L1H : Lifetime+
	if (level > 0) { bullet.lifeTime = 125; }

	//L2B : Damage+
	if (level > 1) { bullet.damage = 12; }

	//L3F : Speed+
	if (level > 2) { bullet.velScale = 1.f;}

	//L4S : HP+
	if (level > 3) { bullet.hpMAX = 2; }

	//L5H : Force+
	if (level > 4) { bullet.force.first = 0.1f; bullet.force.second = 1.f; }

	//L6B : Damage++
	if (level > 5) { bullet.damage = 16; }

	//L7F : Speed++
	if (level > 6) { bullet.velScale = 1.2f; }

	//L8S : Penetration+
	if (level > 7) { bullet.penetration = 1; }

	return bullet;
}
//
ProjectileDef Weapon::Armory::getCannonball(int level)
{
	//L0 : Default
	ProjectileDef ball = ProjectileDef();
	ball.velScale = 3;
	ball.hpMAX = 4;
	ball.width = 4.f;
	ball.damage = 2.f;
	ball.lifeTime = 1500;


	//L1H : 
	if (level > 0) {}

	//L2B : 
	if (level > 1) {}

	//L3F : 
	if (level > 2) {}

	//L4S : 
	if (level > 3) {}

	//L5H : 
	if (level > 4) {}

	//L6B :
	if (level > 5) {}

	//L7F :
	if (level > 6) {}

	//L8S :
	if (level > 7) {}

	return ball;
}
//
ProjectileDef Weapon::Armory::getGrenade(int level)
{
	//L0 : Default
	ProjectileDef grenade = ProjectileDef();
	grenade.velScale = 1.f;
	grenade.oneHit = true;
	grenade.hpMAX = 1;
	grenade.force.first = 0.25f;
	grenade.force.second = 2.f;
	grenade.width = 1.75f;
	grenade.height = 1.75f;
	grenade.shrapnel.first = 12;
	grenade.shrapnel.second = 4;
	grenade.damage = 1;
	grenade.lifeTime = 1000;

	//L1H : 
	if (level > 0) {}

	//L2B : 
	if (level > 1) {}

	//L3F : 
	if (level > 2) {}

	//L4S : 
	if (level > 3) {}

	//L5H : 
	if (level > 4) {}

	//L6B :
	if (level > 5) {}

	//L7F :
	if (level > 6) {}

	//L8S :
	if (level > 7) {}

	return grenade;
}
//
ProjectileDef Weapon::Armory::getRocket(int level)
{
	//L0 : Default
	ProjectileDef rocket = ProjectileDef();
	rocket.velScale = 2.f;
	rocket.oneHit = true;
	rocket.hpMAX = 1;
	rocket.width = 2.f;
	rocket.height = 3.f;
	rocket.shrapnel.first = 8;
	rocket.damage = 1.f;
	rocket.lifeTime = 500;

	//L1H : 
	if (level > 0) {}

	//L2B : 
	if (level > 1) {}

	//L3F : 
	if (level > 2) {}

	//L4S : 
	if (level > 3) {}

	//L5H : 
	if (level > 4) {}

	//L6B :
	if (level > 5) {}

	//L7F :
	if (level > 6) {}

	//L8S :
	if (level > 7) {}

	return rocket;
}
//
ProjectileDef Weapon::Armory::getLaser(int level)
{
	//L0 : Default
	ProjectileDef laser = ProjectileDef();
	laser.velScale = 1.5f;
	laser.hpMAX = 1;
	laser.width = .5f;
	laser.height = 6.f;
	laser.penetration = 3;
	laser.damage = 1.f;
	laser.lifeTime = 500;

	//L1H : 
	if (level > 0) {}

	//L2B : 
	if (level > 1) {}

	//L3F : 
	if (level > 2) {}

	//L4S : 
	if (level > 3) {}

	//L5H : 
	if (level > 4) {}

	//L6B :
	if (level > 5) {}

	//L7F :
	if (level > 6) {}

	//L8S :
	if (level > 7) {}

	return laser;
}
//
ProjectileDef Weapon::Armory::getSlug(int level)
{
	//L0 : Default
	ProjectileDef slug = ProjectileDef();

	slug.width = .8f;
	slug.height = 1.6f;
	slug.force.first = 0.4f;
	slug.force.second = 0.5f;
	slug.hpMAX = 1;
	slug.penetration = 1;
	slug.damage = 8;
	slug.velScale = 1.f;
	slug.lifeTime = 3000;

	//L1H : 
	if (level > 0) {}

	//L2B : 
	if (level > 1) {}

	//L3F : 
	if (level > 2) {}

	//L4S : 
	if (level > 3) {}

	//L5H : 
	if (level > 4) {}

	//L6B :
	if (level > 5) {}

	//L7F :
	if (level > 6) {}

	//L8S :
	if (level > 7) {}

	return slug;
}

ProjectileDef Weapon::Armory::getFlammen(int level)
{
	//L0 : Default
	ProjectileDef flam = ProjectileDef();

	flam.width = 0.01f;
	flam.height = 1.f;
	flam.hpMAX = 1;
	flam.damage = 2;
	flam.velScale = 0.15f;
	flam.lifeTime = 100;

	//L1H : Speed+
	if (level > 0) { flam.velScale = 0.25f; }

	//L2B : Pen+
	if (level > 1) { flam.penetration = 1; }

	//L3F : Speed++
	if (level > 2) { flam.velScale = 0.3f; }

	//L4S : Damage+
	if (level > 3) { flam.damage = 4; }

	//L5H : Pen++
	if (level > 4) { flam.penetration = 2; }

	//L6B : LifeTime+
	if (level > 5) { flam.lifeTime = 150; }

	//L7F : Damage++
	if (level > 6) { flam.damage = 6; }

	//L8S : LifeTime++
	if (level > 7) { flam.lifeTime = 200; }

	return flam;
}

void Weapon::Armory::upgradePistol(Weapon::SemiMag* pistol, int level, int projLevel)
{
	//Set base attributes
	pistol->setMagSize(12, true);
	pistol->setResetTime(200);
	pistol->setReloadTime(2500);

	//Match weapon level
	if (projLevel > -1) pistol->setProjectile(getPellet(projLevel));
	else pistol->setProjectile(getNinMil(level));

	//L1H : ResetTime-
	if (level > 0) { pistol->setResetTime(150); }

	//L2B : ReloadTime-
	if (level > 1) { pistol->setReloadTime(1750); }

	//L3F : RefireTime--
	if (level > 2) { pistol->setResetTime(100); }

	//L4S : Mag+
	if (level > 3) { pistol->setMagSize(18, true); }

	//L5H : ReloadTime--
	if (level > 4) { pistol->setReloadTime(750); }

	//L6B : Mag++
	if (level > 5) { pistol->setMagSize(24, true); }

	//L7F : ReloadTime--
	if (level > 6) { pistol->setResetTime(50); }

	//L8S : Mag+++
	if (level > 7) { pistol->setMagSize(32, true); }
}

void Weapon::Armory::upgradeRifle(Weapon::AutoMag* rifle, int level, int projLevel)
{
	//Set base attributes
	rifle->setMagSize(27, true);
	rifle->setRefireTime(150);
	rifle->setReloadTime(4000);
	rifle->setSpread(0.005);

	//Match weapon level
	if (projLevel > -1) rifle->setProjectile(getPellet(projLevel));
	else rifle->setProjectile(getBullet(level));

	//L1H : Spread-
	if (level > 0) { rifle->setSpread(0.0025); }

	//L2B : Mag+
	if (level > 1) { rifle->setMagSize(36, true); }

	//L3F : ReloadTime-
	if (level > 2) { rifle->setReloadTime(3000); }

	//L4S : Mag+
	if (level > 3) { rifle->setSpread(.2f); }

	//L5H : ReloadTime--
	if (level > 4) { rifle->setReloadTime(2000); }

	//L6B : Spread--
	if (level > 5) { rifle->setSpread(0.001f); }

	//L7F : RefireTime-
	if (level > 6) { rifle->setRefireTime(50); }

	//L8S : Mag++
	if (level > 7) { rifle->setMagSize(45, true); }
}
//
void Weapon::Armory::upgradeCannon(Weapon::SemiMag* cannon, int level, int projLevel)
{
	cannon->setResetTime(1000);
	cannon->setReloadTime(4000);
	cannon->setMagSize(8);
}

void Weapon::Armory::upgradeShotgun(Weapon::SpreadMag* shotgun, int level, int projLevel)
{
	//Set base attributes
	shotgun->setMagSize(8, true);
	shotgun->setResetTime(400);
	shotgun->setReloadTime(1500);
	shotgun->setPellets(6);
	shotgun->setSpread(.3f);

	//Match weapon level
	if (projLevel != -1) shotgun->setProjectile(getPellet(projLevel));
	else shotgun->setProjectile(getNinMil(level));

	//L1H : Pellets+
	if (level > 0) { shotgun->setPellets(8); }

	//L2B : Spread-
	if (level > 1) { shotgun->setSpread(.2f); }

	//L3F : RefireTime-
	if (level > 2) { shotgun->setResetTime(200); }

	//L4S : Mag+
	if (level > 3) { shotgun->setMagSize(12, true); }

	//L5H : Pellets+
	if (level > 4) { shotgun->setPellets(10); }

	//L6B : Spread-
	if (level > 5) { shotgun->setSpread(.15f); }

	//L7F : RefireTime-
	if (level > 6) { shotgun->setResetTime(100); }

	//L8S : Mag+
	if (level > 7) { shotgun->setMagSize(16, true); }
}

void Weapon::Armory::upgradeWerfer(Weapon::SpreadBat* werfer, int level, int projLevel)
{
	//Match weapon level
	if (projLevel > -1) werfer->setProjectile(getFlammen(projLevel));
	else werfer->setProjectile(getFlammen(level));

	//L1H : Amount+
	if (level > 0) { werfer->setAmount(12); }

	//L2B : AmountScale+
	if (level > 1) { werfer->setAmountScale(0.6f); }

	//L3F : RechargeTime-
	if (level > 2) { werfer->setRechargeTime(1500); }

	//L4S : SpreadScale-
	if (level > 3) { werfer->setSpreadScale(0.75f); }

	//L5H : Amount++
	if (level > 4) { werfer->setAmount(16); }

	//L6B : Spread-
	if (level > 5) { werfer->setSpread(0.75f); }

	//L7F : RechargeTime--
	if (level > 6) { werfer->setRechargeTime(1000); }

	//L8S : AmountScale++
	if (level > 7) { werfer->setAmountScale(0.75f);}
}
//
void Weapon::Armory::upgradeCoilgun(Weapon::AutoBat* coilgun, int level, int projLevel)
{


}
//
void Weapon::Armory::upgradeRailgun(Weapon::SemiBat* railgun, int level, int projLevel)
{

}
//
void Weapon::Armory::upgradeThumper(Weapon::SemiMag* thumper, int level, int projLevel)
{
}
//
void Weapon::Armory::upgradeLauncher(Weapon::SemiMag* launcher, int level, int projLevel)
{
}
