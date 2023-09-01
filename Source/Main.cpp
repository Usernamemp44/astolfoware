#include "SDK.h"
#include "Util.h"

#include "Client.h"
#include "Panels.h"
#include "Menu.h"
#include "CMat.h"
#include <Windows.h>
#include "WinAPI.h"
#include <thread>
#pragma comment(lib, "Winmm.lib")

float SteamAPI_ServerNonLan = false;

CGlobalVariables gCvars;
CInterfaces gInts;

DWORD WINAPI dwMainThread( LPVOID lpArguments  )
{
	gOffsets.GetOffsets();

	Utils::GetInterface<CHLClient*>(gInts.Client, "client.dll", "VClient017");
	Utils::GetInterface<CEntList*>(gInts.EntList, "client.dll", "VClientEntityList003");

	Utils::GetInterface<EngineClient*>(gInts.Engine, "engine.dll", "VEngineClient013");
	Utils::GetInterface<IEngineTrace*>(gInts.EngineTrace, "engine.dll", "EngineTraceClient003");
	Utils::GetInterface<IVModelInfo*>(gInts.ModelInfo, "engine.dll", "VModelInfoClient006");
	Utils::GetInterface<CRenderView*>(gInts.RenderView, "engine.dll", "VEngineRenderView014");
	Utils::GetInterface<CModelRender*>(gInts.ModelRender, "engine.dll", "VEngineModel016");
	Utils::GetInterface<IGameEventManager2*>(gInts.EventManager, "engine.dll", "GAMEEVENTSMANAGER002");

	(GAME_TF2) ? Utils::GetInterface<CMaterialSystem*>(gInts.MatSystem, "MaterialSystem.dll", "VMaterialSystem081")
		: Utils::GetInterface<CMaterialSystemOther*>(gInts.MatSystemOther, "MaterialSystem.dll", "VMaterialSystem080");

	Utils::GetInterface<ISurface*>(gInts.Surface, "vguimatsurface.dll", "VGUI_Surface030");
	Utils::GetInterface<IPanel*>(gInts.Panels, "vgui2.dll", "VGUI_Panel009");

	Utils::GetInterface<ICvar*>(gInts.cvar, "vstdlib.dll", "VEngineCvar004");

	Utils::GetInterface<ISteamClient017*>(gInts.steamclient, "SteamClient.dll", "SteamClient017");
	HSteamPipe hNewPipe = gInts.steamclient->CreateSteamPipe();
	HSteamUser hNewUser = gInts.steamclient->ConnectToGlobalUser(hNewPipe);
	gInts.steamfriends = reinterpret_cast<ISteamFriends002*>(gInts.steamclient->GetISteamFriends(hNewUser, hNewPipe, STEAMFRIENDS_INTERFACE_VERSION_002));
	gInts.steamuser = reinterpret_cast<ISteamUser017*>(gInts.steamclient->GetISteamUser(hNewUser, hNewPipe, STEAMUSER_INTERFACE_VERSION_017));

	gInts.ClientMode = **(ClientModeShared***)((*(DWORD**)gInts.Client)[10] + 0x5);
	gInts.globals = *reinterpret_cast<CGlobals**>(gSignatures.GetEngineSignature("A1 ? ? ? ? 8B 11 68") + 8);

	MH_Initialize(); {
		MH_CreateHook(Utils::GetVirtual(gInts.ClientMode, gOffsets.iCreateMoveOffset), &Hooked_CreateMove, reinterpret_cast<void**>(&oCreateMove));
		MH_CreateHook(Utils::GetVirtual(gInts.Client, 35), &Hooked_FrameStageNotify, reinterpret_cast<void**>(&oFrameStageNotify));
		MH_CreateHook(Utils::GetVirtual(gInts.Panels, gOffsets.iPaintTraverseOffset), &Hooked_PaintTraverse, reinterpret_cast<void**>(&oPaintTraverse));
		MH_CreateHook(Utils::GetVirtual(gInts.ModelRender, 19), &Hooked_DrawModelExecute, reinterpret_cast<void**>(&oDrawModelExecute));
	}
	MH_EnableHook(MH_ALL_HOOKS);

	gMenu.windowProc = (WNDPROC)SetWindowLongPtrA(FindWindowA("Valve001", NULL), GWLP_WNDPROC, (LONG_PTR)&Hooked_WndProc);

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(350));
	gMenu.enabled = false;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	SetWindowLongPtrA(FindWindowA("Valve001", NULL), GWL_WNDPROC, (LONG_PTR)gMenu.windowProc);

	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpArguments), EXIT_SUCCESS);
}


BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		if (const HANDLE hMain = CreateThread(0, 0, dwMainThread, hInstance, 0, 0))
			CloseHandle(hMain);

	return true;
}