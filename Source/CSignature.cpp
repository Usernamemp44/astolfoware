#include "sdk.h"
#include <time.h>
#include <psapi.h>

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

DWORD CSignature::FindIDAPattern(const char* szModuleName, std::string_view szSignature)
{
	static HMODULE hModule = GetModuleHandleSafe(szModuleName);

	MODULEINFO mModuleInfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &mModuleInfo, sizeof(MODULEINFO));
	DWORD iStartAddress = reinterpret_cast<DWORD>(hModule);
	DWORD iEndAddress = iStartAddress + static_cast<DWORD>(mModuleInfo.SizeOfImage);

	const char* chPattern = szSignature.data();
	DWORD iFirstMatch = 0;
	for (DWORD pCur = iStartAddress; pCur < iEndAddress; pCur++) {
		if (!*chPattern) return iFirstMatch;
		if (*reinterpret_cast<const std::uint8_t*>(chPattern) == '?' || *reinterpret_cast<const std::uint8_t*>(pCur) == GetBytes(chPattern)) {
			if (!iFirstMatch) iFirstMatch = pCur;
			if (!chPattern[2]) return iFirstMatch;
			chPattern += (*reinterpret_cast<const std::uint16_t*>(chPattern) == '??' || *reinterpret_cast<const std::uint8_t*>(chPattern) != '?') ? 3 : 2;
		}
		else {
			chPattern = szSignature.data();
			iFirstMatch = 0;
		}
	}

	return NULL;
}
//===================================================================================
HMODULE CSignature::GetModuleHandleSafe(const char* pszModuleName)
{
	HMODULE hmModuleHandle = NULL;

	do
	{
		hmModuleHandle = GetModuleHandle(pszModuleName);
		Sleep(1);
	} while (hmModuleHandle == NULL);

	return hmModuleHandle;
}
//===================================================================================
DWORD CSignature::GetClientSignature(std::string_view szSignature)
{
	return FindIDAPattern("client.dll", szSignature);
}
//===================================================================================
DWORD CSignature::GetEngineSignature(std::string_view szSignature)
{
	return FindIDAPattern("engine.dll", szSignature);
}

CSignature gSignatures;