#ifndef MS_ENEMY_HPP
#define MS_ENEMY_HPP

#include "Shape.hpp"
#include <list>
#include <functional>

class Enemy : public Shape
{
public:
	Enemy(b2Body* body,const ShapeDef& def, std::function<void(SideDef&)>& callback, std::function<Shape*()> &player);

	void update(int milliseconds);
	void setCollector(bool collect);

	bool collide(Entity* other, b2Contact& contact, std::string tag);

	//Debug
	int aistate;

private:
	static std::list<Enemy*> others_; //Provision for swarming

	std::function<Shape*()>& getPlayer_;
	
};

#endif