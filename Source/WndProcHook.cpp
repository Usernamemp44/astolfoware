#include "WndProcHook.h"

#include "Menu.h"
//#include "Menu.cpp"

LONG __stdcall WndProcHook::Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return (gMenu.enabled ? 1 : CallWindowProc(WndProc, hWnd, uMsg, wParam, lParam));
}