#pragma once
#include <Windows.h>

typedef float matrix3x4[3][4];

class CUserCmd; class Vector; class CBaseEntity; class CBaseCombatWeapon; 

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

namespace Utils
{
	void SilentMovementFix(CUserCmd *pUserCmd, Vector angles);
	bool IsVisible2(Vector& vecAbsStart, Vector& vecAbsEnd, CBaseEntity* pLocal, CBaseEntity* pEntity);
	float getVectorLength(Vector vector);
	float getVectorDistance(Vector vec1, Vector vec2);
	void VectorTransform(const Vector& vSome, const matrix3x4& vMatrix, Vector& vOut);
	Vector AngleVector(Vector meme);
	float DistPointToLine(Vector point, Vector origin, Vector direction);
	float flGetDistance(Vector vOrigin, Vector vLocalOrigin);
	bool IsVisible(void* pLocal, void* pEntity, Vector vStart, Vector vEnd);
	bool IsKeyPressed(int i);
	bool IsKeyPressedMisc(int i);
	bool IsHeadshotWeapon(CBaseEntity* pLocal, CBaseCombatWeapon* pWep);
	bool IsOtherSlot(CBaseEntity* pLocal, CBaseCombatWeapon* pWep);
	Vector EstimateAbsVelocity(CBaseEntity *ent);
	float RandFloatRange(float min, float max);
	void* InitKeyValue();
	

	template <typename T>
	inline void GetInterface(T& Interface, const char* szModuleName, const char* szInterfaceName)
	{
		if (HMODULE Module = GetModuleHandleA(szModuleName))
		{
			if (CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(Module, "CreateInterface"))
				Interface = reinterpret_cast<T>(CreateInterface(szInterfaceName, NULL));
			else
				MessageBoxA(NULL, szInterfaceName, "Failed to find interface!", NULL);

			return;
		}

		MessageBoxA(NULL, szInterfaceName, "Failed to find find module!", NULL);
	}

	inline void* GetVirtual(void* iClass, int iIndex)
	{
		return (void*)((DWORD)(*(PVOID**)iClass)[iIndex]);
	}

	template< typename Fn >
	inline Fn CallVirtual(void* iClass, size_t iIndex)
	{
		return reinterpret_cast<Fn>(GetVirtual(iClass, iIndex));
	}
}

#define VectorMA(v, s, b, o)    ((o)[0] = (v)[0] + (b)[0] * (s), (o)[1] = (v)[1] + (b)[1] * (s), (o)[2] = (v)[2] + (b)[2] * (s))
