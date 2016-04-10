#ifndef MS_PROJECTILE_HPP
#define MS_PROJECTILE_HPP

#include "Entity.hpp"
#include "ProjectileDef.hpp"
#include <functional>


typedef std::function<void(b2Vec2 pos, float force, float radius, int time, int faction)> ForceFunc;
class Projectile : public Entity{
public:
	Projectile(b2Body* body, const ProjectileDef& def, ForceFunc& forceCallback);


	void takeDamage(unsigned int damage);
	int getDamage() const;
	ProjectileDef::ProjShrapnel const& getShrapnel() const;

	Entity* getOwner();
	void setOwner(Entity* o);

	b2Vec2 getDirection() const;

	void update(int milliseconds);
	bool collide(Entity* other, b2Contact& contact, std::string tag);

private:
	void setAsCircle(b2Vec2 size, float bounce, bool ghost);
	void setAsRect(b2Vec2 size, float bounce, bool ghost);
	void setTracking(float radius);

	void fire(float mult);
	void track();
	void orient(b2Vec2 target);
	b2Vec3 target_;

	b2Vec2 size_;
	float bounce_;
	float speed_;
	float damage_;
	int penetration_;
	ProjectileDef::ProjDet force_;
	ProjectileDef::ProjShrapnel shrapnel_;
	ProjectileDef::ProjTracking tracking_;
	std::function<void(b2Vec2 pos, float force, float radius, int time, int faction)> forceCallback_;

	Entity* owner_;
	b2Vec2 origin_;
	b2Vec2 heading_;
	b2Vec3 lastPen_;

	int lifeTime_;

	bool fired_;
	bool oneHit_;

	int hp_;
	unsigned int maxHP_;
};

#endif