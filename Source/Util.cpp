#include "Util.h"
#include "SDK.h"

float Utils::flGetDistance(Vector vOrigin, Vector vLocalOrigin)
{
	Vector vDelta = vOrigin - vLocalOrigin;

	float m_fDistance = sqrt(vDelta.Length());

	if (m_fDistance < 1.0f)
		return 1.0f;

	return m_fDistance;
}

void Utils::VectorTransform(const Vector& vSome, const matrix3x4& vMatrix, Vector& vOut)
{
	for (auto i = 0; i < 3; i++)
		vOut[i] = vSome.Dot((Vector&)vMatrix[i]) + vMatrix[i][3];
}

Vector Utils::AngleVector(Vector meme) {
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));
	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI));
	return Vector(cp * cy, cp * sy, -sp);
}

float Utils::DistPointToLine(Vector point, Vector origin, Vector direction) {
	auto point_direction = point - origin;

	auto temporary_offset = point_direction.dot(direction) / (direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
	if (temporary_offset < 0.000001f) {
		return FLT_MAX;
	}

	auto perpendicular_point = origin + (direction * temporary_offset);

	return (point - perpendicular_point).length();
}

bool Utils::IsVisible(void* pLocal, void* pEntity, Vector vStart, Vector vEnd)
{
	trace_t Trace;
	Ray_t Ray;			 // the future of variable naming
	CTraceFilter Filter;

	Filter.pSkip = pLocal;

	Ray.Init(vStart, vEnd);

	gInts.EngineTrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

	return (Trace.m_pEnt == pEntity || Trace.fraction == 1.0);
}

bool Utils::IsKeyPressed(int i)
{
	switch (i)
	{
	case 0: //None
		return true;
	case 1: // Key "B"
		return GetAsyncKeyState(0x42); // 0x42 = "B".
	case 2:
		return GetAsyncKeyState(VK_RBUTTON);
	case 3:										  //Mouses 1-5
		return GetAsyncKeyState(VK_MBUTTON);
	case 4:
		return GetAsyncKeyState(VK_XBUTTON1);
	case 5:
		return GetAsyncKeyState(VK_XBUTTON2);
	case 6: //Shift
		return GetAsyncKeyState(VK_SHIFT);
	case 7: //Alt
		return GetAsyncKeyState(VK_MENU);
	case 8: //F
		return GetAsyncKeyState(0x46);
	default:
		return false;
	}

	return false;
}

bool Utils::IsKeyPressedMisc(int i)
{
	switch (i)
	{
	case 0: //None
		return true;
	case 1:  //Mouses 1-5
		return GetAsyncKeyState(VK_MBUTTON);
	case 2:
		return GetAsyncKeyState(VK_XBUTTON1);
	case 3:
		return GetAsyncKeyState(VK_XBUTTON2);
	case 4: //Shift
		return GetAsyncKeyState(VK_SHIFT);
	case 5: //Alt
		return GetAsyncKeyState(VK_MENU);
	case 6: //F
		return GetAsyncKeyState(0x46);
	case 7: //E
		return GetAsyncKeyState(0x45);
	default:
		return false;
	}

	return false;
}

void Utils::SilentMovementFix(CUserCmd *pUserCmd, Vector angles)
{
	Vector vecSilent(pUserCmd->forwardmove, pUserCmd->sidemove, pUserCmd->upmove);
	float flSpeed = sqrt(vecSilent.x * vecSilent.x + vecSilent.y * vecSilent.y);
	Vector angMove;
	VectorAngles(vecSilent, angMove);
	float flYaw = DEG2RAD(angles.y - pUserCmd->viewangles.y + angMove.y);
	pUserCmd->forwardmove = cos(flYaw) * flSpeed;
	pUserCmd->sidemove = sin(flYaw) * flSpeed;
	pUserCmd->viewangles = angles;
}

bool Utils::IsVisible2(Vector& vecAbsStart, Vector& vecAbsEnd, CBaseEntity* pLocal, CBaseEntity* pEntity)
{
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	ray.Init(vecAbsStart, vecAbsEnd);
	gInts.EngineTrace->TraceRay(ray, 0x200400B, &filter, &tr);

	if (tr.m_pEnt == NULL)
		if (tr.fraction == 1.0f)
			return true;

	return (tr.m_pEnt->GetIndex() == pEntity->GetIndex());
}


float Utils::getVectorLength(Vector vector)
{
	return (float)sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
}
float Utils::getVectorDistance(Vector vec1, Vector vec2)
{
	return getVectorLength(vec2 - vec1);
}

Vector Utils::EstimateAbsVelocity(CBaseEntity *ent)
{
	typedef void(__thiscall * EstimateAbsVelocityFn)(CBaseEntity *, Vector &);



	static DWORD dwFn = gSignatures.GetClientSignature("E8 ? ? ? ? F3 0F 10 4D ? 8D 85 ? ? ? ? F3 0F 10 45 ? F3 0F 59 C9 56 F3 0F 59 C0 F3 0F 58 C8 0F 2F 0D ? ? ? ? 76 07") + 0x1;

	static DWORD dwEstimate = ((*(PDWORD)(dwFn)) + dwFn + 4);

	EstimateAbsVelocityFn vel = (EstimateAbsVelocityFn)dwEstimate;

	Vector v;

	vel(ent, v);

	return v;
}



float Utils::RandFloatRange(float min, float max)
{
	return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
}


bool Utils::IsOtherSlot(CBaseEntity* pLocal, CBaseCombatWeapon* pWep)
{
	if (pWep->GetSlot() == 2)
		return true;

	if (pWep->GetSlot() == 3)
		return true;

	return false;
}

bool Utils::IsHeadshotWeapon(CBaseEntity* pLocal, CBaseCombatWeapon* pWep)
{
	if (pWep->GetSlot() == 0 && pLocal->GetClassNum() == TF2_Sniper)
		if (pWep->GetItemDefinitionIndex() != WPN_Huntsman && pWep->GetItemDefinitionIndex() != WPN_CompoundBow)
			return true;

	if (pWep->GetItemDefinitionIndex() == WPN_Ambassador || pWep->GetItemDefinitionIndex() == WPN_FestiveAmbassador)
		return true;

	return false;
}

PVOID Utils::InitKeyValue() //Credits f1ssion
{
	typedef PDWORD(__cdecl* Init_t)(int);
	static DWORD dwInitLocation = gSignatures.GetClientSignature("E8 ? ? ? ? 83 C4 14 85 C0 74 10 68") + 0x1;
	static DWORD dwInit = ((*(PDWORD)(dwInitLocation)) + dwInitLocation + 4);
	static Init_t InitKeyValues = (Init_t)dwInit;
	return InitKeyValues(32);
}
