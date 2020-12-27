#pragma once

struct Input
{
	/*
	Position of mouse cursor. Snappos is used to get the position the mouse would be at if it snapped to 
	a nearby element that is drawn on the canvas.
	*/
	float mousePosX{ 0 };
	float mousePosY{ 0 };
	float snapmousePosX{ 0 };
	float snapmousePosY{ 0 };
	float mouseDeltaX{ 0 };
	float mouseDeltaY{ 0 };

	//Handling mouse clicks
	short LMBclickPosX{ 0 };
	short LMBclickPosY{ 0 };
	short LMBreleasePosX{ 0 };
	short LMBreleasePosY{ 0 };
	bool LMBisdown{ false };
	bool LMBwasclicked{ false }; //Should only be valid for one frame
	bool LMBwasreleased{ false };

	//Handling mousewheel for application zooming, check code in wndproc. Only zooming while CTRL is pressed.
	int scrollPosX;
	int scrollPosY;
	int scrolldelta;
	float zoomlevel{ 1 };

	//Rendertarget
	int rendertargetCX; //Center of rendertarget
	int rendertargetCY; 
};

extern Input cdinput;

