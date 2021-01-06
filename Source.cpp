#include "Source.h"
#include <windowsx.h>
#include <chrono>
#include "inputhandler.h"
#include "renderhelpers.h"
#include "moleculeobject.h"
#include "globalstate.h"
#include "wndprochandler.h"
#include "Vector.h"
#include <vector> 

#include <iostream>


Input cdinput;
GlobalState cdglobalstate;

std::vector<Molecule*> molekyler;


ID2D1HwndRenderTarget* m_pRenderTarget;
ID2D1LinearGradientBrush* m_pLinearGradientBrush;
ID2D1SolidColorBrush* m_pBlackBrush;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	AllocConsole(); 
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	std::cout << "This works" << std::endl;

	HRESULT hr;
	hr = chemd::CreateDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{
		//Register window class
		WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = chemd::WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof(LONG_PTR);
		wc.hInstance = GetModuleHandle(NULL);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.lpszClassName = L"D2DChemD";

		RegisterClassExW(&wc);

		//Create application window
		int screensizeX = GetSystemMetrics(SM_CXSCREEN);
		int screensizeY = GetSystemMetrics(SM_CYSCREEN);
		//FLOAT dpi = (FLOAT)GetDpiForWindow(GetDesktopWindow());

		chemd::m_hwnd = CreateWindowW(
			wc.lpszClassName,
			L"ChemD",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			screensizeX,
			screensizeY,
			NULL,
			NULL,
			wc.hInstance, 
			NULL);

		hr = chemd::m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(chemd::m_hwnd, SW_SHOWMAXIMIZED);
			UpdateWindow(chemd::m_hwnd);
		}
		

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		//static auto time = std::chrono::high_resolution_clock::now();
		while (msg.message != WM_QUIT)
		{
			static auto time = std::chrono::high_resolution_clock::now();
			while (::GetMessage(&msg, NULL, 0U, 0U))
			{
				auto currenttime = std::chrono::high_resolution_clock::now();
				std::chrono::duration<float, std::milli> elapsedtime = currenttime - time;
				time = std::chrono::high_resolution_clock::now();
				std::cout << 1000.0f / elapsedtime.count() << std::endl;

				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			}
		}

	}
}

void resetInputState()
{
	cdinput.LMBwasclicked = false;
	cdinput.LMBwasreleased = false;
}

LRESULT WINAPI chemd::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static bool needsredraw{ false };
	if (needsredraw)
	{
		PAINTSTRUCT ps;
		BeginPaint(chemd::m_hwnd, &ps);
		chemd::OnRender();
		EndPaint(chemd::m_hwnd, &ps);
		needsredraw = false;
	}

	resetInputState();

	switch (message)
	{
	case WM_SIZE:
		chemd::OnResize(LOWORD(lparam), HIWORD(lparam));
		return 0;

	case WM_LBUTTONDOWN:
	{
		needsredraw = true;
		SetCapture(chemd::m_hwnd);
		processmessage::LButtonDown(wparam, lparam);
		return 0;
	}

	case WM_LBUTTONUP:
	{
		needsredraw = true;
		processmessage::LButtonUp(wparam, lparam);
		ReleaseCapture();
		return 0;
	}
		
	case WM_MOUSEMOVE:
	{
		needsredraw = true;

		processmessage::MouseMove(wparam, lparam);

		//std::cout << cdinput.mousePosX << std::endl;
		
		return 0;
	}

	case WM_KEYDOWN:
	{
		needsredraw = true;
		return 0;
	}

	case WM_MOUSEWHEEL:
	{
		if (GET_KEYSTATE_WPARAM(wparam) == MK_CONTROL) //CTRL is pressed
		{
			needsredraw = true;

			
			cdinput.scrollPosX = cdinput.rendertargetCX;//GET_X_LPARAM(lparam);
			cdinput.scrollPosY = cdinput.rendertargetCY;// GET_Y_LPARAM(lparam);
			

			if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
			{
				if (cdinput.zoomlevel <= 4)
				{
					cdinput.zoomlevel += 0.5f;
				}
			}
				
			if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
			{
				if (cdinput.zoomlevel > 1.f)
				{
					cdinput.zoomlevel -= 0.5f;
				}
				if (cdinput.zoomlevel > 0.5f)
				{
					cdinput.zoomlevel -= 0.1f;
				}
			}
				

		}
		
		return 0;
	}
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return 1;
	}

	return DefWindowProcW(hwnd, message, wparam, lparam);
}

void chemd::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		D2D1_SIZE_U size;
		size.width = width;
		size.height = height;
		cdinput.rendertargetCX = width / 2;
		cdinput.rendertargetCY = height / 2;
		cdinput.scrollPosX = cdinput.rendertargetCX;
		cdinput.scrollPosY = cdinput.rendertargetCY;
		m_pRenderTarget->Resize(size);
	}
}

HRESULT chemd::CreateDeviceIndependentResources()
{
	HRESULT hr;
	static const WCHAR msc_fontName[] = L"Verdana";
	static const FLOAT msc_fontSize = 50;

	ID2D1GeometrySink* pSink = NULL;

	//Create D2D factory
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

	if (SUCCEEDED(hr))
	{
		//Create DirectWrite factory
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	}

	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = m_pDWriteFactory->CreateTextFormat(msc_fontName, NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, msc_fontSize, L"", &m_pTextFormat);
	}

	if (SUCCEEDED(hr))
	{
		// Center the text horizontally and vertically.
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		// Create a path geometry.
		hr = m_pD2DFactory->CreatePathGeometry(&m_pPathGeometry);
	}

	if (SUCCEEDED(hr))
	{
		// Use the geometry sink to write to the path geometry.
		hr = m_pPathGeometry->Open(&pSink);
	}

	if (SUCCEEDED(hr))
	{
		pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);

		pSink->BeginFigure(D2D1::Point2F(0, 0), D2D1_FIGURE_BEGIN_FILLED);

		pSink->AddLine(D2D1::Point2F(200, 0));

		pSink->AddBezier(D2D1::BezierSegment(D2D1::Point2F(150, 50), D2D1::Point2F(150, 150), D2D1::Point2F(200, 200)));

		pSink->AddLine(D2D1::Point2F(0, 200));

		pSink->AddBezier(D2D1::BezierSegment(D2D1::Point2F(50, 150), D2D1::Point2F(50, 50), D2D1::Point2F(0, 0)));

		pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		hr = pSink->Close();
	}

	SafeRelease(&pSink);

	return hr;
}

HRESULT chemd::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(chemd::m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(static_cast<UINT>(rc.right - rc.left), static_cast<UINT>(rc.bottom - rc.top));
		cdinput.rendertargetCX = size.width / 2;
		cdinput.rendertargetCY = size.height / 2;
		cdinput.scrollPosX = cdinput.rendertargetCX;
		cdinput.scrollPosY = cdinput.rendertargetCY;

		// Create Direct2D render target.
		hr = m_pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hwnd, size), &m_pRenderTarget);

		if (SUCCEEDED(hr))
		{
			// Create a black brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
		}

		if (SUCCEEDED(hr))
		{
			ID2D1GradientStopCollection* pGradientStops = NULL;

			// Create a linear gradient.
			static const D2D1_GRADIENT_STOP stops[] =
			{
				{   0.f,  { 0.f, 0.f, 0.f, 0.0f }  },
				{   1.f,  { 0.f, 0.f, 0.f, 1.f }  },
			};

			hr = m_pRenderTarget->CreateGradientStopCollection(stops, ARRAYSIZE(stops), &pGradientStops);

			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(D2D1::Point2F(100, 0), D2D1::Point2F(100, 200)), D2D1::BrushProperties(), pGradientStops, &m_pLinearGradientBrush);
				pGradientStops->Release();
			}
		}
	}
	return hr;
}

void chemd::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pBitmap);
	SafeRelease(&m_pBlackBrush);
	SafeRelease(&m_pLinearGradientBrush);
	SafeRelease(&m_pAnotherBitmap);
	SafeRelease(&m_pGridPatternBitmapBrush);
}

//void GradientLine(D2D1_POINT_2F start)
//{
//	
//	chemd::m_pRenderTarget->DrawLine(D2D1::Point2F(200, 450), D2D1::Point2F(800, 1300), chemd::m_pLinearGradientBrush, 2, NULL);
//}

HRESULT chemd::OnRender()
{
	HRESULT hr;

	hr = CreateDeviceResources();

	if (SUCCEEDED(hr) && !(m_pRenderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
	{
		D2D1_SIZE_F renderTargetSize = m_pRenderTarget->GetSize();

		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		

		static float xpos = 50;

		

		//m_pRenderTarget->DrawLine(D2D1::Point2F(static_cast<int>(xpos), 50), D2D1::Point2F(200, 400), m_pLinearGradientBrush, 2, NULL);
		
		//xpos += 0.1f;
		//if (xpos > 100)
		//	xpos = 50;
		//m_pLinearGradientBrush->SetTransform
		//static float brushthick{ 0.f };
		//m_pRenderTarget->DrawLine(D2D1::Point2F(static_cast<int>(xpos)+50, 50), D2D1::Point2F(250, 400), m_pBlackBrush, brushthick, NULL);
		//brushthick += 0.01;
		//if (brushthick > 10)
		//	brushthick = 0.f;
		//m_pLinearGradientBrush->SetOpacity(1.f);

		//m_pLinearGradientBrush->SetEndPoint(D2D1::Point2F(800, 450));
		//m_pLinearGradientBrush->SetStartPoint(D2D1::Point2F(400, 450));
		//m_pRenderTarget->DrawLine(D2D1::Point2F(400, 450), D2D1::Point2F(800, 450), m_pLinearGradientBrush, 2, NULL);
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(cdinput.zoomlevel, cdinput.zoomlevel, D2D1::Point2F(cdinput.scrollPosX, cdinput.scrollPosY)));
		//m_pRenderTarget->DrawLine(D2D1::Point2F(400,1200), D2D1::Point2F(400, 1440), m_pLinearGradientBrush, 2, NULL);

		//static std::vector<Vector2D> drawpos;
		//
		//if (cdinput.LMBwasclicked)
		//{
		//	Vector2D temp{cdinput.LMBclickPosX, cdinput.LMBclickPosY};
		//	drawpos.push_back(temp);
		//}
		//if (cdinput.LMBwasreleased)
		//{
		//	Vector2D temp{ cdinput.mousePosX, cdinput.mousePosY };
		//	drawpos.push_back(temp);
		//}
		//if (!cdinput.LMBisdown)
		//{
		//	if (drawpos.size()==2)
		//		m_pRenderTarget->DrawLine(D2D1::Point2F(drawpos[0].x, drawpos[0].y), D2D1::Point2F(drawpos[1].x, drawpos[1].y), m_pBlackBrush, 5, NULL);
		//		//m_pRenderTarget->DrawLine(D2D1::Point2F(cdinput.LMBclickPosX, cdinput.LMBclickPosY), D2D1::Point2F(cdinput.mousePosX, cdinput.mousePosY), m_pBlackBrush, 5, NULL);
		//}
		
		m_pRenderTarget->DrawLine(D2D1::Point2F(100,100), D2D1::Point2F(200, 150), m_pLinearGradientBrush, 3, NULL);
		m_pRenderTarget->DrawLine(D2D1::Point2F(200, 150), D2D1::Point2F(300, 100), m_pLinearGradientBrush, 3, NULL);

		//m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		//POINT pos;
		//GetCursorPos(&pos);
		//ScreenToClient(chemd::m_hwnd, &pos);

		//for (int i = 0; i < 50; i++)
		//{
		//	GradientLine(D2D1::Point2F(0, 0), D2D1::Point2F(cdinput.mousePosX, cdinput.mousePosY), 5, 5, 10);
		//}

		//for (int i = 0; i < molekyler.size(); i++)
		//{
		//	if (molekyler.at(i)->GetNumberOfElements() == 2)
		//	{
		//		Vector2D start = molekyler.at(i)->GetElementPosition(0);
		//		Vector2D end = molekyler.at(i)->GetElementPosition(1);
		//		m_pRenderTarget->DrawLine(D2D1::Point2F(start.x, start.y), D2D1::Point2F(end.x, end.y), m_pBlackBrush, 5, NULL);
		//	}
		//}            ,

		for (int i = 0; i < molekyler.size(); i++)
		{
			for (int bond = 0; bond < molekyler.at(i)->GetNumberOfBonds(); bond++)
			{
				Bond temp = molekyler.at(i)->GetBondByIndex(bond);
				Vector2D start = molekyler.at(i)->GetElementPosition(temp.firstelement);
				Vector2D end = molekyler.at(i)->GetElementPosition(temp.secondelement);
				m_pRenderTarget->DrawLine(D2D1::Point2F(start.x, start.y), D2D1::Point2F(end.x, end.y), m_pBlackBrush, 1.32f, NULL);
				
			}
		}

		//Draw bounding box around first molecule
		if (molekyler.size()==1)
		{
			RECT area = molekyler.at(0)->GetBoundingBoxPadded();
			m_pRenderTarget->DrawRectangle(D2D1::RectF(area.left, area.top, area.right, area.bottom), m_pBlackBrush);
		}
		

		PreviewLine();

		if (cdglobalstate.atomishovered)
		{
			m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cdinput.snapmousePosX, cdinput.snapmousePosY), 5, 5), m_pBlackBrush, 10);
		}

		m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cdinput.rendertargetCX, cdinput.rendertargetCY), 5, 5), m_pBlackBrush, 5);
		m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cdinput.mousePosX, cdinput.mousePosY), 5, 5), m_pBlackBrush);
		//m_pBlackBrush->SetColor(D2D1::ColorF(0, 1, 1, 1));
		m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(10,10), 5, 5), m_pBlackBrush,5);
		m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(2550, 10), 5, 5), m_pBlackBrush, 5);
		m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(10, 1430), 5, 5), m_pBlackBrush, 5);
		
		GradientLine(Vector2D{ 500,500 }, Vector2D{ cdinput.mousePosX, cdinput.mousePosY }, 0.9f, 0.4f, 5);

		hr = m_pRenderTarget->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
		}
	}
	return hr;
}
