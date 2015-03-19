#ifndef _APP_H_
#define _APP_H_

#include"../graphics/D3DInit.h"
#include"WinWindow.h"

class App
{
public:
	App();
	~App();

	void Run(HINSTANCE hInstance, int cmdShow);

private:
	bool Init(HINSTANCE hInstance, int cmdShow);
	void ShutDown();

private:
	WinWindow* m_pWin32Window;
	IRenderManager* m_pD3DRenderer;
};

#endif
