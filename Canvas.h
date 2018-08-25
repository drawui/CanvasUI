// Canvas.h: interface for the CCanvas class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CANVAS_H__3D09FF04_996E_449C_9016_7CB93E35A2DF__INCLUDED_)
#define AFX_CANVAS_H__3D09FF04_996E_449C_9016_7CB93E35A2DF__INCLUDED_

#include "GdiPlus.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCanvas  
{
public:
	CCanvas();
	virtual ~CCanvas();

	BOOL AddImage(LPCSTR lpszFilePath, const RECT& rcImage, int iWidth, int iHeight);
	void Clear();

	void Draw(HDC hDC, const RECT& rcPaint, const POINT& ptOffset);

protected:
	CGdiPlus m_GdiPlus;

};

#endif // !defined(AFX_CANVAS_H__3D09FF04_996E_449C_9016_7CB93E35A2DF__INCLUDED_)
