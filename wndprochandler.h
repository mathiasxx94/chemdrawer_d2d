#pragma once
#include <Windows.h>

namespace processmessage
{
	void LButtonDown(WPARAM, LPARAM);
	void LButtonUp(WPARAM, LPARAM);
	void MouseMove(WPARAM, LPARAM);
	void KeyBoardChar(WPARAM, LPARAM);
	void KeyBoardKeyDown(WPARAM);

	void CalculateSnapPosition();							//Pos also used to render overlay when an atom is pointed at, can't optimize out of wm_mousemove
	void CalculatePreviewLineEndPoint(WPARAM, LPARAM);		//
	void CalculateElementEndpoint();						//If mouse wasn't moved between mouse click and release
}

