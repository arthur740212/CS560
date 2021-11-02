#include "speedtime.h"


SpeedTime::SpeedTime(float _t1, float _t2, float _tmax) :t1(_t1), t2(_t2), tmax(_tmax)
{
	CalculateVmax();
}

void SpeedTime::CalculateVmax()
{
	vmax = 1.0f / (tmax + t2 - t1) * 2.0f;
}

float SpeedTime::getSpeedFromDeltaTime(float input)
{
	if (input < t1) { return vmax / t1 * input; }
	else if (input > t2) { return vmax / (tmax - t2) * (tmax - input); }
	else { return vmax; }
}

float SpeedTime::getArcRatioFromDeltaTime(float input)
{
	if (input < t1) { return vmax / t1 * input * input / 2.0f; }
	else if (input > t2) { return 1.0f - vmax / (tmax - t2) * (tmax - input) * (tmax - input) / 2.0f; }
	else { return vmax * t1 / 2.0f + vmax * (input - t1); }
}