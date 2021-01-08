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

//void WedgedBondDraw(Vector2D start, Vector2D end)
//{
//	HRESULT hr;
//	ID2D1GeometrySink* pSink = NULL;
//	ID2D1PathGeometry* m_pPathGeometry;
//
//	Vector2D target;
//	float offsetangle = mathhelp::DegToRad(5);
//	float linelength = (end-start).length();
//	linelength /= std::cosf(offsetangle);
//	float angle = atan2(-(end.y - start.y), end.x - start.x);
//	target.x = linelength * std::cosf(angle - offsetangle) + start.x;
//	target.y = -linelength * std::sinf(angle - offsetangle) + start.y;
//
//	hr = m_pD2DFactory->CreatePathGeometry(&m_pPathGeometry);
//	hr = m_pPathGeometry->Open(&pSink);
//	pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
//	pSink->BeginFigure(D2D1::Point2F(start.x, start.y), D2D1_FIGURE_BEGIN_FILLED);
//	pSink->AddLine(D2D1::Point2F(target.x, target.y));
//	target.x = linelength * std::cosf(angle + offsetangle) + start.x;
//	target.y = -linelength * std::sinf(angle + offsetangle) + start.y;
//	pSink->AddLine(D2D1::Point2F(target.x, target.y));
//	pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
//	hr = pSink->Close();
//	SafeRelease(&pSink);
//
//	m_pRenderTarget->FillGeometry(m_pPathGeometry, m_pBlackBrush);
//	m_pPathGeometry->Release();
//}

void WedgedBondDraw(Vector2D start, Vector2D end)
{
	HRESULT hr;
	ID2D1GeometrySink* pSink = NULL;
	ID2D1PathGeometry* m_pPathGeometry;

	Vector2D target;
	int startoffset = 1;
	int endoffset = 5;
	Vector2D vector = end - start;
	Vector2D normalvector{ vector.y, -vector.x };
	normalvector = normalvector / vector.length();

	target = start - (normalvector * startoffset);

	hr = m_pD2DFactory->CreatePathGeometry(&m_pPathGeometry);
	hr = m_pPathGeometry->Open(&pSink);
	pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
	pSink->BeginFigure(D2D1::Point2F(target.x, target.y), D2D1_FIGURE_BEGIN_FILLED);
	target = end - (normalvector * endoffset);
	pSink->AddLine(D2D1::Point2F(target.x, target.y));
	target = end + (normalvector * endoffset);
	pSink->AddLine(D2D1::Point2F(target.x, target.y));
	target = start + (normalvector * startoffset);
	pSink->AddLine(D2D1::Point2F(target.x, target.y));
	
	pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	hr = pSink->Close();
	SafeRelease(&pSink);

	m_pRenderTarget->FillGeometry(m_pPathGeometry, m_pBlackBrush); 
	m_pPathGeometry->Release();
}

void DashedBondDraw(Vector2D start, Vector2D end)
{
	//Draw dashed bond by drawing along normal vector at end position, walk towards start.
	//Test 5 pixels between dashes

	int startoffset = 1;
	int endoffset = 5;

	Vector2D directionunitvector = end - start;
	float bondlength = directionunitvector.length();
	int numberofwedges = bondlength / 5;
	float wedgeoffsetinc = 5.0f / numberofwedges;
	directionunitvector = directionunitvector / directionunitvector.length();
	Vector2D normalunitvector{ directionunitvector.y, -directionunitvector.x };

	Vector2D target1, target2;

	for (int n = 1; n <= numberofwedges; n++)
	{
		target1 = start + (directionunitvector * endoffset*n) - (normalunitvector * wedgeoffsetinc * n);
		target2 = start + (directionunitvector * endoffset*n) + (normalunitvector * wedgeoffsetinc * n);
		m_pRenderTarget->DrawLine(D2D1::Point2F(target1.x, target1.y), D2D1::Point2F(target2.x, target2.y), m_pBlackBrush, 1.32f);
	}
}