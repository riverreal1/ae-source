#include"pch.h"
#include"App.h"


App::App()
{
	m_pWin32Window = nullptr;
	m_pD3DRenderer = nullptr;
}


bool App::Init(HINSTANCE hInstance, int cmdShow)
{
	m_pWin32Window = new WinWindow(800, 600, "Box demo", hInstance, cmdShow);

	if (!m_pWin32Window->VInitWindow())
	{
		return false;
	}

	m_pD3DRenderer = new D3DInit(m_pWin32Window->GetHWnd(), 800, 600);

	if (!m_pD3DRenderer->VInit())
	{
		return false;
	}

	return true;
}

void App::Render()
{
	m_pD3DRenderer->VStartRendering();
	// render stuff

	m_pD3DRenderer->VEndRendering();
}

void App::Run(HINSTANCE hInstance, int cmdShow)
{
	if (!Init(hInstance, cmdShow))
	{
		MessageBox(0, TEXT("Failed to init app"), TEXT("Error"), MB_OK);
		return;
	}

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();
		}
	}

	ShutDown();
}

void App::ShutDown()
{
	delete m_pWin32Window;
	m_pD3DRenderer->VShutDown();
	delete m_pD3DRenderer;

}