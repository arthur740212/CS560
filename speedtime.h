#ifndef SPEED_TIME_H
#define SPEED_TIME_H

#include <cmath>
class SpeedTime 
{
public:
	//Create speedtime function with given parameters tmax >= t2 >= t1>= 0
	SpeedTime(float _t1, float _t2, float _tmax);

	//calcalates the vmax 
	void CalculateVmax();

	//returns the velocity at a given time, works with animPace to modify animation speed
	float getSpeedFromDeltaTime(float input);

	//returns the ratio of the arclength at a given time
	float getArcRatioFromDeltaTime(float input);

	float t1;
	float t2;
	float tmax;
	float vmax;
};
#endif
