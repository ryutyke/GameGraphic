#include "Game/Game.h"

Game::Game(_In_ PCWSTR pszGameName)
	: m_pszGameName(pszGameName)
    , m_mainWindow(std::make_unique<MainWindow>())
    , m_renderer(std::make_unique<Renderer>())
{
}

HRESULT Game::Initialize(_In_ HINSTANCE hInstance, _In_ INT nCmdShow)
{
    HRESULT hr = m_mainWindow->InitWindow(hInstance, nCmdShow, m_pszGameName);
    if (FAILED(hr))
    {
        return hr;
    }

    hr = m_renderer->InitDevice(m_mainWindow->GetWindow());
    return hr;
}

INT Game::Run()
{
    LARGE_INTEGER startTime;
    LARGE_INTEGER endTime;
    LARGE_INTEGER frequency;
    LARGE_INTEGER elapsedMsc;

    QueryPerformanceCounter(&startTime);
    QueryPerformanceFrequency(&frequency);

    // Main message loop
    MSG msg = { 0 };
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            QueryPerformanceCounter(&endTime);
            elapsedMsc.QuadPart = endTime.QuadPart - startTime.QuadPart;
            elapsedMsc.QuadPart *= 1000000;
            elapsedMsc.QuadPart /= frequency.QuadPart;

            QueryPerformanceFrequency(&frequency);
            QueryPerformanceCounter(&startTime);

            FLOAT deltaTime = static_cast<FLOAT>(elapsedMsc.QuadPart) / 1000000.0f;

            m_renderer->HandleInput(m_mainWindow->GetDirections(), m_mainWindow->GetMouseRelativeMovement(), m_mainWindow->GetMouseRightClick(), deltaTime);
            m_mainWindow->ResetMouseMovement();
            m_renderer->Update(deltaTime);
            m_renderer->Render();
        }
    }

    return static_cast<INT>(msg.wParam);
}

PCWSTR Game::GetGameName() const
{
    return m_pszGameName;
}

std::unique_ptr<MainWindow>& Game::GetWindow()
{
    return m_mainWindow;
}

std::unique_ptr<Renderer>& Game::GetRenderer()
{
    return m_renderer;
}

