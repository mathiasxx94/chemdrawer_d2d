#include "renderhelpers.h"
#include "Source.h"


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