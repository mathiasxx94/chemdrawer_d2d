#include "wndprochandler.h"
#include "inputhandler.h"
#include "moleculeobject.h"
#include "globalstate.h"

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
	cdinput.LMBisdown = false;
	cdinput.LMBwasreleased = true;
}

void processmessage::CalculateSnapPosition()
{
	for (int i = 0; i < molekyler.size(); i++)
	{
		cdglobalstate.objectishovered = false;

		if (molekyler.at(i)->IsMoleculeHovered(cdinput.mousePosX, cdinput.mousePosY))
		{
			cdglobalstate.objectishovered = true;
			cdglobalstate.hoveredobject = i;
			std::pair<bool, int> hover = molekyler.at(i)->HoveredElement(cdinput.mousePosX, cdinput.mousePosY);
			if (hover.first)
			{
				cdglobalstate.atomishovered = true;
				cdglobalstate.hoveredatom = hover.second;
			}
			else
			{
				cdglobalstate.atomishovered = false;
			}
		}
	}
}