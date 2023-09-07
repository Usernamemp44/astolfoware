#include "Misc.h"
#include "Util.h"
#include "Aimbot.h"
#include "Client.h"
#include <chrono>
#include <random>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include <cstdlib>
#include "FNV1A.h"
CMisc gMisc;

namespace Global {
	CBaseEntity* Global::Local;
	CUserCmd* Global::Command;
	CUserCmd		Global::OGCommand;
	CBaseEntity* Global::Entity;
}

std::string repeat(int n, const char* str)
{
	std::ostringstream os;
	for (int i = 0; i < n; i++)
		os << str;
	return os.str();
}

bool CMisc::CanShoot()
{
	CBaseCombatWeapon* pWeapon = g.local->GetActiveWeapon();
	if (!pWeapon)
		return false;
	if (!g.local->IsAlive())
		return false;

	return (pWeapon->GetNextPrimaryAttack() < g.local->TickBase() * gInts.globals->interval_per_tick);
}

std::string GetSpam(const int nIndex) {
	std::string str;

	switch (nIndex)
	{
	case 0: str = XorStr("say Astolfoware - Step up the game with a boykisser cheat! ").str(); break;
	case 1: str = XorStr("say Astolfoware - Way to the top!").str(); break;
	case 2: str = XorStr("say Astolfoware - Based on NaCl!").str(); break;
	case 3: str = XorStr("say dsc.gg/astolfoware").str(); break;
	default: str = XorStr("say Astolfoware - Step up the game with a boykisser cheat!").str(); break;
	}

	return str;
}
void CMisc::AutoPistol(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	if (gInts.Engine->GetAppId() != 440) //super advanced auto pistol code :O
	{
		static bool check = false;
		CBaseCombatWeapon* pWeapon = pLocal->GetActiveWeapon();
		if (CanShoot())
		{
			if (pCommand->buttons & IN_ATTACK)
			{
				check = true;
				{
					static bool flipFlop = true;
					if (flipFlop) { pCommand->buttons |= IN_ATTACK; }
					else { pCommand->buttons &= (~IN_ATTACK); }
					flipFlop = !flipFlop;
				}
			}
			else
			{
				check = false;
			}
		}
	}
}
void CMisc::killyourselfcpp(CBaseEntity* pEntity)
{
	float flCurTime = gInts.Engine->Time();
	static float flNextSend = 0.0f;
	player_info_t pInfo;
	if (!gInts.Engine->GetPlayerInfo(pEntity->GetIndex(), &pInfo))
		return;
	std::string fucku = "say \"" + std::string(pInfo.name) + " is hacking, Please kick!\"";
	if (flCurTime > flNextSend)
	{
		gInts.Engine->ClientCmd_Unrestricted(fucku.c_str());
		flNextSend = (flCurTime + 8.0f);
	}
}
void CMisc::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{

	if (!pLocal->IsAlive())
		return;
	if (shithead.value)
	{
		// lol i had to do was this. I dont know what this does btw,
		// the only thing i know what it does it is define pEntity the right way, and run the hackusation void.
		for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
		{
			if (i == me)
				continue;

			CBaseEntity* pEntity = GetBaseEntity(i);

			if (!pEntity)
				continue;

			if (pEntity->IsDormant())
				continue;

			if (pEntity->GetLifeState() != LIFE_ALIVE)
				continue;

			killyourselfcpp(pEntity);
		}
	}
	if (GAME_CSS && gAim.antirecoil.value)
	{
		Vector AimPunch = pLocal->GetVecPunchAngle();
		pCommand->viewangles -= (AimPunch * 2.f);
	}
	if (gAim.autopistol.value)
		AutoPistol(pLocal, pCommand);

	if (GAME_TF2 && tauntslide.value) //This is for some reason broken, I have an idea why just haven't got to trying to fix it.
	{
		if ((pLocal->GetCond() & tf_cond::TFCond_Taunting))
		{

			float forward = 0.0f, side = 0.0f;

			if (pCommand->buttons & IN_FORWARD) { forward += 450; }
			if (pCommand->buttons & IN_BACK) { forward -= 450; }
			if (pCommand->buttons & IN_MOVELEFT) { side -= 450; }
			if (pCommand->buttons & IN_MOVERIGHT) { side += 450; }

			pCommand->forwardmove = forward;
			pCommand->sidemove = side;

			Vector viewangles;
			gInts.Engine->GetViewAngles(viewangles);

			pCommand->viewangles.y = viewangles[1];
			Global::OGCommand.viewangles.y = viewangles[1];
		}
	}
	if (GAME_TF2 && speedcrouch.value && !(pCommand->buttons & IN_ATTACK) && (pCommand->buttons & IN_DUCK)) // who changed my comment >:(
	{
		Vector vLocalAngles = pCommand->viewangles;
		float speed = pCommand->forwardmove;
		if (fabs(speed) > 0.0f) {
			pCommand->forwardmove = -speed;
			pCommand->sidemove = 0.0f;
			pCommand->viewangles.y = vLocalAngles.y;
			pCommand->viewangles.y -= 180.0f;
			if (pCommand->viewangles.y < -180.0f) pCommand->viewangles.y += 360.0f;
			pCommand->viewangles.z = 90.0f;
		}
	}
	if (!(pLocal->GetFlags() & FL_ONGROUND) && pCommand->buttons & IN_JUMP)
	{
		//Autostrafe	
		if (astrafer.value)
			if (pCommand->mousedx > 1 || pCommand->mousedx < -1)  //> 1 < -1 so we have some wiggle room
				pCommand->sidemove = pCommand->mousedx > 1 ? 450.f : -450.f;

		//Bunnyhop
		if (bhop.value)
			pCommand->buttons &= ~IN_JUMP;
	}
	if (niceshot.value)
	{
		float flCurTime = gInts.Engine->Time();
		static float flNextSend = 0.0f;
		if (flCurTime > flNextSend)
		{
			gInts.Engine->ClientCmd_Unrestricted("voicemenu 2 6");
			flNextSend = (flCurTime + 8.0f);
		}
	}
	if (medic.value)
	{
		float flCurTime = gInts.Engine->Time();
		static float flNextSend = 0.0f;
		if (flCurTime > flNextSend)
		{
			gInts.Engine->ClientCmd_Unrestricted("voicemenu 0 0");
			flNextSend = (flCurTime + 8.0f);
		}
	}
	if (helpme.value)
	{
		float flCurTime = gInts.Engine->Time();
		static float flNextSend = 0.0f;
		if (flCurTime > flNextSend)
		{
			gInts.Engine->ClientCmd_Unrestricted("voicemenu 2 0");
			flNextSend = (flCurTime + 8.0f);
		}
	}
	if (GAME_TF2 && nopush.value)
	{
		ConVar* tf_avoidteammates_pushaway = gInts.cvar->FindVar("tf_avoidteammates_pushaway");
		if (tf_avoidteammates_pushaway->GetInt() == 1)
			tf_avoidteammates_pushaway->SetValue(0);
	}
	if (GAME_TF2 && fakecrouch.value)
	{
		for (int i = 0; i < (int)fakecrouch.value; i++)
		{
			gInts.Engine->ClientCmd_Unrestricted("+duck;wait 300;-duck");
		}
	}
	if (GAME_TF2 && svcheats.value)
	{
		static bool cheatset = false;
		if (ConVar* sv_cheats = gInts.cvar->FindVar("sv_cheats"))
		{
			sv_cheats->SetValue(1);
			cheatset = true;
		}
	}
	if (GAME_TF2 && bypasssrootlod.value)
	{
		static bool cheatset = false;
		if (ConVar* sv_cheats = gInts.cvar->FindVar("sv_cheats"))
		{
			sv_cheats->SetValue(1);
			cheatset = true;
		}
		gInts.Engine->ClientCmd_Unrestricted("r_rootlod 7");
	}
	if (GAME_TF2 && chatspam.value) {
		float flCurTime = gInts.Engine->Time();
		static float flNextSend = 0.0f;
		// this is not pasted xde
		if (flCurTime > flNextSend) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, 3);

			int randomIndex = dis(gen);

			gInts.Engine->ClientCmd_Unrestricted(GetSpam(randomIndex).c_str());
			flNextSend = (flCurTime + 8.0f); // you will get insta chatbanned if you dont wait for 8.0f!
		}
	}

}

//pAAs
void CMisc::AntiAim()
{

}
