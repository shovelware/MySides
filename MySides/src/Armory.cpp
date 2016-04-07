#include "Armory.hpp"

Weapon::Armory::Armory(FireFunc& fireCallback) :
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
	//Min level 0
	int weaplv = std::max(0, std::min(level, 8));
	//If -1, match weapon level (default behaviour)
	int projlv = (projLevel > -1 ? projLevel : level);

	Weapon::SpreadMag* thumper = new Weapon::SpreadMag(fireCallback_, getGrenade(projlv), "thumper");
	upgradeThumper(thumper, weaplv, projLevel);
	return static_cast<Weapon::WeaponI*>(thumper);
}

Weapon::WeaponI* Weapon::Armory::getLauncher(int level, int projLevel)
{
	//Min level 0
	int weaplv = std::max(0, std::min(level, 8));
	//If -1, match weapon level (default behaviour)
	int projlv = (projLevel > -1 ? projLevel : level);

	Weapon::SemiMag* launcher = new Weapon::SemiMag(fireCallback_, getRocket(projlv), "launcher");
	upgradeLauncher(launcher, weaplv, projLevel);
	return static_cast<Weapon::WeaponI*>(launcher);
}

//Fun weapons
Weapon::WeaponI* Weapon::Armory::getFun(int type)
{
	Weapon::WeaponI * funGun = nullptr;

	switch (type)
	{
		//Boomerang
	case 22:
	{
	}


	// Explosive Buckshot
	case 44:
	{
		ProjectileDef gren = ProjectileDef();
		gren.bounce = 0.f;
		gren.hpMAX = 1;
		gren.damage = 10;
		//gren.oneHit = true;
		gren.detonation.force = 0.25f;
		gren.detonation.radius = 2.f;
		gren.detonation.lifeTime = 50;
		gren.shrapnel.shards = 8;
		gren.shrapnel.level = 4;
		gren.width = 4.f;
		gren.lifeTime = 250;

		funGun = new Weapon::SpreadMag(fireCallback_, gren, "thumper", 16, 50, 500, 8, 0.3f);
		break;
	}
	
	//Broad Beam
	case 42:
	{
		ProjectileDef laser = getLaser();
		laser.damage = 80.f;
		laser.hpMAX = 20;
		laser.lifeTime = 5000;
		laser.height = 50.f;
		laser.width = 200.f;
		laser.penetration = 100;
		
		funGun = new Weapon::SemiBat(fireCallback_, laser, "coilgun", 1280, 32, 64);
		break;
	}
	
	//Flak cannon
	case 47:
	{
		ProjectileDef shrap = getShrapnel(8);
		shrap.lifeTime = 175;
		shrap.oneHit = true;
		shrap.height = shrap.width * 8;
		shrap.width *= 4;
		shrap.shrapnel.shards = 4;
		shrap.shrapnel.level = 8;
		shrap.detonation.force = 0.5f;
		shrap.detonation.radius = 1.f;
		shrap.detonation.lifeTime = 150;

		funGun = new Weapon::SpreadMag(fireCallback_, shrap, "shotgun", 4, 0, 300, 10, 2.f);

		break;
	}	

	//Chug cannon
	case 60:
	{
		ProjectileDef shug = ProjectileDef();
		shug.velScale = 1.f;
		shug.width = 1.f;
		shug.hpMAX = 1;
		//shug.bounce = 1.0;
		shug.damage = 20;
		//shug.oneHit = true;
		shug.shrapnel.shards = 32;
		shug.shrapnel.level = 8;
		shug.lifeTime = 500;

		funGun = new Weapon::AutoMag(fireCallback_, shug, "launcher", 24, 50, 2000, 0.15f );
		break;
	}

	//Black Hole Gun
	case 88:
	{
		ProjectileDef voib = ProjectileDef();
		voib.detonation.force = -0.1f;
		voib.detonation.radius = 10.f;
		voib.detonation.lifeTime = 2500;
		voib.lifeTime = 500;
		voib.penetration = 15;
		voib.hpMAX = 10;
		voib.bounce = 0.75f;
		voib.width = 1;
		voib.height = 1;

		funGun = new Weapon::SemiMag(fireCallback_, voib, "railgun", 2, 400, 2000);
		break;
	}

	//Lance Beam
	case 111:
	{
		ProjectileDef lance = ProjectileDef();
		lance.velScale = 1.f;
		lance.width = 0.5f;
		lance.height = 4.f;
		lance.penetration = 4;
		lance.hpMAX = 4;
		lance.damage = 30;
		lance.lifeTime = 300;

		funGun = new Weapon::SpreadBat(fireCallback_, lance, "coilgun", 2400, 16, 500, 16, 1, -10.0, 4.f, 2.f);
		break;
	}

	case 404:
	{
		ProjectileDef missile = ProjectileDef();
		missile.velScale = 1.2f;
		missile.width = 2.5f;
		missile.height = 5.f;
		missile.tracking.radius = 20;
		missile.tracking.speed = 5.f;
		missile.lifeTime = 7500;
		//missile.shrapnel.level = 7;
		//missile.shrapnel.shards = 4;
		missile.detonation.force = 0.4f;
		missile.detonation.lifeTime = 50;
		missile.detonation.radius = 1.f;
		missile.oneHit = true;
		missile.hpMAX = 1;
		
		funGun = new Weapon::SemiMag(fireCallback_, missile, "launcher", 8, 500, 3000);
		break;
	}

	//Mikrowerfer
	case 666:
	{
		ProjectileDef wave = ProjectileDef();
		wave.velScale = 4.f;
		wave.width = 2.5f;
		wave.bounce = 1.f;
		wave.hpMAX = 8;
		wave.damage = 40;
		wave.lifeTime = 3000;

		funGun = new Weapon::SpreadBat(fireCallback_, wave, "launcher", 320, 16, 800, 16, 6, -1.0, 0.9f, -2.f);
		break;
	}

	//Chaingun
	case 888:
	{
		ProjectileDef bb = getShrapnel(8);
		bb.lifeTime = 200;
		bb.oneHit = true;
		bb.bounce = 0.f;
		bb.detonation.force = 0.025f;
		bb.detonation.radius = 0.5f;
		bb.detonation.lifeTime = 25;

		funGun = new Weapon::SpreadBat(fireCallback_, bb, "pistol", 2000, 32, 1000, 10, 8, 0, 0.5, 1.1f);
		
		break;
	}

	}//End switch

	if (funGun != nullptr)
		funGun->setLevel(type);
	
	return funGun;
}

void Weapon::Armory::setWeaponLevel(WeaponI * weapon, int level, int projLevel)
{
	if (weapon != nullptr && weapon->getLevel() != level)
	{
		std::string id = weapon->getID();
		int lv = std::max(0, std::min(level, 8));

		if (id == "pistol")			upgradePistol(static_cast<Weapon::SemiMag*>(weapon), lv, projLevel);
		else if (id == "rifle")		upgradeRifle(static_cast<Weapon::AutoMag*>(weapon), lv, projLevel);
		else if (id == "cannon")	upgradeCannon(static_cast<Weapon::SemiMag*>(weapon), lv, projLevel);
		else if (id == "shotgun")	upgradeShotgun(static_cast<Weapon::SpreadMag*>(weapon), lv, projLevel);
		else if (id == "werfer")	upgradeWerfer(static_cast<Weapon::SpreadBat*>(weapon), lv, projLevel);
		else if (id == "coilgun")	upgradeCoilgun(static_cast<Weapon::AutoBat*>(weapon), lv, projLevel);
		else if (id == "railgun")	upgradeRailgun(static_cast<Weapon::SemiBat*>(weapon), lv, projLevel);
		else if (id == "thumper")	upgradeThumper(static_cast<Weapon::SpreadMag*>(weapon), lv, projLevel);
		else if (id == "launcher")	upgradeLauncher(static_cast<Weapon::SemiMag*>(weapon), lv, projLevel);

		weapon->reup(true);
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
	shard.velScale = 0.75f;
	shard.hpMAX = 100;
	shard.width = 0.15f;
	shard.damage = 4;
	shard.lifeTime = 25;

	//L1H : Damage+
	if (level > 0) { shard.damage = 6; }

	//L2B : Lifetime+
	if (level > 1) { shard.lifeTime = 75; }

	//L3F : Speed+
	if (level > 2) { shard.velScale = 1.f; }

	//L4S : Speed++
	if (level > 3) { shard.velScale = 1.25f; }

	//L5H : Damage++
	if (level > 4) { shard.damage = 8; }

	//L6B : Bounce+
	if (level > 5) { shard.bounce = 1.f; }

	//L7F : Speed++
	if (level > 6) { shard.velScale = 1.5f; }

	//L8S : Lifetime++
	if (level > 7) { shard.lifeTime = 100; }

	return shard;
}

ProjectileDef Weapon::Armory::getPellet(int level)
{
	//L0 : Default
	ProjectileDef pellet = ProjectileDef();
	pellet.velScale = 0.75f;
	pellet.hpMAX = 1;
	pellet.width = 0.35f;
	pellet.damage = 4;
	pellet.lifeTime = 150;

	//L1H : Damage+
	if (level > 0) { pellet.damage = 6; }

	//L2B : Lifetime+
	if (level > 1) { pellet.lifeTime = 200; }

	//L3F : Speed+
	if (level > 2) { pellet.velScale = 1.f; }

	//L4S : HP+
	if (level > 3) { pellet.hpMAX = 2; }

	//L5H : Damage++
	if (level > 4) { pellet.damage = 8; }

	//L6B : Bounce+
	if (level > 5) { pellet.bounce = 0.75f; }

	//L7F : Lifetime++
	if (level > 6) { pellet.lifeTime = 500; }

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

	//L6B : Det+
	if (level > 5) { ninmil.detonation.force = 0.05f; ninmil.detonation.radius = 0.5f; ninmil.detonation.lifeTime = 25; }

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

	//L5H : Det+
	if (level > 4) { bullet.detonation.force = 0.05f; bullet.detonation.radius = 0.5f; bullet.detonation.lifeTime = 25; }

	//L6B : Damage++
	if (level > 5) { bullet.damage = 16; }

	//L7F : Lifetime++
	if (level > 6) { bullet.lifeTime = 250; }

	//L8S : Penetration+
	if (level > 7) { bullet.penetration = 1; }

	return bullet;
}

ProjectileDef Weapon::Armory::getCannonball(int level)
{
	//L0 : Default
	ProjectileDef ball = ProjectileDef();
	ball.velScale = 2;
	ball.hpMAX = 4;
	ball.width = 4.f;
	ball.damage = 24;
	ball.lifeTime = 200;

	//L1H : Damage+
	if (level > 0) { ball.damage = 32; }

	//L2B : Size+
	if (level > 1) { ball.width = 6.f; }

	//L3F : Speed+
	if (level > 2) { ball.velScale = 3; }

	//L4S : Det+
	if (level > 3) { ball.shrapnel.shards = 16; ball.shrapnel.level = 5; }

	//L5H : Lifetime+
	if (level > 4) { ball.lifeTime = 400; }

	//L6B : Size+
	if (level > 5) { ball.width = 8.f; }

	//L7F : Speed++
	if (level > 6) { ball.velScale = 4; }

	//L8S : HP+
	if (level > 7) { ball.hpMAX = 8; ball.bounce = 0.9f; }

	return ball;
}

ProjectileDef Weapon::Armory::getGrenade(int level)
{
	//L0 : Default
	ProjectileDef grenade = ProjectileDef();
	grenade.velScale = 1.f;
	grenade.oneHit = true;
	grenade.hpMAX = 1;
	grenade.detonation.force = 0.5f;
	grenade.detonation.radius = 2.f;
	grenade.detonation.lifeTime = 250;
	grenade.width = 1.75f;
	grenade.height = 1.75f;
	grenade.shrapnel.shards = 12;
	grenade.shrapnel.level = 6;
	grenade.damage = 10;
	grenade.lifeTime = 400;

	//L1H : Damage+
	if (level > 0) { grenade.damage = 12; }

	//L2B : LifeTime++
	if (level > 1) { grenade.lifeTime = 600; }

	//L3F : Speed+
	if (level > 2) { grenade.velScale = 1.2f; }

	//L4S :  Det+
	if (level > 3) { grenade.detonation.force = 0.2f; }

	//L5H : Shrapnel+
	if (level > 4) { grenade.shrapnel.level = 8; }

	//L6B : Damage++
	if (level > 5) { grenade.damage = 16; }

	//L7F : Det++
	if (level > 6) { grenade.detonation.force = 1.f; grenade.detonation.radius = 2.5f; grenade.detonation.lifeTime = 300; }

	//L8S : Speed++
	if (level > 7) { grenade.velScale = 1.5f; }

	return grenade;
}

ProjectileDef Weapon::Armory::getRocket(int level)
{
	//L0 : Default
	ProjectileDef rocket = ProjectileDef();
	rocket.velScale = 1.f;
	rocket.oneHit = true;
	rocket.hpMAX = 1;
	rocket.width = 2.f;
	rocket.height = 3.f;
	rocket.detonation.force = 0.5f;
	rocket.detonation.radius = 2.f;
	rocket.detonation.lifeTime = 50;
	rocket.shrapnel.shards = 8;
	rocket.shrapnel.level = 4;
	rocket.damage = 16;
	rocket.lifeTime = 500;

	//L1H : Speed+
	if (level > 0) { rocket.velScale = 1.5f; }

	//L2B : Damage+
	if (level > 1) { rocket.damage = 24; }

	//L3F : Speed+
	if (level > 2) { rocket.velScale = 2.f; }

	//L4S : Damage++
	if (level > 3) { rocket.damage = 32; }

	//L5H : Shrapnel+
	if (level > 4) { rocket.shrapnel.level = 6; }

	//L6B : Det+
	if (level > 5) { rocket.detonation.force = 1.f; rocket.detonation.radius = 3.f;	rocket.detonation.lifeTime = 75; }

	//L7F : Damage++++
	if (level > 6) { rocket.damage = 48; }

	//L8S : Det++
	if (level > 7) { rocket.detonation.force = 0.5f; rocket.detonation.radius = 4.f; rocket.detonation.lifeTime = 100; }

	return rocket;
}

ProjectileDef Weapon::Armory::getLaser(int level)
{
	//L0 : Default
	ProjectileDef laser = ProjectileDef();
	laser.velScale = 1.25f;
	laser.hpMAX = 1;
	laser.width = .5f;
	laser.height = 6.f;
	laser.penetration = 1;
	laser.damage = 4;
	laser.lifeTime = 175;

	//L1H : Damage+
	if (level > 0) { laser.damage = 8; }

	//L2B : Penetration+
	if (level > 1) { laser.penetration = 2; }

	//L3F : Speed+
	if (level > 2) { laser.velScale = 1.5f; }

	//L4S : Lifetime+
	if (level > 3) { laser.lifeTime = 300; }

	//L5H : Penetration++
	if (level > 4) { laser.penetration = 3; }

	//L6B : Damage++
	if (level > 5) { laser.damage = 16; }

	//L7F : Speed++
	if (level > 6) { laser.velScale = 1.75f; }

	//L8S : Damage +++
	if (level > 7) { laser.damage = 24; }

	return laser;
}

ProjectileDef Weapon::Armory::getSlug(int level)
{
	//L0 : Default
	ProjectileDef slug = ProjectileDef();

	slug.width = .8f;
	slug.hpMAX = 1;
	slug.damage = 32;
	slug.velScale = 0.75f;
	slug.lifeTime = 500;
	slug.height = 1.6f;

	//L1H : Damage+
	if (level > 0) { slug.damage = 48; }

	//L2B : Penetration+
	if (level > 1) { slug.penetration = 1; }

	//L3F : Speed+
	if (level > 2) { slug.velScale = 1.f;  }

	//L4S : Det+
	if (level > 3) { slug.detonation.force = 0.2f; slug.detonation.radius = 1.f; slug.detonation.lifeTime = 50; }

	//L5H : Damage++
	if (level > 4) { slug.damage = 64; }

	//L6B : Lifetime+
	if (level > 5) { slug.lifeTime = 750; }

	//L7F : Speed++
	if (level > 6) { slug.velScale = 1.1; }

	//L8S : Penetration++
	if (level > 7) { slug.penetration = 2; }

	return slug;
}

ProjectileDef Weapon::Armory::getFlammen(int level)
{
	//L0 : Default
	ProjectileDef flam = ProjectileDef();

	flam.width = 4.f;
	flam.height = 1.5f;
	flam.hpMAX = 1;
	flam.damage = 2;
	flam.velScale = 0.75f;
	flam.lifeTime = 100;

	//L1H : LifeTime+
	if (level > 0) { flam.lifeTime = 150; }

	//L2B : Pen+
	if (level > 1) { flam.penetration = 1; }

	//L3F : Speed++
	if (level > 2) { flam.velScale = 1.f; }

	//L4S : Damage+
	if (level > 3) { flam.damage = 4; }

	//L5H : Pen++
	if (level > 4) { flam.penetration = 2; }

	//L6B : LifeTime++
	if (level > 5) { flam.lifeTime = 200; }

	//L7F : Damage++
	if (level > 6) { flam.damage = 6; }

	//L8S : LifeTime+++
	if (level > 7) { flam.lifeTime = 300; }

	return flam;
}

void Weapon::Armory::upgradePistol(Weapon::SemiMag* pistol, int level, int projLevel)
{
	//Match weapon level
	if (projLevel > -1) pistol->setProjectile(getNinMil(projLevel));
	else pistol->setProjectile(getNinMil(level));
	pistol->setLevel(level);

	//Set base attributes
	pistol->setMagSize(12, true);
	pistol->setResetTime(200);
	pistol->setReloadTime(2500);

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
	//Match weapon level
	if (projLevel > -1) rifle->setProjectile(getBullet(projLevel));
	else rifle->setProjectile(getBullet(level));
	rifle->setLevel(level);

	//Set base attributes
	rifle->setMagSize(27, true);
	rifle->setRefireTime(150);
	rifle->setReloadTime(4000);
	rifle->setSpread(0.125f);

	//L1H : Spread-
	if (level > 0) { rifle->setSpread(0.0025); }

	//L2B : Mag+
	if (level > 1) { rifle->setMagSize(36, true); }

	//L3F : ReloadTime-
	if (level > 2) { rifle->setReloadTime(3000); }

	//L4S : Mag+
	if (level > 3) { rifle->setSpread(.1f); }

	//L5H : ReloadTime--
	if (level > 4) { rifle->setReloadTime(2000); }

	//L6B : Spread--
	if (level > 5) { rifle->setSpread(0.05f); }

	//L7F : RefireTime-
	if (level > 6) { rifle->setRefireTime(50); }

	//L8S : Mag++
	if (level > 7) { rifle->setMagSize(45, true); }
}

void Weapon::Armory::upgradeCannon(Weapon::SemiMag* cannon, int level, int projLevel)
{
	//Match weapon level
	if (projLevel > -1) cannon->setProjectile(getCannonball(projLevel));
	else cannon->setProjectile(getCannonball(level));
	cannon->setLevel(level);

	//Set base attributes
	cannon->setMagSize(6);
	cannon->setResetTime(1000);
	cannon->setReloadTime(4000);

	//L1H : ResetTime-
	if (level > 0) { cannon->setResetTime(800); }

	//L2B : Magsize+
	if (level > 1) { cannon->setMagSize(12); }

	//L3F : ReloadTime-
	if (level > 2) { cannon->setReloadTime(3000); }

	//L4S : ResetTime--
	if (level > 3) { cannon->setResetTime(600); }

	//L5H : Magsize++
	if (level > 4) { cannon->setMagSize(18); }

	//L6B : Magsize+++
	if (level > 5) { cannon->setMagSize(24); }

	//L7F : ReloadTime--
	if (level > 6) { cannon->setReloadTime(2000); }

	//L8S : ResetTime---
	if (level > 7) { cannon->setResetTime(400); }

}

void Weapon::Armory::upgradeShotgun(Weapon::SpreadMag* shotgun, int level, int projLevel)
{
	//Match weapon level
	if (projLevel != -1) shotgun->setProjectile(getPellet(projLevel));
	else shotgun->setProjectile(getPellet(level));
	shotgun->setLevel(level);

	//Set base attributes
	shotgun->setMagSize(8, true);
	shotgun->setResetTime(200);
	shotgun->setReloadTime(1500);
	shotgun->setPellets(6);
	shotgun->setSpread(.3f);

	//L1H : Pellets+
	if (level > 0) { shotgun->setPellets(8); }

	//L2B : Spread-
	if (level > 1) { shotgun->setSpread(.2f); }

	//L3F : ResetTime-
	if (level > 2) { shotgun->setResetTime(100); }

	//L4S : Mag+
	if (level > 3) { shotgun->setMagSize(12, true); }

	//L5H : Pellets+
	if (level > 4) { shotgun->setPellets(10); }

	//L6B : Spread-
	if (level > 5) { shotgun->setSpread(.15f); }

	//L7F : ReloadTime-
	if (level > 6) { shotgun->setReloadTime(750); }

	//L8S : Mag+
	if (level > 7) { shotgun->setMagSize(16, true); }
}

void Weapon::Armory::upgradeWerfer(Weapon::SpreadBat* werfer, int level, int projLevel)
{
	//Match weapon level
	if (projLevel > -1) werfer->setProjectile(getFlammen(projLevel));
	else werfer->setProjectile(getFlammen(level));
	werfer->setLevel(level);

	//Set base attributes
	werfer->setBatterySize(1600);
	werfer->setRefireTime(50);
	werfer->setRechargeTime(2000);
	werfer->setAmount(8);
	werfer->setAmountScale(0.5f);
	werfer->setSpread(1.f);
	werfer->setSpreadScale(0.5f);

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

void Weapon::Armory::upgradeCoilgun(Weapon::AutoBat* coilgun, int level, int projLevel)
{
	//Match weapon level
	if (projLevel > -1) coilgun->setProjectile(getLaser(projLevel));
	else coilgun->setProjectile(getLaser(level));
	coilgun->setLevel(level);

	//Set base attributes
	coilgun->setBatterySize(1600);
	coilgun->setRefireTime(150);
	coilgun->setRechargeTime(1000);
	coilgun->setFireCharge(40);

	//L1H : RefireTime-
	if (level > 0) { coilgun->setRefireTime(100); }

	//L2B : BatterySize+
	if (level > 1) { coilgun->setBatterySize(2400, true); }

	//L3F : RechargeTime-
	if (level > 2) { coilgun->setRechargeTime(500); }

	//L4S : FireCharge-
	if (level > 3) { coilgun->setFireCharge(20); }

	//L5H : RefireTime--
	if (level > 4) { coilgun->setRefireTime(50); }

	//L6B : RechargeTime--
	if (level > 5) { coilgun->setRechargeTime(250); }

	//L7F : BatterySize++
	if (level > 6) { coilgun->setBatterySize(3200, true); }

	//L8S : FireCharge--
	if (level > 7) { coilgun->setFireCharge(16); }

}

void Weapon::Armory::upgradeRailgun(Weapon::SemiBat* railgun, int level, int projLevel)
{
	//Match weapon level
	if (projLevel > -1) railgun->setProjectile(getSlug(projLevel));
	else railgun->setProjectile(getSlug(level));
	railgun->setLevel(level);

	//Set base attributes
	railgun->setBatterySize(1280);
	railgun->setChargeAmount(8);
	railgun->setDischargeAmount(16);

	//L1H : Discharge+
	if (level > 0) { railgun->setDischargeAmount(24); }

	//L2B : Charge+
	if (level > 1) { railgun->setChargeAmount(12); }

	//L3F : Battery-
	if (level > 2) { railgun->setBatterySize(960); }

	//L4S : Charge++
	if (level > 3) { railgun->setChargeAmount(16); }

	//L5H : Battery--
	if (level > 4) { railgun->setBatterySize(800); }

	//L6B : Discharge++
	if (level > 5) { railgun->setDischargeAmount(32); }

	//L7F : Discharge++
	if (level > 6) { railgun->setDischargeAmount(48); }

	//L8S : Charge++
	if (level > 7) { railgun->setChargeAmount(24); }

}

void Weapon::Armory::upgradeThumper(Weapon::SpreadMag* thumper, int level, int projLevel)
{
	//Match weapon level
	if (projLevel > -1) thumper->setProjectile(getGrenade(projLevel));
	else thumper->setProjectile(getGrenade(level));
	thumper->setLevel(level);

	//Set base attributes
	thumper->setMagSize(6, true);
	thumper->setResetTime(750);
	thumper->setReloadTime(1500);
	thumper->setPellets(1);
	thumper->setSpread(0.1f);

	//L1H : ResetTime-
	if (level > 0) { thumper->setResetTime(500); }

	//L2B : ReloadTime-
	if (level > 1) { thumper->setReloadTime(1000); }

	//L3F : Spread-
	if (level > 2) { thumper->setSpread(0.05f); }

	//L4S : ResetTime--
	if (level > 3) { thumper->setResetTime(300); }

	//L5H : Magsize+
	if (level > 4) { thumper->setMagSize(12, true); }

	//L6B : ReloadTime--
	if (level > 5) { thumper->setReloadTime(730); }

	//L7F : Pellets+
	if (level > 6) { thumper->setPellets(3); thumper->setSpread(0.1f); }

	//L8S : ReloadTime---
	if (level > 7) { thumper->setReloadTime(500); }

}

void Weapon::Armory::upgradeLauncher(Weapon::SemiMag* launcher, int level, int projLevel)
{
	//Match weapon level
	if (projLevel > -1) launcher->setProjectile(getRocket(projLevel));
	else launcher->setProjectile(getRocket(level));
	launcher->setLevel(level);

	//Set base attributes
	launcher->setMagSize(4, true);
	launcher->setResetTime(800);
	launcher->setReloadTime(3000);

	//L1H : ReloadTime-
	if (level > 0) { launcher->setReloadTime(2500); }

	//L2B : ResetTime-
	if (level > 1) { launcher->setResetTime(600); }

	//L3F : Magsize+
	if (level > 2) { launcher->setMagSize(8, true); }
	
	//L4S : ReestTime--
	if (level > 3) { launcher->setResetTime(400); }

	//L5H : Magsize++
	if (level > 4) { launcher->setMagSize(12, true); }

	//L6B : Reloadtime--
	if (level > 5) { launcher->setReloadTime(2000); }

	//L7F : Magsize+++
	if (level > 6) { launcher->setMagSize(16, true); }

	//L8S : ReloadTime--
	if (level > 7) { launcher->setReloadTime(1500); }

}
