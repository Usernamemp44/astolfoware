#pragma once

#include "SDK.h"

#define INRANGE(x,a,b)	(x >= a && x <= b)
#define GetBits(x)		(INRANGE((x & (~0x20)),'A','F') ? ((x & (~0x20)) - 'A' + 0xA) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GetBytes(x)		(GetBits(x[0]) << 4 | GetBits(x[1]))

class CSignature
{
public:
	DWORD FindIDAPattern(const char* szModuleName, std::string_view szSignature);
	HMODULE GetModuleHandleSafe(const char* pszModuleName);
	DWORD GetClientSignature(std::string_view szSignature);
	DWORD GetEngineSignature(std::string_view szSignature);
};

extern CSignature gSignatures;
