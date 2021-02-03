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

int mathhelp::AngleToQuadrant(float radangle)
{
	if (radangle >= 0 && radangle <= M_PI_2) return 0;
	else if (radangle > M_PI_2&& radangle <= M_PI) return 1;
	else if (radangle > M_PI&& radangle <= ((2 * M_PI) - M_PI_2)) return 2;
	else return 3;

	//if (radangle >= 0)
	//{
	//	if (radangle <= M_PI_2)
	//	{
	//		return 0;
	//	}
	//	return 1;
	//}
	//if (radangle >= -M_PI_2)
	//	return 3;
	//return 2;
}

float mathhelp::NormalizeAngle0To2Pi(float angle)
{
	//Returns angles 
	//if (angle >= 0) return std::fmodf(angle, 2*M_PI);
	//else
	//{
	//	angle += (2*M_PI);
	//	return std::fmodf(angle, 2*M_PI);
	//}

	if (angle >= 0)
	{
		angle = std::fmodf(angle, 2 * M_PI);
	}
	else
	{
		angle += (2*M_PI);
		angle = std::fmodf(angle, 2*M_PI);
	}

	if (std::abs(angle - (2 * M_PI)) < 0.0001) return 0;
	return angle;
}