#include "wndprochandler.h"
#include "inputhandler.h"
#include "chemproperties.h"
#include "moleculeobject.h"
#include "globalstate.h"
#include "mathhelpers.h"
#include <windowsx.h>
#include <tuple>
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
	if (!cdglobalstate.atomishovered)
	{
		Molecule* molekyl = new Molecule(Vector2D{cdinput.LMBclickPosXSnap, cdinput.LMBclickPosYSnap},hybridization::sp3);
		molekyler.push_back(molekyl);

		cdglobalstate.editingisactive = true;
		cdglobalstate.newobjectcreated = true;
		cdglobalstate.curreditetobject = molekyler.size();
	}
	else
	{
		cdglobalstate.curreditetobject = cdglobalstate.hoveredobject;
		cdglobalstate.curreditedatom = cdglobalstate.hoveredatom;
	}
}

void processmessage::LButtonUp(WPARAM wparam, LPARAM lparam)
{
	cdinput.LMBisdown = false;
	cdinput.LMBwasreleased = true;
	cdglobalstate.editingisactive = false;

	if (cdglobalstate.newobjectcreated) //FIX! take care of self hovering on the newly created object
	{
		cdglobalstate.newobjectcreated = false;
		molekyler.back()->AddElement(Vector2D{ cdinput.snapmouseTargetX, cdinput.snapmouseTargetY }, hybridization::sp3);
		molekyler.back()->AddBond(0, 1, bond::sb);
	}
	else
	{
		//We are not creating any new atoms if the endpoint is hovering over an existing atom in the molecule
		//In this case we only need a new bond between the starting and end point
		if (cdglobalstate.atomishovered && (cdglobalstate.curreditedatom != cdglobalstate.hoveredatom))
		{
			molekyler.at(cdglobalstate.hoveredobject)->AddBond(cdglobalstate.curreditedatom, cdglobalstate.hoveredatom);
		}
		else
		{
			molekyler.at(cdglobalstate.curreditetobject)->AddElement(Vector2D{ cdinput.snapmouseTargetX,cdinput.snapmouseTargetY }, hybridization::sp3);
			molekyler.at(cdglobalstate.curreditetobject)->AddBond(cdglobalstate.hoveredatom, molekyler.at(cdglobalstate.curreditetobject)->GetNumberOfElements() - 1);
		}
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


void processmessage::KeyBoardKeyDown(WPARAM wparam)
{
	if (wparam == 0x31)
	{
		if (cdglobalstate.bondishovered)
		{
			molekyler[cdglobalstate.hoveredobject]->SetBondType(cdglobalstate.hoveredbond, bond::db);
		}
	}
}

void processmessage::KeyBoardChar(WPARAM wparam, LPARAM lparam)
{

}


//void processmessage::CalculateSnapPosition() //FIX! If bounding box of molecules are overlapping, mouse will only snap to last found
//{
//	int closestobject; //Based on shortest distance from mouse to node out of all objects
//	int closestelementindex;
//	float closestelementdistance = 50000;
//	cdglobalstate.objectishovered = false;
//
//	for (int i = 0; i < molekyler.size(); i++)
//	{
//		if (i == cdglobalstate.curreditetobject && cdglobalstate.editingisactive) continue;
//
//		if (molekyler.at(i)->IsMoleculeHovered(cdinput.mousePosX, cdinput.mousePosY))
//		{
//			cdglobalstate.objectishovered = true;
//
//			std::pair<int, float> closestelement = molekyler.at(i)->ClosestElement(cdinput.mousePosX, cdinput.mousePosY);
//			if (closestelementdistance > closestelement.second)
//			{
//				closestelementdistance = closestelement.second;
//				cdglobalstate.hoveredobject = i;
//				closestelementindex = closestelement.first;
//				closestobject = i;
//			}
//		}	
//	}
//
//	//If distance from mousecursor to closest element is less than 5 pixels, set snapmouse
//	if (closestelementdistance < 10.0f) //FIX! needs adjustable snap limit
//	{
//		cdinput.snapmousePosX = molekyler.at(closestobject)->GetElementPosition(closestelementindex).x;
//		cdinput.snapmousePosY = molekyler.at(closestobject)->GetElementPosition(closestelementindex).y;
//
//		cdglobalstate.hoveredatom = closestelementindex;
//		cdglobalstate.atomishovered = true;
//	}
//	else
//	{
//		cdinput.snapmousePosX = cdinput.mousePosX;
//		cdinput.snapmousePosY = cdinput.mousePosY;
//
//		cdglobalstate.atomishovered = false;
//	}
//}

void processmessage::CalculateSnapPosition() //FIX! If bounding box of molecules are overlapping, mouse will only snap to last found
{
	int closestobject; //Based on shortest distance from mouse to node out of all objects
	int closestelementindex;
	float closestelementdistance = 50000;
	bool whatishovered;
	cdglobalstate.objectishovered = false;

	for (int i = 0; i < molekyler.size(); i++)
	{
		if (i == cdglobalstate.curreditetobject && cdglobalstate.editingisactive) continue;

		if (molekyler.at(i)->IsMoleculeHovered(cdinput.mousePosX, cdinput.mousePosY))
		{
			cdglobalstate.objectishovered = true;

			//std::pair<int, float> closestelement = molekyler.at(i)->ClosestElement(cdinput.mousePosX, cdinput.mousePosY);
			std::tuple<bool, int, float> closestpart = molekyler.at(i)->ClosestPart(cdinput.mousePosX, cdinput.mousePosY);

			if (closestelementdistance > std::get<2>(closestpart))
			{
				closestelementdistance = std::get<2>(closestpart);
				cdglobalstate.hoveredobject = i;
				closestelementindex = std::get<1>(closestpart);
				closestobject = i;
				whatishovered = std::get<0>(closestpart);
			}
		}
	}

	//If distance from mousecursor to closest element is less than 5 pixels, set snapmouse
	if (closestelementdistance < 10.0f) //FIX! needs adjustable snap limit
	{
		if (whatishovered)
		{
			cdinput.snapmousePosX = molekyler.at(closestobject)->GetElementPosition(closestelementindex).x;
			cdinput.snapmousePosY = molekyler.at(closestobject)->GetElementPosition(closestelementindex).y;

			cdglobalstate.hoveredatom = closestelementindex;
			cdglobalstate.atomishovered = true;
			cdglobalstate.bondishovered = false;
		}
		else
		{
			cdinput.snapmousePosX = molekyler.at(closestobject)->GetBondPosition(closestelementindex).x;
			cdinput.snapmousePosY = molekyler.at(closestobject)->GetBondPosition(closestelementindex).y;

			cdglobalstate.hoveredbond = closestelementindex;
			cdglobalstate.bondishovered = true;
			cdglobalstate.atomishovered = false;
		}
		
	}
	else
	{
		cdinput.snapmousePosX = cdinput.mousePosX;
		cdinput.snapmousePosY = cdinput.mousePosY;

		cdglobalstate.atomishovered = false;
		cdglobalstate.bondishovered = false;
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