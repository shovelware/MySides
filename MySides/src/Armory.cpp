#include "Armory.hpp"

Weapon::Armory::Armory(std::function<void(std::vector<ProjectileDef>&defs, std::string id)> fireCallback)
{
	fireCallback_ = fireCallback;
}

//Upgrade order: HARDER, BETTER, FASTER, STRONGER

ProjectileDef Weapon::Armory::getShrapnel(int level)
{
	//L0 : Default
	ProjectileDef shard = ProjectileDef();
	shard.velScale = 0.5f;
	shard.hpMAX = 1;
	shard.width = 0.15f;
	shard.damage = 1;
	shard.lifeTime = 25;

	//L1 : 
	if (level > 0) {}

	//L2 : Lifetime+
	if (level > 1) { shard.lifeTime = 40; }

	//L3 : Speed+
	if (level > 2) { shard.velScale = 0.75f; }

	//L4 : 
	if (level > 3) {}

	//L5 : 
	if (level > 4) {}

	//L6 : Bounce+
	if (level > 5) { shard.bounce = 0.5f; }

	//L7 :
	if (level > 6) {}

	//L8 : Damage+
	if (level > 7) { shard.damage = 2; }

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

	//L1 : Damage+
	if (level > 0) { pellet.damage = 6; }

	//L2 : Life+
	if (level > 1) { pellet.lifeTime = 200; }

	//L3 : Speed+
	if (level > 2) { pellet.velScale = 1.25f; }

	//L4 : HP+
	if (level > 3) { pellet.hpMAX = 2; }

	//L5 : Damage++
	if (level > 4) { pellet.damage = 8; }

	//L6 : Bounce+
	if (level > 5) { pellet.bounce = 0.5f; }

	//L7 : Speed++
	if (level > 6) { pellet.velScale = 1.5f; }

	//L8 : HP++
	if (level > 7) { pellet.hpMAX = 3; }

	return pellet;
}

ProjectileDef Weapon::Armory::getNinMil(int level)
{
	//L0 : Default
	ProjectileDef ninmil = ProjectileDef();
	ninmil.velScale = 1;
	ninmil.hpMAX = 1;
	ninmil.width = 0.5f;
	ninmil.damage = 1;
	ninmil.lifeTime = 500;

	//L1 : 
	if (level > 0) {}

	//L2 : 
	if (level > 1) {}

	//L3 : 
	if (level > 2) {}

	//L4 : 
	if (level > 3) {}

	//L5 : 
	if (level > 4) {}

	//L6 :
	if (level > 5) {}

	//L7 :
	if (level > 6) {}

	//L8 :
	if (level > 7) {}

	return ninmil;
}

ProjectileDef Weapon::Armory::getBullet(int level)
{
	//L0 : Default
	ProjectileDef bullet = ProjectileDef();
	bullet.velScale = 1;
	bullet.hpMAX = 1;
	bullet.width = 1;
	bullet.damage = 1.25f;
	bullet.lifeTime = 1000;

	//L1 : 
	if (level > 0) {}

	//L2 : 
	if (level > 1) {}

	//L3 : 
	if (level > 2) {}

	//L4 : 
	if (level > 3) {}

	//L5 : 
	if (level > 4) {}

	//L6 :
	if (level > 5) {}

	//L7 :
	if (level > 6) {}

	//L8 :
	if (level > 7) {}

	return bullet;
}

ProjectileDef Weapon::Armory::getDumDum(int level)
{
	//L0 : Default
	ProjectileDef dumdum = ProjectileDef();
	dumdum.velScale = 2;
	dumdum.hpMAX = 2;
	dumdum.width = 2.f;
	dumdum.damage = 1.5f;
	dumdum.lifeTime = 1250;

	//L1 : 
	if (level > 0) {}

	//L2 : 
	if (level > 1) {}

	//L3 : 
	if (level > 2) {}

	//L4 : 
	if (level > 3) {}

	//L5 : 
	if (level > 4) {}

	//L6 :
	if (level > 5) {}

	//L7 :
	if (level > 6) {}

	//L8 :
	if (level > 7) {}

	return dumdum;
}

ProjectileDef Weapon::Armory::getCannonball(int level)
{
	//L0 : Default
	ProjectileDef ball = ProjectileDef();
	ball.velScale = 3;
	ball.hpMAX = 4;
	ball.width = 4.f;
	ball.damage = 2.f;
	ball.lifeTime = 1500;


	//L1 : 
	if (level > 0) {}

	//L2 : 
	if (level > 1) {}

	//L3 : 
	if (level > 2) {}

	//L4 : 
	if (level > 3) {}

	//L5 : 
	if (level > 4) {}

	//L6 :
	if (level > 5) {}

	//L7 :
	if (level > 6) {}

	//L8 :
	if (level > 7) {}

	return ball;
}

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

	//L1 : 
	if (level > 0) {}

	//L2 : 
	if (level > 1) {}

	//L3 : 
	if (level > 2) {}

	//L4 : 
	if (level > 3) {}

	//L5 : 
	if (level > 4) {}

	//L6 :
	if (level > 5) {}

	//L7 :
	if (level > 6) {}

	//L8 :
	if (level > 7) {}

	return grenade;
}

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

	//L1 : 
	if (level > 0) {}

	//L2 : 
	if (level > 1) {}

	//L3 : 
	if (level > 2) {}

	//L4 : 
	if (level > 3) {}

	//L5 : 
	if (level > 4) {}

	//L6 :
	if (level > 5) {}

	//L7 :
	if (level > 6) {}

	//L8 :
	if (level > 7) {}

	return rocket;
}

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

	//L1 : 
	if (level > 0) {}

	//L2 : 
	if (level > 1) {}

	//L3 : 
	if (level > 2) {}

	//L4 : 
	if (level > 3) {}

	//L5 : 
	if (level > 4) {}

	//L6 :
	if (level > 5) {}

	//L7 :
	if (level > 6) {}

	//L8 :
	if (level > 7) {}

	return laser;
}

ProjectileDef Weapon::Armory::getSlug(int level)
{
	//L0 : Default
	ProjectileDef slug = ProjectileDef();

	slug.width = .8f;
	slug.height = 1.6f;
	slug.force.first = 4.f;
	slug.force.second = 1.f;
	slug.hpMAX = 1;
	slug.penetration = 1;
	slug.damage = 8;
	slug.velScale = 1.f;
	slug.lifeTime = 3000;
	//L1 : 
	if (level > 0) {}

	//L2 : 
	if (level > 1) {}

	//L3 : 
	if (level > 2) {}

	//L4 : 
	if (level > 3) {}

	//L5 : 
	if (level > 4) {}

	//L6 :
	if (level > 5) {}

	//L7 :
	if (level > 6) {}

	//L8 :
	if (level > 7) {}

	return slug;
}

Weapon::WeaponI* Weapon::Armory::getShotgun(int level, int projLevel)
{
	//L0 : Default
	Weapon::SpreadMag* shotty = new Weapon::SpreadMag(fireCallback_, getPellet(projLevel));
	//Base on creation: //Reset = 300 //Reload = 1250 //Pellets = 8; //Spread =  .15f; //MagSize(8)
	shotty->setMagSize(8);
	shotty->setResetTime(400);
	shotty->setReloadTime(1500);
	shotty->setPellets(6);
	shotty->setSpread(.3f);

	//L1 : Pellets+
	if (level > 0) { shotty->setPellets(8); }

	//L2 : Spread-
	if (level > 1) { shotty->setSpread(.2f); }

	//L3 : RefireTime-
	if (level > 2) { shotty->setResetTime(200); }

	//L4 : Mag+
	if (level > 3) { shotty->setMagSize(12, true); }

	//L5 : Pellets+
	if (level > 4) { shotty->setPellets(10); }

	//L6 : Spread-
	if (level > 5) { shotty->setSpread(.15f); }

	//L7 : RefireTime-
	if (level > 6) { shotty->setResetTime(100); }

	//L8 : Mag+
	if (level > 7) { shotty->setMagSize(16, true); }

	return static_cast<Weapon::WeaponI*>(shotty);
}