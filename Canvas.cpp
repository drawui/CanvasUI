// Canvas.cpp: implementation of the CCanvas class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Canvas.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCanvas::CCanvas()
{

}

CCanvas::~CCanvas()
{

}

BOOL CCanvas::AddImage(LPCSTR lpszFilePath, const RECT &rcImage, int iWidth, int iHeight)
{
	return m_GdiPlus.AddImage(lpszFilePath, rcImage, iWidth, iHeight);

}

void CCanvas::Clear()
{
	m_GdiPlus.Clear();

}

void CCanvas::Draw(HDC hDC, const RECT &rcPaint, const POINT &ptOffset)
{
	RECT rcBackground;

	::CopyRect(&rcBackground, &rcPaint);
	::OffsetRect(&rcBackground, -ptOffset.x, -ptOffset.y);
	::FillRect(hDC, &rcBackground, (HBRUSH)::GetStockObject(BLACK_BRUSH));

	m_GdiPlus.Draw(hDC, rcPaint, ptOffset);

}
