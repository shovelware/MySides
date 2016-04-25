// WhiteBox.cpp : Defines the entry point for the console application.
//

#include <assert.h>
#include <iostream>

bool alive = false;

int uhp = 0;
int uhpScale = 0;
int uhpMAX = 0;

int vertices = 2;
int verticesMIN = 2;

int hp = 0;
int hpScale = 5;
int hpMAX = 0;

void setupTestuhp()
{
	alive = true;

	uhp = 4;
	uhpScale = 5;
	uhpMAX = 25;

	vertices = 4;
	verticesMIN = 2;

	hp = 3;
	hpScale = 5;
	hpMAX = 20;
}

void setupTesthp()
{
	alive = true;

	uhp = 0;
	uhpScale = 5;
	uhpMAX = 25;

	vertices = 4;
	verticesMIN = 2;

	hp = 3;
	hpScale = 5;
	hpMAX = 20;
}

void setupTestFull()
{
	alive = true;

	uhp = 90;
	uhpScale = 10;
	uhpMAX = 90;

	vertices = 8;
	verticesMIN = 2;

	hp = 80;
	hpScale = 10;
	hpMAX = 80;
}

void setupTestDead()
{
	alive = false;

	uhp = 0;
	uhpScale = 10;
	uhpMAX = 40;

	vertices = 8;
	verticesMIN = 2;

	hp = 0;
	hpScale = 10;
	hpMAX = 30;
}

void takeDamage(int damage)
{
	std::cout << "DMG " << damage << "\n==========" << std::endl;

	if (!alive)
	{
		assert(alive == false);
		return;
	}

	for (int dmg = damage; dmg > 0; --dmg)
	{
		assert(dmg > 0);

		//If we're overhealed
		if (uhp > 0)
		{
			uhp -= 1;
			assert(uhp >= 0);
		}

		//If we're about to go down, go back a side
		else if (hp == 1)
		{
			hp -= 1;

			assert(hp == 0);

			vertices = (vertices - 1 >= verticesMIN ? vertices - 1 : verticesMIN);

			if (vertices > verticesMIN)
			{
				assert(vertices != verticesMIN);

				//Reset health
				hpMAX = vertices * hpScale;
				hp = hpMAX;

				//Re-enable upgrades if we must
				uhpMAX = (vertices + 1) * uhpScale;
			}
		}

		//Otherwise just take some hp
		else if (vertices > verticesMIN)
		{
			hp -= 1;
			assert(hp > 0);
		}

		//We're dead, stop hurting
		if (vertices <= verticesMIN && hp <= 0 && uhp <= 0)
		{
			break;
		}
	}//End for

	 //HP should be refilled in loop, this means we're actually dead
	if (hp <= 0)
	{
		assert(vertices <= verticesMIN);
		assert(uhp <= 0);

		alive = false;
	}
}

void writeStatus()
{

	std::cout << "\nA\t" << alive << "\n" <<
		"\nU\t" << uhp << "/" << uhpMAX << "(" << uhpScale << ")" <<
		"\nH\t" << hp << "/" << hpMAX << "(" << hpScale << ")" <<
		"\nV\t" << vertices << "/" << verticesMIN <<
		"\n==========" <<
		std::endl;
}

int main()
{
	std::cout << "Take damage while dead" << std::endl;
	writeStatus();
	takeDamage(4);
	writeStatus();

	std::cout << "Take negative damage while dead" << std::endl;
	writeStatus();
	takeDamage(-4);
	writeStatus();

	std::cout << "Take uhp damage while alive" << std::endl;
	setupTestuhp();
	writeStatus();
	takeDamage(40);
	writeStatus();

	std::cout << "Take damage from alive hp full to 0" << std::endl;
	setupTesthp();
	writeStatus();
	takeDamage(999);
	writeStatus();

	std::cout << "Take negative damage while alive" << std::endl;
	setupTesthp();
	writeStatus();
	takeDamage(-40);
	writeStatus();

	std::cout << "Take negative damage while alive and uhp" << std::endl;
	setupTestuhp();
	writeStatus();
	takeDamage(-10);
	writeStatus();

	std::cin.get();
}