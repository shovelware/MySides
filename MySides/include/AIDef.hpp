#ifndef MS_AIDEF_HPP
#define MS_AIDEF_HPP

#include <Box2D\Box2D.h>

class AIDef {
public:
	AIDef() :
		
		aggression(0),
		inner_peace(0),
		sensitivity(0),
		personal_space(0),
		perserverance(0),
		friendliness(0),
		wanderlust(0),
		pain_tolerance(0),
		weapon_training(0),
		evasiveness(0),
		paranoia(0),
		CCW(false),
		dancer(0)
	{}

	AIDef(const AIDef& aid) :
		aggression(aid.aggression),
		inner_peace(aid.inner_peace),
		sensitivity(aid.sensitivity),
		personal_space(aid.personal_space),
		perserverance(aid.perserverance),
		friendliness(aid.friendliness),
		wanderlust(aid.wanderlust),
		pain_tolerance(aid.pain_tolerance),
		weapon_training(aid.weapon_training),
		evasiveness(aid.evasiveness),
		paranoia(aid.paranoia),
		CCW(aid.CCW),
		dancer(aid.dancer)
	{}

	int surroundings; //Bounds radius
	
	int aggression; //Angry Rate/effects 0 to 4
	int inner_peace; //Chill rate/effects 0 to 4

	float sensitivity; //Sense radius

	int personal_space; // ChaseMin 0 to 4
	int perserverance; //ChaseMAX 0 to 4

	int friendliness; //Flock Same, 0 to 4
	int wanderlust; //Speed/rate of wander 0 to 4

	float pain_tolerance; //How much we can be hurt before we're scared 0 to 1f
	int weapon_training; //Burst length/accuracy 0 to 4

	int evasiveness; //Panic start, 0 to 4
	int paranoia; //Panic finish 0 to 4

	bool CCW; //Are we a lefty?
	int dancer; //type of dance 0 to 4
	
	static AIDef testDef()
	{
		AIDef base = AIDef();
		base.aggression = 1;
		base.sensitivity = 2;
		base.personal_space = 2;
		base.perserverance = 2;
		return base;
	}

	static AIDef midDef()
	{
		AIDef mod = AIDef();

		return mod;
	}

	static AIDef maxDef()
	{
		AIDef modmax = AIDef();;

		return modmax;
	}


};

#endif