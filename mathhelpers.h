#pragma once

namespace mathhelp
{
	float DegToRad(const float deg);
	float RadToDeg(const float rad);
	int AngleToQuadrant(const float radangle);
	float NormalizeAngle0To2Pi(float angle);
}