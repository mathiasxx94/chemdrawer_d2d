#include "renderhelpers.h"
#include "inputhandler.h"
#include "mathhelpers.h"

#include "Source.h" //FIX! maybe not needed if remove d2d1point

#include <iostream>




void GradientLine(Vector2D start, Vector2D end, float startalpha, float endalpha, float thickness)
{
	if (abs(endalpha - startalpha) < 0.05)
	{
		float averagealpha = (startalpha + endalpha) / 2.0f;
		m_pBlackBrush->SetOpacity(averagealpha);
		m_pRenderTarget->DrawLine(D2D1::Point2F(start.x, start.y), D2D1::Point2F(end.x, end.y), m_pBlackBrush);
	}
	else
	{
		float brushmultiple = 1.0f / (endalpha - startalpha); 
		Vector2D brushvector = (end - start) * brushmultiple;

		Vector2D endpoint = end + (brushvector * (1 - endalpha));

		Vector2D startpoint = start - (brushvector * startalpha);

		m_pLinearGradientBrush->SetStartPoint(D2D1::Point2F(startpoint.x, startpoint.y));
		m_pLinearGradientBrush->SetEndPoint(D2D1::Point2F(endpoint.x, endpoint.y));

		m_pRenderTarget->DrawLine(D2D1::Point2F(start.x, start.y), D2D1::Point2F(end.x, end.y), m_pLinearGradientBrush, thickness);
	}
}


void PreviewLine()
{
	//check selected tool first, return if tool is wrong.
	//check bounding box of objects
	//check if mousepos is at object node

	if (!cdinput.LMBisdown) return;

	D2D1_POINT_2F startpos = { cdinput.LMBclickPosXSnap, cdinput.LMBclickPosYSnap };
	D2D1_POINT_2F endpos = { cdinput.snapmouseTargetX, cdinput.snapmouseTargetY };
	
	m_pRenderTarget->DrawLine(startpos, endpos, m_pBlackBrush, 1.32f);
	
}

