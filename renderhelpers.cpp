#include "renderhelpers.h"
#include "Source.h"


//void Figures::GradientLine(D2D1_POINT_2F start)
//{
//	if (chemd::m_pRenderTarget)
//	chemd::m_pRenderTarget->DrawLine(D2D1::Point2F(200, 450), D2D1::Point2F(800, 450), chemd::m_pLinearGradientBrush, 2, NULL);
//}


void GradientLine(D2D1_POINT_2F start, D2D1_POINT_2F end, float startalpha, float endalpha, float thickness)
{
	float ratio = endalpha - startalpha;
	m_pLinearGradientBrush->SetStartPoint(start);
	m_pLinearGradientBrush->SetEndPoint(end);
	m_pRenderTarget->DrawLine(start, end, m_pLinearGradientBrush, thickness, NULL);
}