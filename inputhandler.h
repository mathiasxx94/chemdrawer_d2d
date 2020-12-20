#pragma once

struct Input
{
	float mousePosX{ 0 };
	float mousePosY{ 0 };
	float mouseDeltaX{ 0 };
	float mouseDeltaY{ 0 };

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