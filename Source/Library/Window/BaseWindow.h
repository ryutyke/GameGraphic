#pragma once

#include "Common.h"

template<class DerivedType>
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	BaseWindow();
	virtual ~BaseWindow() = default;

	virtual	HRESULT InitWindow(_In_ HINSTANCE hInstance, _In_ INT nCmdShow, _In_ PCWSTR pszWindowName) = 0;
	virtual	PCWSTR GetWindowClassName() const = 0;
	virtual LRESULT HandleMessage(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) = 0;

	HWND GetWindow() const;

protected:
	HRESULT initialize(
		_In_ HINSTANCE hInstance,
		_In_ INT nCmdShow,
		_In_ PCWSTR pszWindowName,
		_In_ DWORD dwStyle,
		_In_opt_ INT x = CW_USEDEFAULT,
		_In_opt_ INT y = CW_USEDEFAULT,
		_In_ INT nWidth = CW_USEDEFAULT,
		_In_ INT nHeight = CW_USEDEFAULT,
		_In_ HWND hWndParent = nullptr,
		_In_ HMENU hMenu = nullptr
	);
	
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	LPCWSTR m_pszWindowName;
};

template<class DerivedType>
inline LRESULT BaseWindow<DerivedType>::WindowProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    DerivedType* pThis = nullptr;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<DerivedType*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

        pThis->m_hWnd = hWnd;
    }
    else
    {
        pThis = reinterpret_cast<DerivedType*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis)
    {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

template<class DerivedType>
inline BaseWindow<DerivedType>::BaseWindow()
	:m_hInstance(nullptr)
	, m_hWnd(nullptr)
	, m_pszWindowName(nullptr)
{
}

template<class DerivedType>
inline HWND BaseWindow<DerivedType>::GetWindow() const
{
	return m_hWnd;
}

template<class DerivedType>
inline HRESULT BaseWindow<DerivedType>::initialize(
	_In_ HINSTANCE hInstance,
	_In_ INT nCmdShow,
	_In_ PCWSTR pszWindowName,
	_In_ DWORD dwStyle,
	_In_opt_ INT x,
	_In_opt_ INT y,
	_In_ INT nWidth,
	_In_ INT nHeight,
	_In_ HWND hWndParent,
	_In_ HMENU hMenu)
{
    // Register the window class
    WNDCLASSEX wcex =
    {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = WindowProc,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = hInstance,
        .hIcon = LoadIcon(hInstance, reinterpret_cast<LPCTSTR>IDI_APPLICATION),
        .hCursor = LoadCursor(nullptr, IDC_ARROW),
        .hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),
        .lpszMenuName = nullptr,
        .lpszClassName = GetWindowClassName(),
        .hIconSm = LoadIcon(wcex.hInstance, reinterpret_cast<LPCTSTR>IDI_APPLICATION),
    };

    if (!RegisterClassEx(&wcex))
    {
        DWORD dwError = GetLastError();

        MessageBox(
            nullptr,
            L"Call to RegisterClassEx failed!",
            L"Game Graphics Programming",
            NULL
        );

        if (dwError != ERROR_CLASS_ALREADY_EXISTS)
        {
            return HRESULT_FROM_WIN32(dwError);
        }

        return E_FAIL;
    }

    m_hInstance = hInstance;
    m_pszWindowName = pszWindowName;
    RECT rc = { 0, 0, nWidth, nHeight};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    m_hWnd = CreateWindow(
        GetWindowClassName(),
        m_pszWindowName,
        dwStyle,
        x, y, rc.right - rc.left, rc.bottom - rc.top,
        hWndParent,
        hMenu,
        m_hInstance,
        this
    );

    if (!m_hWnd)
    {
        DWORD dwError = GetLastError();

        MessageBox(
            nullptr,
            L"Call to CreateWindow failed!",
            L"Game Graphics Programming",
            NULL
        );

        if (dwError != ERROR_CLASS_ALREADY_EXISTS)
        {
            return HRESULT_FROM_WIN32(dwError);
        }

        return E_FAIL;
    }

    ShowWindow(m_hWnd, nCmdShow);

    return S_OK;
}