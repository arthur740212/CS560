#ifndef SPEED_TIME_H
#define SPEED_TIME_H

#include <cmath>
class SpeedTime 
{
public:
	
	SpeedTime(float _t1, float _t2, float _tmax);

	void CalculateVmax();

	float getSpeedFromDeltaTime(float input);
	float getArcRatioFromDeltaTime(float input);

	float t1;
	float t2;
	float tmax;
	float vmax;
};
#endif
