#pragma once
#include "SDK.h"

namespace Global {
	extern CBaseEntity*	Local;
	extern CUserCmd*	Command;
	extern CUserCmd		OGCommand;
	extern CBaseEntity* Entity;
}

class CMisc
{
public:

	void AutoPistol(CBaseEntity* pLocal, CUserCmd* pCommand);
	void killyourselfcpp(CBaseEntity* pEntity);
	void Run(CBaseEntity* pLocal, CUserCmd* pCommand);
	bool CanShoot();

	Checkbox bhop = Checkbox("Bunnyhop");
	Checkbox shithead = Checkbox("Hackusate People :troll:");
	Checkbox astrafer = Checkbox("Autostrafer");
	Checkbox tauntslide = Checkbox("Taunt Slide");
	Checkbox speedcrouch = Checkbox("Speed Crouch");
	Checkbox nopush = Checkbox("No Push");
	Checkbox cscreenshots = Checkbox("Clean Screenshots");
	Checkbox purebypass = Checkbox("Pure Bypass");
	Checkbox chatspam = Checkbox("Chat spam");
	Checkbox niceshot = Checkbox("Nice shot!");
	Checkbox medic = Checkbox("MEDIICCCKKK");
	Checkbox helpme = Checkbox("Help!");
	Checkbox fakecrouch = Checkbox("Fake crouch RIJIN!!!!");
	Checkbox svcheats = Checkbox("Cheats bypass");
	Checkbox bypasssrootlod = Checkbox("Bypass rootlod");
	Checkbox doubletap = Checkbox("Doubletap");
	
	Checkbox warp = Checkbox("Warp");
	Listbox warp_key = Listbox("Warp Key", { "None", "Mouse 3", "Mouse 4", "Mouse 5", "Shift", "Alt", "F", "E" }, 0);
	Listbox warp_charge_key = Listbox("Charge Key", { "None", "Mouse 3", "Mouse 4", "Mouse 5", "Shift", "Alt", "F", "E" }, 0);
	Slider warp_value = Slider("Value", 1, 1, 64, 1);

	Checkbox backtrack = Checkbox("Backtrack");
	Slider backtrack_ticks = Slider("Backtrack Ticks", 0, 0, 12, 1);

	Checkbox flag = Checkbox("Fakelag");
	Slider flagamount = Slider("Fakelag Amount", 0, 0, 15, 1);

private:

	//Other funcs

	//void NoisemakerSpam(PVOID kv); fak off
	void AntiAim();
	void AntiAim2();
};



extern CMisc gMisc;