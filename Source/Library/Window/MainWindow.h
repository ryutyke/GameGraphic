#pragma once

#include "Common.h"

#include "Window/BaseWindow.h"
#include "Renderer/DataTypes.h"

class MainWindow :public BaseWindow<MainWindow>
{
public:
	MainWindow() = default;
	virtual ~MainWindow() = default;

	HRESULT InitWindow(_In_ HINSTANCE hInstance, _In_ INT nCmdShow, _In_ PCWSTR pszWindowName) override;
	PCWSTR GetWindowClassName() const override;
	LRESULT HandleMessage(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) override;

	const InputDirections& GetDirections() const;
	const MouseRelativeMovement& GetMouseRelativeMovement() const;
	const BOOL& GetMouseRightClick() const;
	void ResetMouseMovement();

private:
	InputDirections m_directions;
	MouseRelativeMovement m_mouseRelativeMovement;
	BOOL m_bMouseRightClick;
};