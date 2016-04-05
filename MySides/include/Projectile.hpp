#ifndef MS_PROJECTILE_HPP
#define MS_PROJECTILE_HPP

#include "Entity.hpp"
#include "ProjectileDef.hpp"
#include <functional>


typedef std::function<void(b2Vec2 pos, float force, float radius, int time)> ForceFunc;
class Projectile : public Entity{
public:
	Projectile(b2Body* body, const ProjectileDef& def, ForceFunc& forceCallback);

	void fire(float mult);

	void takeDamage(unsigned int damage);
	int getDamage() const;
	ProjectileDef::ProjShrapnel const& getShrapnel() const;

	Entity* getOwner();
	void setOwner(Entity* o);

	Entity* getTarget();
	void setTarget(Entity* s);

	b2Vec2 getDirection() const;

	void update(int milliseconds);
	bool collide(Entity* other, b2Contact& contact, std::string tag);

private:
	void setAsCircle(b2Vec2 size, float bounce, bool ghost);
	void setAsRect(b2Vec2 size, float bounce, bool ghost);

	b2Vec2 size_;
	float speed_;
	float damage_;
	int penetration_;
	ProjectileDef::ProjDet force_;
	ProjectileDef::ProjShrapnel shrapnel_;
	std::function<void(b2Vec2 pos, float force, float radius, int time)> forceCallback_;

	Entity* owner_;
	Entity* target_;
	b2Vec2 origin_;
	b2Vec2 heading_;
	b2Vec2 lastPen_;

	int lifeTime_;

	bool fired_;
	bool oneHit_;

	int hp_;
	unsigned int maxHP_;
};

#endif