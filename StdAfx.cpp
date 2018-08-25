// stdafx.cpp : source file that includes just the standard includes
//	CanvasUI.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

BOOL Current2Unicode(LPCTSTR lpszString, std::wstring &wstrString)
{
#ifdef _UNICODE
	wstrString = lpszString;
#else	// _UNICODE
	LPWSTR wszString = NULL;
	int iStringLen;

	iStringLen = MultiByteToWideChar(CP_ACP, 0, lpszString, -1, NULL, 0);
	if( iStringLen == 0 )
		return FALSE;
	wszString = (LPWSTR)malloc(sizeof(wchar_t) * (iStringLen + 1));
	if( wszString == NULL )
		return FALSE;
	MultiByteToWideChar(CP_ACP, 0, lpszString, -1, wszString, iStringLen);
	wstrString = wszString;
	free(wszString);
#endif	// _UNICODE
	return TRUE;
}
