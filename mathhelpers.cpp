#include "mathhelpers.h"
#define _USE_MATH_DEFINES // for C++
#include <cmath>

float mathhelp::RadToDeg(float rad)
{
	float deg = (rad / (M_PI)) * 180;
	return deg;
}

float mathhelp::DegToRad(float deg)
{
	float rad = M_PI * deg / 180;
	return rad;
}