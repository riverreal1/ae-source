#include"pch.h"
#include"WinWindow.h"

namespace
{
	WinWindow* gpWindow = 0;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return gpWindow->WndProc(hwnd, msg, wParam, lParam);
}

WinWindow::WinWindow(unsigned int width, unsigned int height,
	LPCSTR winName, HINSTANCE hInstance, int cmdShow)
{
	m_hwnd = 0;
	m_width = width;
	m_height = height;
	m_winName = winName;
	m_pHInstance = &hInstance;
	m_cmdShow = cmdShow;
}

WinWindow::~WinWindow()
{

}

bool WinWindow::VInitWindow()
{
	LPCSTR windowClassName = "AtomEngineApp";

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = *m_pHInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDC_ICON);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = TEXT("AtomEngineApp");

	if (!RegisterClassEx(&wc))
	{
		MessageBox(0, TEXT("Failed to register class"), TEXT("Error"), MB_OK);
		return false;
	}

	DWORD dwStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	RECT windowRect = { 0, 0, m_width, m_height };
	AdjustWindowRect(&windowRect, dwStyle, false);

	m_hwnd = CreateWindowA("AtomEngineApp", m_winName,
		dwStyle, CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, *m_pHInstance, nullptr);

	if (!m_hwnd)
	{
		MessageBox(0, TEXT("Failed to create window (win32)"), TEXT("Error"), MB_OK);
		return false;
	}

	ShowWindow(m_hwnd, m_cmdShow);
	UpdateWindow(m_hwnd);

	return true;
}

LRESULT CALLBACK WinWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDC;

	switch (message)
	{
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

HWND* WinWindow::GetHWnd()
{
	return &m_hwnd;
}

