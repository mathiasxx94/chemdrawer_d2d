#pragma once
#include <Windows.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <vector>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite")
#include "moleculeobject.h"

template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

namespace chemd
{
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	HRESULT OnRender();
	void OnResize(UINT width, UINT height);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static HWND m_hwnd;
	//static ID2D1Factory* m_pD2DFactory;
	static IWICImagingFactory* m_pWICFactory;
	static IDWriteFactory* m_pDWriteFactory;
	//extern ID2D1HwndRenderTarget* m_pRenderTarget;
	static IDWriteTextFormat* m_pTextFormat;
	static ID2D1PathGeometry* m_pPathGeometry;
	//static ID2D1LinearGradientBrush* m_pLinearGradientBrush;
	//static ID2D1SolidColorBrush* m_pBlackBrush;
	static ID2D1BitmapBrush* m_pGridPatternBitmapBrush;
	static ID2D1Bitmap* m_pBitmap;
	static ID2D1Bitmap* m_pAnotherBitmap;
}

extern ID2D1Factory* m_pD2DFactory;
extern ID2D1HwndRenderTarget* m_pRenderTarget;
extern ID2D1LinearGradientBrush* m_pLinearGradientBrush;
extern ID2D1SolidColorBrush* m_pBlackBrush;
extern ID2D1SolidColorBrush* m_pObjectHoverBrush;
extern std::vector<Molecule*> molekyler;