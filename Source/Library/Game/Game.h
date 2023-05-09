#pragma once

#include "Common.h"

#include "Window/MainWindow.h"
#include "Renderer/Renderer.h"

class Game final
{
public:
	Game(_In_ PCWSTR pszGameName);
	~Game() = default;

	HRESULT Initialize(_In_ HINSTANCE hInstance, _In_ INT nCmdShow);
	INT Run();

	PCWSTR GetGameName() const;
	std::unique_ptr<MainWindow>& GetWindow();
	std::unique_ptr<Renderer>& GetRenderer();

private:
	PCWSTR m_pszGameName;
	std::unique_ptr<MainWindow> m_mainWindow;
	std::unique_ptr<Renderer> m_renderer;
};