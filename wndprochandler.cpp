#include "wndprochandler.h"
#include "inputhandler.h"
#include "chemproperties.h"
#include "moleculeobject.h"
#include "globalstate.h"
#include "mathhelpers.h"
#include <windowsx.h>
#include "Source.h"

void processmessage::LButtonDown(WPARAM wparam, LPARAM lparam)
{
	cdinput.LMBclickPosX = cdinput.mousePosX;
	cdinput.LMBclickPosY = cdinput.mousePosY;

	if (cdglobalstate.atomishovered)
	{
		cdinput.LMBclickPosXSnap = cdinput.snapmousePosX;
		cdinput.LMBclickPosYSnap = cdinput.snapmousePosY;
	}
	else
	{
		cdinput.LMBclickPosXSnap = cdinput.mousePosX;
		cdinput.LMBclickPosYSnap = cdinput.mousePosY;
	}

	cdinput.LMBisdown = true;
	cdinput.LMBwasclicked = true;

	//Create a new Molecule object if no molecule is already hovered
	if (!cdglobalstate.objectishovered)
	{
		Molecule* molekyl = new Molecule(Vector2D{cdinput.LMBclickPosXSnap, cdinput.LMBclickPosYSnap},hybridization::sp3);
		molekyler.push_back(molekyl);
	}
}

void processmessage::LButtonUp(WPARAM wparam, LPARAM lparam)
{
	cdinput.LMBisdown = false;
	cdinput.LMBwasreleased = true;

	if (!cdglobalstate.objectishovered) //FIX! take care of self hovering on the newly created object
	{
		molekyler.back()->AddElement(Vector2D{ cdinput.snapmouseTargetX, cdinput.snapmouseTargetY }, hybridization::sp3);
	}
}

void processmessage::MouseMove(WPARAM wparam, LPARAM lparam)
{
	float previousPosX = cdinput.mousePosX;
	float previousPosY = cdinput.mousePosY;
	
	cdinput.mousePosX = GET_X_LPARAM(lparam);
	cdinput.mousePosY = GET_Y_LPARAM(lparam);

	// Mousepos will be the same regardless of zoom level if mouse is hovering over the same object.
	int mouseoffset = (cdinput.mousePosX - cdinput.scrollPosX) * (1.0f / cdinput.zoomlevel);
	cdinput.mousePosX = cdinput.rendertargetCX + mouseoffset;

	mouseoffset = (cdinput.mousePosY - cdinput.scrollPosY) * (1.0f / cdinput.zoomlevel);
	cdinput.mousePosY = cdinput.rendertargetCY + mouseoffset;

	cdinput.mouseDeltaX = cdinput.mousePosX - previousPosX; //FIX! : check if mousedelta at various zoomlevel is okay.
	cdinput.mouseDeltaY = cdinput.mousePosY - previousPosY; //do i really need mouse delta for anything???

	

	processmessage::CalculateSnapPosition(); 
	processmessage::CalculatePreviewLineEndPoint(wparam, lparam);
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
			if (closestelement.second < 5.0f) //FIX! needs adjustable snap limit
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

			break; //FIX! remove break when overlapping molecule bounding boxes are fixed
		}
	}
}

void processmessage::CalculatePreviewLineEndPoint(WPARAM wparam, LPARAM lparam)
{
	if (!cdinput.LMBisdown) return; //FIX!!! remember to edit this if function if used for other things than previewline

	Vector2D startpos = { cdinput.LMBclickPosXSnap, cdinput.LMBclickPosYSnap };
	//Vector2D startpos = { cdinput.snapmousePosX, cdinput.snapmousePosY };
	Vector2D endpos = { cdinput.mousePosX, cdinput.mousePosY };

	//Get delta angle between current mousepos and LMBclickpos
	float angle = atan2(-(endpos.y - startpos.y), endpos.x - startpos.x);

	// Angle snapping can be freed by holding ctrl
	if (!(wparam & MK_CONTROL))
	{
		float iangle = mathhelp::RadToDeg(abs(angle)) + (15);
		iangle -= std::fmodf(iangle, 30);
		angle = mathhelp::DegToRad(iangle) * (angle > 0 ? 1 : -1);
	}

	// Line length 
	int linelength = 40; //FIX! use global line length from settings
	if (wparam & MK_SHIFT)
	{
		linelength = (endpos - startpos).length();
	}

	endpos.x = linelength * std::cosf(angle) + startpos.x;
	endpos.y = -linelength * std::sinf(angle) + startpos.y;


	cdinput.snapmouseTargetX = endpos.x;
	cdinput.snapmouseTargetY = endpos.y;
}