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
	if (level > 1) { shard.lifeTime = 25; }

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
	pellet.damage = 2;
	pellet.lifeTime = 150;

	//L1 : Damage+
	if (level > 0) { pellet.damage = 4; }

	//L2 : Life+
	if (level > 1) { pellet.lifeTime = 200; }

	//L3 : Speed+
	if (level > 2) { pellet.velScale = 1.25f; }

	//L4 : HP+
	if (level > 3) { pellet.hpMAX = 2; }

	//L5 : Damage++
	if (level > 4) { pellet.damage = 6; }

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
	ProjectileDef ninmil = ProjectileDef();

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

ProjectileDef Weapon::Armory::getDumDum(int level)
{
	//L0 : Default
	ProjectileDef ninmil = ProjectileDef();

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

ProjectileDef Weapon::Armory::getCannonball(int level)
{
	//L0 : Default
	ProjectileDef ninmil = ProjectileDef();

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

ProjectileDef Weapon::Armory::getGrenade(int level)
{
	//L0 : Default
	ProjectileDef ninmil = ProjectileDef();

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

ProjectileDef Weapon::Armory::getRocket(int level)
{
	//L0 : Default
	ProjectileDef ninmil = ProjectileDef();

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

ProjectileDef Weapon::Armory::getLaser(int level)
{
	//L0 : Default
	ProjectileDef ninmil = ProjectileDef();

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

ProjectileDef Weapon::Armory::getSlug(int level)
{
	//L0 : Default
	ProjectileDef slug = ProjectileDef();

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
	Weapon::Shotgun* shotty = new Weapon::Shotgun(fireCallback_, getPellet(projLevel));
	//Base on creation: //Refire = 600 //Reload = 1250 //Pellets = 8; //Spread =  .15f; //MagSize(8)
	shotty->setMagSize(8);
	shotty->setRefireTime(750);
	shotty->setReloadTime(1500);
	shotty->setPellets(6);
	shotty->setSpread(.3f);

	//L1 : Pellets+
	if (level > 0) { shotty->setPellets(8); }

	//L2 : Spread-
	if (level > 1) { shotty->setSpread(.2f); }

	//L3 : RefireTime-
	if (level > 2) { shotty->setRefireTime(600); }

	//L4 : Mag+
	if (level > 3) { shotty->setMagSize(12, true); }

	//L5 : Pellets+
	if (level > 4) { shotty->setPellets(10); }

	//L6 : Spread-
	if (level > 5) { shotty->setSpread(.15f); }

	//L7 : RefireTime-
	if (level > 6) { shotty->setRefireTime(400); }

	//L8 : Mag+
	if (level > 7) { shotty->setMagSize(16, true); }

	return static_cast<Weapon::WeaponI*>(shotty);
}