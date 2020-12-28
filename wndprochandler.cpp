#include "wndprochandler.h"
#include "inputhandler.h"
#include "moleculeobject.h"
#include "globalstate.h"
#include <windowsx.h>

void processmessage::LButtonDown(WPARAM wparam, LPARAM lparam)
{
	cdinput.LMBclickPosX = cdinput.mousePosX;
	cdinput.LMBclickPosY = cdinput.mousePosY;
	cdinput.LMBisdown = true;
	cdinput.LMBwasclicked = true;
}

void processmessage::LButtonUp(WPARAM wparam, LPARAM lparam)
{
	cdinput.LMBisdown = false;
	cdinput.LMBwasreleased = true;
}

void processmessage::MouseMove(WPARAM wparam, LPARAM lparam)
{
	float previousPosX = cdinput.mousePosX;
	float previousPosY = cdinput.mousePosY;

	cdinput.mousePosX = GET_X_LPARAM(lparam);
	cdinput.mousePosY = GET_Y_LPARAM(lparam);

	int mouseoffset = (cdinput.mousePosX - cdinput.scrollPosX) * (1.0f / cdinput.zoomlevel);
	cdinput.mousePosX = cdinput.rendertargetCX + mouseoffset;

	mouseoffset = (cdinput.mousePosY - cdinput.scrollPosY) * (1.0f / cdinput.zoomlevel);
	cdinput.mousePosY = cdinput.rendertargetCY + mouseoffset;

	cdinput.mouseDeltaX = cdinput.mousePosX - previousPosX; //FIX! : check if mousedelta at various zoomlevel is okay.
	cdinput.mouseDeltaY = cdinput.mousePosY - previousPosY;
}

void processmessage::CalculateSnapPosition() //FIX! If bounding box of molecules are overlapping, mouse will only snap to last found
{
	for (int i = 0; i < molekyler.size(); i++)
	{
		cdglobalstate.objectishovered = false;

		if (molekyler.at(i)->IsMoleculeHovered(cdinput.mousePosX, cdinput.mousePosY))
		{
			cdglobalstate.objectishovered = true;
			cdglobalstate.hoveredobject = i;
			//std::pair<bool, int> hover = molekyler.at(i)->HoveredElement(cdinput.mousePosX, cdinput.mousePosY);
			std::pair<int,float> closestelement = molekyler.at(i)->ClosestElement(cdinput.mousePosX, cdinput.mousePosY);

			//If distance from mousecursor to closest element is less than 5 pixels, set snapmouse
			if (closestelement.second < 5) //FIX! needs adjustable snap limit
			{
				cdinput.snapmousePosX = molekyler.at(i)->GetElementPosition(closestelement.first).x;
				cdinput.snapmousePosY = molekyler.at(i)->GetElementPosition(closestelement.first).y;

				cdglobalstate.hoveredatom = closestelement.first;
				cdglobalstate.atomishovered = true;
			}
			else
			{
				cdinput.snapmousePosX = cdinput.mousePosX;
				cdinput.snapmousePosY = cdinput.mousePosY;

				cdglobalstate.atomishovered = false;
			}
		}
	}
}