#include"pch.h"
#include"engine/system/App.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	App test;

	test.Run(hInstance, showCmd);
	 
	return 0;
}