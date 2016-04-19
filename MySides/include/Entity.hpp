#ifndef MS_ENTITY_HPP
#define MS_ENTITY_HPP

#include <stdafx.h>
#include <Box2D\Box2D.h>

//!	@class	Entity
//!	@brief	Base Game Entity class

//!		Base class includes box2d body, active and alive status,
//!		movement and rotation, collision hooks and colours.

class Entity {
public:
	Entity(b2Body * body);
	~Entity();

	enum Faction {
		NEUTRAL = 0,
		FRIEND = 1,
		ENEMY = 2,
		GOD = 3
	};

	virtual void setPosition(b2Vec2 position);	//!< Directly sets position of b2Body
	virtual void setRotation(float amount);		//!< Directly sets rotation of b2Body

	b2Vec2 getPosition() const;	//!< Gets position of b2Body
	float getRotation() const;	//!< Gets rotation of b2Body

	b2Vec2 getVelocity() const; //!< Gets velocity of b2Body

	b2Body* getBody() const;	//!< Gets a pointer to attached b2Body

	bool getAlive() const;	//!< Gets alive status
	bool getActive() const;	//!< Gets active status

	void setAlive(bool a);	//!< Sets alive status
	void setActive(bool a);	//!< Sets active status

	void setFaction(int faction);//!< Sets faction
	int getFaction() const; //!< Gets faction

	b2Color getPrimary() const;
	b2Color getSecondary() const;
	b2Color getTertiary() const;

	void setPrimary(b2Color col);
	void setSecondary(b2Color col);
	void setTertiary(b2Color col);

	virtual void kill();

	virtual bool collide(Entity* other, b2Contact& contact, std::string tag); //!< Collision with other entities. 
protected:

	b2Body * body_;

	bool alive_;
	bool active_;

	b2Color colPrim_;
	b2Color colSecn_;
	b2Color colTert_;

	int faction_;
	std::string enemyTag_;
private:
};

#endif