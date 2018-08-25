// GdiPlus.cpp: implementation of the CGdiPlus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GdiPlus.h"

char CGdiPlus::s_szGdiPlusFilePath[MAX_PATH + 80];
bool CGdiPlus::s_bGdiPlusInitialized = false;
int CGdiPlus::s_cGdiPlusInstances = 0;
HMODULE CGdiPlus::s_hGdiPlusModule = NULL;

GdiplusStartupInput CGdiPlus::s_gdiplusStartupInput;
ULONG_PTR CGdiPlus::s_gdiplusToken;

GDIPLUS_STARTUP_PROC CGdiPlus::GdiplusStartup = NULL;
GDIPLUS_SHUTDOWN_PROC CGdiPlus::GdiplusShutdown = NULL;
GDIP_LOAD_IMAGE_FROM_FILE_PROC CGdiPlus::GdipLoadImageFromFile = NULL;
GDIP_GET_IMAGE_WIDTH_PROC CGdiPlus::GdipGetImageWidth = NULL;
GDIP_GET_IMAGE_HEIGHT_PROC CGdiPlus::GdipGetImageHeight = NULL;
GDIP_DISPOSE_IMAGE_PROC CGdiPlus::GdipDisposeImage = NULL;
GDIP_CREATE_FROM_HDC_PROC CGdiPlus::GdipCreateFromHDC = NULL;
GDIP_DELETE_GRAPHICS_PROC CGdiPlus::GdipDeleteGraphics = NULL;
GDIP_DRAW_IMAGE_RECT_RECT_PROC CGdiPlus::GdipDrawImageRectRect = NULL;

BOOL Current2Unicode(LPCTSTR lpszString, std::wstring& wstrString);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGdiPlus::CGdiPlus()
{

}

CGdiPlus::~CGdiPlus()
{

}

BOOL CGdiPlus::s_Initialize()
{
	if( s_hGdiPlusModule == NULL )
	{
		s_hGdiPlusModule = LoadLibrary(s_szGdiPlusFilePath);
		if( s_hGdiPlusModule != NULL )
		{
			GdiplusStartup = (GDIPLUS_STARTUP_PROC)GetProcAddress(s_hGdiPlusModule, "GdiplusStartup");
			GdiplusShutdown = (GDIPLUS_SHUTDOWN_PROC)GetProcAddress(s_hGdiPlusModule, "GdiplusShutdown");
			GdipLoadImageFromFile = (GDIP_LOAD_IMAGE_FROM_FILE_PROC)GetProcAddress(s_hGdiPlusModule, "GdipLoadImageFromFile");
			GdipGetImageWidth = (GDIP_GET_IMAGE_WIDTH_PROC)GetProcAddress(s_hGdiPlusModule, "GdipGetImageWidth");
			GdipGetImageHeight = (GDIP_GET_IMAGE_HEIGHT_PROC)GetProcAddress(s_hGdiPlusModule, "GdipGetImageHeight");
			GdipDisposeImage = (GDIP_DISPOSE_IMAGE_PROC)GetProcAddress(s_hGdiPlusModule, "GdipDisposeImage");
			GdipCreateFromHDC = (GDIP_CREATE_FROM_HDC_PROC)GetProcAddress(s_hGdiPlusModule, "GdipCreateFromHDC");
			GdipDeleteGraphics = (GDIP_DELETE_GRAPHICS_PROC)GetProcAddress(s_hGdiPlusModule, "GdipDeleteGraphics");
			GdipDrawImageRectRect = (GDIP_DRAW_IMAGE_RECT_RECT_PROC)GetProcAddress(s_hGdiPlusModule, "GdipDrawImageRectRect");
			if( (GdiplusStartup != NULL) && (GdiplusShutdown != NULL) && (GdipLoadImageFromFile != NULL) &&
				(GdipGetImageWidth != NULL) && (GdipGetImageHeight != NULL) && (GdipDisposeImage != NULL) &&
				(GdipCreateFromHDC != NULL) && (GdipDeleteGraphics != NULL) && (GdipDrawImageRectRect != NULL) )
				s_bGdiPlusInitialized = true;
		}
	}
	if( !s_bGdiPlusInitialized )
		return FALSE;
	if( s_cGdiPlusInstances++ == 0 )
		GdiplusStartup(&s_gdiplusToken, &s_gdiplusStartupInput, NULL);
	return TRUE;

}

void CGdiPlus::s_Uninitialize()
{
	if( --s_cGdiPlusInstances == 0 )	// 这里不能用小于等于，否则当GDI+没有顺利初始化时会有问题
	{
		GdiplusShutdown(s_gdiplusToken);
		if( s_hGdiPlusModule != NULL )
		{
			FreeLibrary(s_hGdiPlusModule);
			s_hGdiPlusModule = NULL;
		}
	}

}

BOOL CGdiPlus::AddImage(LPCSTR lpszFilePath, const RECT &rcImage, int iWidth, int iHeight)
{
	if( !s_bGdiPlusInitialized )
		return FALSE;

	std::wstring wstrFilePath;

	if( Current2Unicode(lpszFilePath, wstrFilePath) )
	{
		struct _tagImage Image;

		GdipLoadImageFromFile(wstrFilePath.c_str(), &Image.pImage);
		CopyRect(&Image.rcImage, &rcImage);
		if( iWidth >= 0 )
			Image.iWidth = iWidth;
		else
			GdipGetImageWidth(Image.pImage, ((UINT*)&Image.iWidth));
		if( iHeight >= 0 )
			Image.iHeight = iHeight;
		else
			GdipGetImageHeight(Image.pImage, ((UINT*)&Image.iHeight));
		if( Image.rcImage.right < Image.rcImage.left )
			Image.rcImage.right = Image.rcImage.left + Image.iWidth;
		if( rcImage.bottom < Image.rcImage.top )
			Image.rcImage.bottom = Image.rcImage.top + Image.iHeight;
		m_vImages.push_back(Image);
		return TRUE;
	}
	return FALSE;

}

void CGdiPlus::Clear()
{
	for( size_t nImage = 0; nImage < m_vImages.size(); nImage++ )
	{
		struct _tagImage& Image = m_vImages[nImage];

		GdipDisposeImage(Image.pImage);
	}
	m_vImages.clear();

}

void CGdiPlus::Draw(HDC hDC, const RECT &rcPaint, const POINT &ptOffset)
{
	if( !s_bGdiPlusInitialized )
		return;

	GpGraphics* pGraphics;

	GdipCreateFromHDC(hDC, &pGraphics);
	for( size_t nImage = 0; nImage < m_vImages.size(); nImage++ )
	{
		const struct _tagImage& Image = m_vImages[nImage];

		GdipDrawImageRectRect(pGraphics, Image.pImage,
			(Gdiplus::REAL)(rcPaint.left + Image.rcImage.left - ptOffset.x), (Gdiplus::REAL)(rcPaint.top + Image.rcImage.top - ptOffset.y),
			(Gdiplus::REAL)(Image.rcImage.right - Image.rcImage.left), (Gdiplus::REAL)(Image.rcImage.bottom - Image.rcImage.top),
			(Gdiplus::REAL)0.0, (Gdiplus::REAL)0.0, (Gdiplus::REAL)Image.iWidth, (Gdiplus::REAL)Image.iHeight,
			UnitPixel, NULL, NULL, NULL);
	}
	GdipDeleteGraphics(pGraphics);

}
