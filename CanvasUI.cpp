// CanvasUI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "Canvas.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text
CCanvas g_Canvas;
SIZE g_sizeOldTotal = { 0, 0 };
SIZE g_sizeOldClient = { 0, 0 };
HBITMAP g_hbmMem = NULL;

#if (_WIN32_WINNT >= 0x400)
int g_iMouseWheel = 0;
#endif

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	strcpy(CGdiPlus::s_szGdiPlusFilePath, "C:\\Windows\\System32\\gdiplus.dll");

	CGdiPlus::s_Initialize();

	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CANVASUI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		CGdiPlus::s_Uninitialize();
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_CANVASUI);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	g_Canvas.Clear();
	CGdiPlus::s_Uninitialize();
	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_CANVASUI);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_CANVASUI;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_CREATE:
			{
				RECT rcImage = { 100, 50, -1, -1 };

				g_Canvas.AddImage(_T("panda.gif"), rcImage, -1, -1);
			}
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			RECT rcCanvas;
			GetClientRect(hWnd, &rt);
			HDC hdcMem;
			HBITMAP hbmOld;
			hdcMem = ::CreateCompatibleDC(ps.hdc);
			hbmOld = NULL;
			if( (g_sizeOldClient.cx != (rt.right - rt.left)) ||
				(g_sizeOldClient.cy != (rt.bottom - rt.top)) )
			{
				DeleteObject(g_hbmMem);
				g_hbmMem = NULL;
			}
			if( g_hbmMem == NULL )
			{
				g_hbmMem = ::CreateCompatibleBitmap(hdc, rt.right - rt.left, rt.bottom - rt.top);
			}
			hbmOld = (HBITMAP)::SelectObject(hdcMem, g_hbmMem);
			::FillRect(hdcMem, &rt, (HBRUSH)::GetStockObject(WHITE_BRUSH));
			rcCanvas.left = max(0, (rt.left + rt.right) / 2 - 450);
			rcCanvas.right = rcCanvas.left + 900;
			rcCanvas.top = 20;
			rcCanvas.bottom = rcCanvas.top + 400;
			POINT ptOffset;
			ptOffset.x = GetScrollPos(hWnd, SB_HORZ);
			ptOffset.y = GetScrollPos(hWnd, SB_VERT);
#if (_WIN32_WINNT >= 0x400)
			if( g_iMouseWheel != 0 )
			{
				POINT ptVRange = { 0, g_sizeOldTotal.cy };
				int iVPage = g_sizeOldClient.cy;

				if( ptVRange.y - ptVRange.x > iVPage )
				{
					int iVStep = 16;

					if( iVStep > 0 )
					{
						ptOffset.y = min((ptVRange.y - iVPage + iVStep) / iVStep * iVStep,
							max(ptVRange.x / iVStep * iVStep, (ptOffset.y + iVStep * g_iMouseWheel) / iVStep * iVStep));
						SetScrollPos(hWnd, SB_VERT, ptOffset.y, TRUE);
					}
				}
				g_iMouseWheel = 0;
			}
#endif
			g_Canvas.Draw(hdcMem, rcCanvas, ptOffset);
			::BitBlt(hdc, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top,
				hdcMem, rt.left, rt.top, SRCCOPY);
			::SelectObject(hdcMem, hbmOld);
			::DeleteDC(hdcMem);
			EndPaint(hWnd, &ps);
			{
				SIZE sizeTotal = { rcCanvas.right - rcCanvas.left, rcCanvas.bottom - rcCanvas.top + 40 };
				SCROLLINFO sinfo;

				memset(&sinfo, 0, sizeof(sinfo));
				if( (sizeTotal.cx != g_sizeOldTotal.cx) ||
					(rt.right - rt.left != g_sizeOldClient.cx) )
				{
					g_sizeOldTotal.cx = sizeTotal.cx;
					g_sizeOldClient.cx = rt.right - rt.left;
					sinfo.cbSize = sizeof(sinfo);
					if( g_sizeOldTotal.cx < rt.right - rt.left )
					{
						SetScrollPos(hWnd, SB_HORZ, 0, TRUE);
						EnableScrollBar(hWnd, SB_HORZ, ESB_DISABLE_BOTH);
					}
					else
					{
						EnableScrollBar(hWnd, SB_HORZ, ESB_ENABLE_BOTH);
						sinfo.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
						sinfo.nMin = 0;
						sinfo.nMax = g_sizeOldTotal.cx;
						sinfo.nPage = g_sizeOldClient.cx;
						sinfo.nPos = ptOffset.x;
						SetScrollInfo(hWnd, SB_HORZ, &sinfo, TRUE);
					}
				}
				memset(&sinfo, 0, sizeof(sinfo));
				if( (sizeTotal.cy != g_sizeOldTotal.cy) ||
					(rt.bottom - rt.top != g_sizeOldClient.cy) )
				{
					g_sizeOldTotal.cy = sizeTotal.cy;
					g_sizeOldClient.cy = rt.bottom - rt.top;
					sinfo.cbSize = sizeof(sinfo);
					if( g_sizeOldTotal.cy < rt.bottom - rt.top )
					{
						SetScrollPos(hWnd, SB_VERT, 0, TRUE);
						EnableScrollBar(hWnd, SB_VERT, ESB_DISABLE_BOTH);
					}
					else
					{
						EnableScrollBar(hWnd, SB_VERT, ESB_ENABLE_BOTH);
						sinfo.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
						sinfo.nMin = 0;
						sinfo.nMax = g_sizeOldTotal.cy;
						sinfo.nPage = g_sizeOldClient.cy;
						sinfo.nPos = ptOffset.y;
						SetScrollInfo(hWnd, SB_VERT, &sinfo, TRUE);
					}
				}
			}
			break;
		case WM_ERASEBKGND:
			return 0;
#if (_WIN32_WINNT >= 0x400)
		case WM_MOUSEWHEEL:
			{
				short sDelta = (short)HIWORD(wParam);

				if( sDelta > 0 )	// ио╧Ж
				{
					g_iMouseWheel--;
				}
				else
				{
					g_iMouseWheel++;
				}
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
#endif
		case WM_HSCROLL:
			{
				SCROLLINFO si;
				int xCurrentScroll = GetScrollPos(hWnd, SB_HORZ);
				int xDelta;     // xDelta = new_pos - current_pos
				int xNewPos;    // new position
				int yDelta = 0;

				switch ((short)LOWORD(wParam))
				{
				case SB_PAGEUP:
					xNewPos = xCurrentScroll - g_sizeOldClient.cx;
					break;
				// User clicked the shaft right of the scroll box.
				case SB_PAGEDOWN:
					xNewPos = xCurrentScroll + g_sizeOldClient.cx;
					break;
				// User clicked the left arrow.
				case SB_LINEUP:
					xNewPos = xCurrentScroll - 16;
					break;
				// User clicked the right arrow.
				case SB_LINEDOWN:
					xNewPos = xCurrentScroll + 16;
					break;
				// User dragged the scroll box.
				case SB_THUMBPOSITION:
				case SB_THUMBTRACK:
					xNewPos = HIWORD(wParam);
					break;
				default:
					xNewPos = xCurrentScroll;
				}

				// New position must be between 0 and the screen width.
				xNewPos = max(0, xNewPos);
				xNewPos = min(g_sizeOldTotal.cx, xNewPos);
				// If the current position does not change, do not scroll.
				if (xNewPos == xCurrentScroll)
					break;
				// Determine the amount scrolled (in pixels).
				xDelta = xNewPos - xCurrentScroll;
				// Reset the current scroll position.
				xCurrentScroll = xNewPos;
				// Scroll the window. (The system repaints most of the
				// client area when ScrollWindowEx is called; however, it is
				// necessary to call UpdateWindow in order to repaint the
				// rectangle of pixels that were invalidated.)
				ScrollWindowEx(hWnd, xDelta, yDelta, (CONST RECT *) NULL,
					(CONST RECT *) NULL, (HRGN) NULL, (LPRECT) NULL, 0);
				InvalidateRect(hWnd, NULL, FALSE);
				// Reset the scroll bar.
				si.cbSize = sizeof(si);
				si.fMask  = SIF_POS;
				si.nPos   = xCurrentScroll;
				SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
			}
			break;
		case WM_VSCROLL:
			{
				SCROLLINFO si;
				int yCurrentScroll = GetScrollPos(hWnd, SB_VERT);
				int xDelta = 0;
				int yDelta;     // yDelta = new_pos - current_pos
				int yNewPos;    // new position

				switch ((short)LOWORD(wParam))
				{
				// User clicked the shaft above the scroll box.
				case SB_PAGEUP:
					yNewPos = yCurrentScroll - g_sizeOldClient.cy;
					break;
				// User clicked the shaft below the scroll box.
				case SB_PAGEDOWN:
					yNewPos = yCurrentScroll + g_sizeOldClient.cy;
					break;
				// User clicked the top arrow.
				case SB_LINEUP:
					yNewPos = yCurrentScroll - 16;
					break;
				// User clicked the bottom arrow.
				case SB_LINEDOWN:
					yNewPos = yCurrentScroll + 16;
					break;
				// User dragged the scroll box.
				case SB_THUMBPOSITION:
				case SB_THUMBTRACK:
					yNewPos = HIWORD(wParam);
					break;
				default:
					yNewPos = yCurrentScroll;
				}

				// New position must be between 0 and the screen height.
				yNewPos = max(0, yNewPos);
				yNewPos = min(g_sizeOldTotal.cy, yNewPos);
				// If the current position does not change, do not scroll.
				if (yNewPos == yCurrentScroll)
					break;
				// Determine the amount scrolled (in pixels).
				yDelta = yNewPos - yCurrentScroll;
				// Reset the current scroll position.
				yCurrentScroll = yNewPos;
				// Scroll the window. (The system repaints most of the
				// client area when ScrollWindowEx is called; however, it is
				// necessary to call UpdateWindow in order to repaint the
				// rectangle of pixels that were invalidated.)
				ScrollWindowEx(hWnd, xDelta, yDelta, (CONST RECT *) NULL,
					(CONST RECT *) NULL, (HRGN) NULL, (LPRECT) NULL, 0);
				InvalidateRect(hWnd, NULL, FALSE);
				// Reset the scroll bar.
				si.cbSize = sizeof(si);
				si.fMask  = SIF_POS;
				si.nPos   = yCurrentScroll;
				SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			if( g_hbmMem != NULL )
				DeleteObject(g_hbmMem);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
