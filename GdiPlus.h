// GdiPlus.h: interface for the CGdiPlus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDIPLUS_H__B08A7509_322C_44E6_9C7A_348DBF18A910__INCLUDED_)
#define AFX_GDIPLUS_H__B08A7509_322C_44E6_9C7A_348DBF18A910__INCLUDED_

#include <windows.h>
#include <comdef.h>
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif
#include "gdiplus/GdiPlus.h"
#include "gdiplus/GdiplusFlat.h"
using namespace Gdiplus;
using namespace Gdiplus::DllExports;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef Status (WINAPI *GDIPLUS_STARTUP_PROC)(ULONG_PTR*, const GdiplusStartupInput*, GdiplusStartupOutput*);
typedef VOID (WINAPI *GDIPLUS_SHUTDOWN_PROC)(ULONG_PTR);
typedef GpStatus (WINGDIPAPI *GDIP_LOAD_IMAGE_FROM_FILE_PROC)(GDIPCONST WCHAR*, GpImage**);
typedef GpStatus (WINGDIPAPI *GDIP_GET_IMAGE_WIDTH_PROC)(GpImage*, UINT*);
typedef GpStatus (WINGDIPAPI *GDIP_GET_IMAGE_HEIGHT_PROC)(GpImage*, UINT*);
typedef GpStatus (WINGDIPAPI *GDIP_DISPOSE_IMAGE_PROC)(GpImage*);
typedef GpStatus (WINGDIPAPI *GDIP_CREATE_FROM_HDC_PROC)(HDC, GpGraphics**);
typedef GpStatus (WINGDIPAPI *GDIP_DELETE_GRAPHICS_PROC)(GpGraphics*);
typedef GpStatus (WINGDIPAPI *GDIP_DRAW_IMAGE_RECT_RECT_PROC)(GpGraphics*, GpImage*, REAL, REAL, REAL, REAL,
															  REAL, REAL, REAL, REAL, GpUnit, GDIPCONST GpImageAttributes*,
															  DrawImageAbort, VOID*);

class CGdiPlus  
{
public:
	CGdiPlus();
	virtual ~CGdiPlus();

	static BOOL s_Initialize();
	static void s_Uninitialize();

	BOOL AddImage(LPCSTR lpszFilePath, const RECT& rcImage, int iWidth, int iHeight);
	void Clear();

	void Draw(HDC hDC, const RECT& rcPaint, const POINT& ptOffset);

	static char s_szGdiPlusFilePath[MAX_PATH + 80];

	static bool s_bGdiPlusInitialized;

protected:
	struct _tagImage
	{
		GpImage* pImage;
		RECT rcImage;
		int iWidth;
		int iHeight;
	};
	std::vector<struct _tagImage> m_vImages;

	static int s_cGdiPlusInstances;
	static HMODULE s_hGdiPlusModule;

	static GdiplusStartupInput s_gdiplusStartupInput;
	static ULONG_PTR s_gdiplusToken;

	static GDIPLUS_STARTUP_PROC GdiplusStartup;
	static GDIPLUS_SHUTDOWN_PROC GdiplusShutdown;
	static GDIP_LOAD_IMAGE_FROM_FILE_PROC GdipLoadImageFromFile;
	static GDIP_GET_IMAGE_WIDTH_PROC GdipGetImageWidth;
	static GDIP_GET_IMAGE_HEIGHT_PROC GdipGetImageHeight;
	static GDIP_DISPOSE_IMAGE_PROC GdipDisposeImage;
	static GDIP_CREATE_FROM_HDC_PROC GdipCreateFromHDC;
	static GDIP_DELETE_GRAPHICS_PROC GdipDeleteGraphics;
	static GDIP_DRAW_IMAGE_RECT_RECT_PROC GdipDrawImageRectRect;

};

#endif // !defined(AFX_GDIPLUS_H__B08A7509_322C_44E6_9C7A_348DBF18A910__INCLUDED_)
