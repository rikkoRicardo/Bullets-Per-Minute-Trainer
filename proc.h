#pragma once
#include "stdafx.h"

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAdress(DWORD procID, const wchar_t* modName);

uintptr_t GetDMAAddy(HANDLE hSnap, uintptr_t ptr, std::vector<unsigned int> offsets);
