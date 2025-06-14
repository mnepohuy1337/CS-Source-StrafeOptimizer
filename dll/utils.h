#include <TlHelp32.h>
#include <Psapi.h>

#define INRANGE(x, a, b) (x >= a && x <= b) 
#define getBits(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (INRANGE(x, '0', '9') ? x - '0': 0))
#define getByte(x) (getBits(x[0]) << 4 | getBits(x[1]))

typedef struct BASE_RELOCATION_ENTRY {
	USHORT Offset : 12;
	USHORT Type : 4;
} BASE_RELOCATION_ENTRY, * PBASE_RELOCATION_ENTRY;

typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

class IInputSystem
{
public:

};

class CInput
{
public:

};


void* AddrFromLea(uintptr_t start)
{
	auto offset = *(int32_t*)(start + 3);
	auto rip = start + 7;
	return (void*)((intptr_t)rip + offset);
}


uintptr_t FindPatternEx(HANDLE hProc, uintptr_t base, uintptr_t len, BYTE* sig, char* mask)
{
	BYTE* buf = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (ReadProcessMemory(hProc, (LPCVOID)base, buf, len, NULL))
	{
		for (uintptr_t i = 0; i <= (len - strlen(mask)); i++)
		{
			if ((buf[i] == sig[0] && mask[0] == 'x') || (mask[0] == '?'))
			{
				for (int x = 0;; x++)
				{
					if (mask[x] == 'x')
					{
						if (buf[i + x] == sig[x])
							continue;
						else
							break;
					}
					else if (mask[x] == 0x00)
					{
						return (uintptr_t)(base + i);
					}
				}
			}
		}
	}
	return NULL;
}

uintptr_t FindPattern(const char* szModule, const char* szSignature)
{
	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
	uintptr_t startAddress = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);
	uintptr_t endAddress = startAddress + modInfo.SizeOfImage;
	const char* pat = szSignature;
	uintptr_t firstMatch = 0;
	for (uintptr_t pCur = startAddress; pCur < endAddress; pCur++)
	{
		if (!*pat) return firstMatch;
		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch) firstMatch = pCur;
			if (!pat[2]) return firstMatch;
			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
			else pat += 2;
		}
		else
		{
			pat = szSignature;
			firstMatch = 0;
		}
	}

	return NULL;
}

float MIN(float a, float b)
{
	return a < b ? a : b;
}