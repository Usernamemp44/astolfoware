#pragma once

#include "SDK.h"
#include "Detours.h"


class CGlobalzz {
public:
	CBaseEntity* local{};
	CUserCmd* cmd{};
	CUserCmd original_cmd{};
	bool isfiring, silenttime;
	bool* sendpacket;
};

struct DrawModelState_t;
struct ModelRenderInfo_t;
typedef float matrix3x4[3][4];

typedef bool(__fastcall* CreateMove_t)(void*, void*, float, CUserCmd*);
inline CreateMove_t oCreateMove;
bool __fastcall Hooked_CreateMove(void* ClientMode, void* edx, float flInputSampleFrametime, CUserCmd* pCommand);

typedef void(__fastcall* FrameStageNotify_t)(void*, void*, ClientFrameStage_t);
inline FrameStageNotify_t oFrameStageNotify;
void __fastcall Hooked_FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t Stage);

typedef void(__thiscall* DrawModelExecute_t)(CModelRender*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4* pCustomBoneToWorld);
inline DrawModelExecute_t oDrawModelExecute;
void __stdcall Hooked_DrawModelExecute(const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld);

extern CGlobalzz g;