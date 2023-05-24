#include "Common.h"

#include "Window/MainWindow.h"


HRESULT MainWindow::InitWindow(_In_ HINSTANCE hInstance, _In_ INT nCmdShow, _In_ PCWSTR pszWindowName)
{
    RAWINPUTDEVICE rid = {};
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = 0u;
    rid.hwndTarget = nullptr;

    if (!RegisterRawInputDevices(&rid, 1u, sizeof(rid)))
        return E_FAIL; 

	return initialize(
		hInstance,
		nCmdShow,
		pszWindowName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		600
	);
}

PCWSTR MainWindow::GetWindowClassName() const
{
	return L"MainWindowClass";
}

LRESULT MainWindow::HandleMessage(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(m_hWnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

   case WM_KEYDOWN:
       switch (wParam)
       {
       case 'w':
       case 'W':
           m_directions.bFront = TRUE;
           break;
       case 'a':
       case 'A':
           m_directions.bLeft = TRUE;
           break;
       case 's':
       case 'S':
           m_directions.bBack = TRUE;
           break;
       case 'd':
       case 'D':
           m_directions.bRight = TRUE;
           break;
       default:
           break;
       }
       break;
   case WM_KEYUP:
       switch (wParam)
       {
       case 'w':
       case 'W':
           m_directions.bFront = FALSE;
           break;
       case 'a':
       case 'A':
           m_directions.bLeft = FALSE;
           break;
       case 's':
       case 'S':
           m_directions.bBack = FALSE;
           break;
       case 'd':
       case 'D':
           m_directions.bRight = FALSE;
           break;

       default:
           break;
       }
       break;

    default:
        return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

const InputDirections& MainWindow::GetDirections() const
{
    return m_directions;
}
