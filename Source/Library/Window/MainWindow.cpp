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

   case WM_INPUT:
   {
       UINT dataSize = 0u;

       GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &dataSize, sizeof(RAWINPUTHEADER));

       if (dataSize > 0)
       {
           std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(dataSize);
           if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), &dataSize, sizeof(RAWINPUTHEADER)))
           {
               RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());

               if (raw->header.dwType == RIM_TYPEMOUSE)
               {
                   m_mouseRelativeMovement.X = raw->data.mouse.lLastX;
                   m_mouseRelativeMovement.Y = raw->data.mouse.lLastY;

                   RECT rc;
                   RECT rc2;
                   POINT p1;
                   POINT p2;

                   GetWindowRect(m_hWnd, &rc2);
                   GetClientRect(m_hWnd, &rc);

                   p1.x = rc.left;
                   p1.y = rc.top;
                   p2.x = rc.right;
                   p2.y = rc.bottom;

                   ClientToScreen(m_hWnd, &p1);
                   ClientToScreen(m_hWnd, &p2);

                   rc.left = p1.x;
                   rc.top = rc2.top;
                   rc.right = p2.x;
                   rc.bottom = p2.y;

                   ClipCursor(&rc);
               }

           }
       }
       return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
   }


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
       case 'e':
       case 'E':
           m_directions.bUp = TRUE;
           break;
       case 'q':
       case 'Q':
           m_directions.bDown = TRUE;
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
       case 'e':
       case 'E':
           m_directions.bUp = FALSE;
           break;
       case 'q':
       case 'Q':
           m_directions.bDown = FALSE;
           break;

       default:
           break;
       }
       break;

   case WM_RBUTTONDOWN:
       m_bMouseRightClick = TRUE;
       break;
   case WM_RBUTTONUP:
       m_bMouseRightClick = FALSE;
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

const MouseRelativeMovement& MainWindow::GetMouseRelativeMovement() const
{
    return m_mouseRelativeMovement;
}

const BOOL& MainWindow::GetMouseRightClick() const
{
    return m_bMouseRightClick;
}

void MainWindow::ResetMouseMovement()
{
    memset(&m_mouseRelativeMovement, 0, sizeof(MouseRelativeMovement));
}