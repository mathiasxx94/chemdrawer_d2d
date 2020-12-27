#pragma once
#include <Windows.h>

namespace processmessage
{
	void LButtonDown(WPARAM, LPARAM);
	void LButtonUp(WPARAM, LPARAM);
	void MouseMove(WPARAM, LPARAM);

	void CalculateSnapPosition();
}

