#ifndef _WINWINDOW_H_
#define _WINWINDOW_H_

#include"../interfaces/IWindowManager.h"

class WinWindow : public IWindowManager
{
public:
	WinWindow(unsigned int width, unsigned int height,
		LPCSTR winName, HINSTANCE hInstance, int cmdShow);
	virtual ~WinWindow();

	virtual bool VInitWindow() override;
	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND* GetHWnd();

private:
	HWND m_hwnd;
	unsigned int m_width;
	unsigned int m_height;

	LPCSTR m_winName;
	HINSTANCE* m_pHInstance;
	int m_cmdShow;

};



#endif