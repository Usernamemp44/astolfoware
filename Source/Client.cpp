#include "SDK.h"
#include "Client.h"
#include "Util.h"
#include "Aimbot.h"
#include "Triggerbot.h"
#include "HvH.h"
#include "ESP.h"
#include "Misc.h"
#include "Backtrack.h"
#include "AutoAirblast.h"
#include "Sticky.h"
#include "CMat.h"
#include "CDrawManager.h"
#include <intrin.h>
#include <Windows.h>
#include "WinAPI.h"

CGlobalzz g;

Vector qLASTTICK;

bool Key(INT vKey)
{
	if (GetAsyncKeyState(vKey) & 1)
		return true;

	return false;
}
int WarpCharge = 0;
bool IsDTing = false;
//============================================================================================

bool __fastcall Hooked_CreateMove(void* ecx, void* edx, float input_sample_frametime, CUserCmd* pCommand)
{
	try
	{													// tf was this man thinking???
		g.silenttime = false;

		g.original_cmd = *pCommand;

		if (!pCommand->command_number)
			return false;

		CBaseEntity* pLocal = GetBaseEntity(me);

		if (!pLocal)
		{
			WarpCharge = 0;
			return oCreateMove(ecx, edx, input_sample_frametime, pCommand);
		}

		g.local = reinterpret_cast<CBaseEntity*>(gInts.EntList->GetClientEntity(gInts.Engine->GetLocalPlayer()));

		if (!g.local->IsAlive() || g.local->IsDormant() || !gInts.Engine->IsInGame() || !gInts.Engine->IsConnected() || gInts.Engine->IsDrawingLoadingImage()) WarpCharge = 0;

		uintptr_t _bp; __asm mov _bp, ebp;
		g.sendpacket = (bool*)(***(uintptr_t***)_bp - 1);

		g.isfiring = (pCommand->buttons & IN_ATTACK);
		if (pLocal->IsAlive())
		{
			auto netchan = gInts.Engine->GetNetChannelInfo();
			if ((gMisc.flag.value && netchan->m_nChokedPackets < (int)gMisc.flagamount.value) || IsDTing && gMisc.CanShoot() && g.isfiring)
				*g.sendpacket = false;
			else
				*g.sendpacket = true;
		}


		
		gMisc.Run(pLocal, pCommand);

		oCreateMove(ecx, edx, input_sample_frametime, pCommand);

		Vector oAngles = pCommand->viewangles, Angles = pCommand->viewangles;
		float oForwrad = pCommand->forwardmove, oSideMove = pCommand->sidemove;

		gAim.Run(pLocal, pCommand);
		gHvH.Run(pLocal, pCommand);
		gTrigger.Run(pLocal, pCommand);
		if (GAME_TF2)
		{
			gBlast.Run(pLocal, pCommand);
			gSticky.Run(pLocal, pCommand);
		}
		backtrack::do_backtrack();

		static bool WasSet = false;
		if (pLocal->GetActiveWeapon())
		{
			if (gCvars.aimbot_silent && gAim.IsProjectileWeapon(pLocal, pLocal->GetActiveWeapon()))
			{
				if (g.silenttime) {
					*g.sendpacket = false;
					WasSet = true;
				}

				else
				{
					if (WasSet)
					{
						*g.sendpacket = true;
						pCommand->viewangles = oAngles;
						pCommand->sidemove = oSideMove;
						pCommand->forwardmove = oForwrad;
						WasSet = false;
					}
				}
			}
		}
	}
	catch (...)
	{
		printf("Failed Hooked_CreateMove");
	}
	qLASTTICK = pCommand->viewangles;
	return false/*bReturn*/;
}

void __fastcall Hooked_FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t Stage)
{
	gESP.FrameStageNotify(Stage);

	if (Stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		if (GAME_TF2)
		{
			for (auto i = 1; i <= gInts.Engine->GetMaxClients(); i++)
			{
				CBaseEntity *entity = nullptr;
				player_info_t temp;

				if (!(entity = gInts.EntList->GetClientEntity(i)))
					continue;

				if (entity->IsDormant())
					continue;

				if (!gInts.Engine->GetPlayerInfo(i, &temp))
					continue;

				if (!entity->GetLifeState() == LIFE_ALIVE)
					continue;

				Vector vX = entity->GetAngles();
				Vector vY = entity->GetAnglesHTC();
				auto *WritePitch = reinterpret_cast<float*>(reinterpret_cast<DWORD>(entity) + gNetVars.get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]"));
				auto *WriteYaw = reinterpret_cast<float*>(reinterpret_cast<DWORD>(entity) + gNetVars.get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[1]"));

				bool resolver = false;

				if (Utils::IsKeyPressedMisc(gCvars.aimbot_resolver_key))
				{
					resolver = !resolver;
				}
				if (gCvars.aimbot_resolver && resolver)
				{
					if (vX.x == -89.0f)
					{
						*WritePitch = 90.0f;
					}
					if (vX.x == 89.0f)
					{
						*WritePitch = -90.0f;
					}
				}
			}
		}
   }

	if (gInts.Engine->IsInGame() && Stage == FRAME_RENDER_START)
	{
			CBaseEntity *oEntity = gInts.EntList->GetClientEntity(gInts.Engine->GetLocalPlayer());

			int *Fov;
			int OldFov;

			int m_flFOVRate = 0xE5C;// Broken: nv.get_offset("DT_BasePlayer", "localdata", "m_flFOVRate");
			int &fovPtr = *(int*)(oEntity + gNetVars.get_offset("DT_BasePlayer", "m_iFOV")), defaultFov = *(int*)(oEntity + gNetVars.get_offset("DT_BasePlayer", "m_iDefaultFOV"));
			if (gESP.nozoom.value) //Thanks cademtz/Hold on! for this code, works amazingly aside from the inivisible sniper rifle xd
			{
				fovPtr = defaultFov;
				*(float*)(oEntity + m_flFOVRate) = 0;
			}

			if (gESP.thirdperson.value) //shows antiaims just fine
			{
				CBaseEntity *oEntity = gInts.EntList->GetClientEntity(gInts.Engine->GetLocalPlayer());

				auto *m_angEyeAngles = reinterpret_cast<float*>(reinterpret_cast<DWORD>(oEntity) + gNetVars.get_offset("DT_BasePlayer", "pl", "deadflag") + 8);

				auto *HTC = reinterpret_cast<float*>(reinterpret_cast<DWORD>(oEntity) + gNetVars.get_offset("DT_BasePlayer", "pl", "deadflag") + 4);

				*HTC = qLASTTICK.x;
				*m_angEyeAngles = qLASTTICK.y;

				ConVar* sv_cheats = gInts.cvar->FindVar("sv_cheats");
				if (sv_cheats->GetInt() == 0) sv_cheats->SetValue(1);
				ConVar* pThirdCamYaw = gInts.cvar->FindVar("cam_idealyaw");
				gInts.Engine->ClientCmd_Unrestricted("thirdperson");

				pThirdCamYaw->SetValue(0);
			}
			else if (!gESP.thirdperson.value)
			{
				gInts.Engine->ClientCmd_Unrestricted("firstperson");
			}
	}

	oFrameStageNotify(ecx, edx, Stage);
}

void __stdcall Hooked_DrawModelExecute(const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	const char* pszModelName = gInts.ModelInfo->GetModelName((DWORD*)pInfo.pModel);
	try
	{
		if (pInfo.pModel)
			gESP.DrawModelExecute(state, pInfo, pCustomBoneToWorld);

		if (gESP.hands_enabled.value == 1 && strstr(pszModelName, "arms"))
			return;
	}
	catch (...)
	{
		//MessageBox(NULL, "Failed Hooked_DrawModelExecute()", "Error", MB_OK);
		return oDrawModelExecute(gInts.ModelRender, state, pInfo, pCustomBoneToWorld);
	}

	oDrawModelExecute(gInts.ModelRender, state, pInfo, pCustomBoneToWorld);
	gMat.ResetMaterial();
}
