#include "Armory.hpp"

Weapon::Armory::Armory(std::function<void(std::vector<ProjectileDef>&defs, std::string id)> fireCallback)
{
	fireCallback_ = fireCallback;
}

ProjectileDef Weapon::Armory::getPellet(int level)
{
	//L0 : Default
	ProjectileDef pellet = ProjectileDef();
	pellet.velScale = 1;
	pellet.hpMAX = 1;
	pellet.width = 0.25f;
	pellet.damage = 1;
	pellet.lifeTime = 150;

	//L1 : Range+
	if (level > 0) { pellet.lifeTime = 200; }

	//L2 : Damage+
	if (level > 1) { }

	//L3 : 
	if (level > 2) { }

	//L4 : Damage+
	if (level > 3) { }

	//L5 : Speed+
	if (level > 4) { }

	//L6 : 
	if (level > 5) { }

	//L7 : HP+
	if (level > 6) { }

	//L8 : Size+
	if (level > 7) { }

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

	return static_cast<Weapon::WeaponI*>(shotty);
}