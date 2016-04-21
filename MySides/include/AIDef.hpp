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

	void validate()
	{
		aggression = fmax(0, fmin(aggression, 4));
		inner_peace = fmax(0, fmin(inner_peace, 4));

		sensitivity = fmax(0, fmin(sensitivity, 4));

		personal_space = fmax(0, fmin(personal_space, 4));
		perserverance = fmax(0, fmin(perserverance, 4));

		friendliness = fmax(0, fmin(friendliness, 4));
		wanderlust = fmax(0, fmin(wanderlust, 4));

		pain_tolerance = fmax(0, fmin(pain_tolerance, 4));
		weapon_training = fmax(0, fmin(weapon_training, 4));

		evasiveness = fmax(0, fmin(evasiveness, 4));
		paranoia = fmax(0, fmin(paranoia, 4));

		dancer = fmax(0, fmin(dancer, 4));
	}

	int surroundings; //Bounds radius
	
	int aggression; //Angry Rate/effects 0 to 4
	int inner_peace; //Chill rate/effects 0 to 4

	int sensitivity; //Sense radius 0 to 4

	int personal_space; // ChaseMin 0 to 4
	int perserverance; //ChaseMAX 0 to 4

	int friendliness; //Flock Same, 0 to 4
	int wanderlust; //Speed/rate of wander 0 to 4

	int pain_tolerance; //How much we can be hurt before we're scared 0 to 4
	int weapon_training; //Burst length/accuracy 0 to 4

	int evasiveness; //Panic start, 0 to 4
	int paranoia; //Panic finish 0 to 4

	bool CCW; //Are we a lefty?
	int dancer; //type of dance 0 to 4
	
	static AIDef testDef()
	{
		AIDef base = AIDef();
		base.aggression = 4;
		base.inner_peace = 2;

		base.sensitivity = 4;
		
		base.personal_space = 4;
		base.perserverance = 3;
		
		base.friendliness = 4;
		base.wanderlust = 0;
		
		base.pain_tolerance = 1;
		base.weapon_training = 3;
		
		base.evasiveness = 4;
		base.paranoia = 4;

		base.dancer = 3;
		return base;
	}

	static AIDef badDef()
	{
		AIDef bad = AIDef();

		bad.aggression = -1;
		bad.inner_peace = 30;

		bad.sensitivity = 2;

		bad.personal_space = 142;
		bad.perserverance = 2;

		bad.friendliness = 3;
		bad.wanderlust = 14;

		bad.pain_tolerance = 2;
		bad.weapon_training = 7;

		bad.evasiveness = 2;
		bad.paranoia = FLT_MAX;

		bad.dancer = 5;
		return bad;
	}

	static AIDef maxDef()
	{
		AIDef big = AIDef();

		big.aggression = 4;
		big.inner_peace = 4;

		big.sensitivity = 4;

		big.personal_space = 4;
		big.perserverance = 4;

		big.friendliness = 4;
		big.wanderlust = 4;

		big.pain_tolerance = 4;
		big.weapon_training = 4;

		big.evasiveness = 4;
		big.paranoia = 4;

		big.dancer = 4;
		return big;
	}


};

#endif