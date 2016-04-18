#ifndef MS_ENEMY_HPP
#define MS_ENEMY_HPP

#include <list>
#include <functional>

#include "Shape.hpp"
#include "EnemyDef.hpp"


class Enemy : public Shape
{
public:
	Enemy(b2Body* body, const ShapeDef& def, std::function<void(SideDef&)>& callback, std::function<Shape*()> &player);
	Enemy(b2Body* body, const EnemyDef& def, std::function<void(SideDef&)>& callback, std::function<Shape*()> &player);

	void update(int milliseconds);

	bool collide(Entity* other, b2Contact& contact, std::string tag);

	//AI Behaviours
	void arrive(b2Vec2 position);
	void seek(b2Vec2 target);
	void flee(b2Vec2 pursuer);

	void pursue(b2Vec2 target);
	void evade(b2Vec2 pursuer);

	//Debug
	int aistate;

private:
	static std::list<Enemy*> others_; //Provision for swarming

	std::function<Shape*()>& getPlayer_;
	
};

#endif