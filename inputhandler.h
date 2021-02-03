#pragma once

struct Input
{
	/*
	Position of mouse cursor. Snappos is used to get the position the mouse would be at if it snapped to 
	a nearby element that is drawn on the canvas.
	*/
	volatile float mousePosX{ 0 };
	volatile float mousePosY{ 0 };
	volatile float snapmousePosX{ 0 };
	volatile float snapmousePosY{ 0 };
	volatile float snapmouseTargetX{ 0 };
	volatile float snapmouseTargetY{ 0 };
	volatile float mouseDeltaX{ 0 };
	volatile float mouseDeltaY{ 0 };

	//Handling mouse clicks
	volatile float LMBclickPosX{ 0 };
	volatile float LMBclickPosY{ 0 };
	volatile float LMBclickPosXSnap{ 0 };
	volatile float LMBclickPosYSnap{ 0 };
	volatile float LMBreleasePosX{ 0 };
	volatile float LMBreleasePosY{ 0 };
	volatile bool LMBisdown{ false };
	volatile bool LMBwasclicked{ false }; //Should only be valid for one frame
	volatile bool LMBwasreleased{ false };

	//Handling mousewheel for application zooming, check code in wndproc. Only zooming while CTRL is pressed.
	volatile int scrollPosX;
	volatile int scrollPosY;
	volatile int scrolldelta;
	volatile float zoomlevel{ 1 };

	//Rendertarget
	volatile int rendertargetCX; //Center of rendertarget
	volatile int rendertargetCY; 

	//Createmolecule properties
	float bondangle; //FIX!! not really needed?
};

extern Input cdinput;

