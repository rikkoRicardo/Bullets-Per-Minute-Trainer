#include "proc.h"
#include "stdafx.h"

DWORD GetProcId(const wchar_t* procName) {
	DWORD procID = 0;
	HANDLE hSnap = 0;

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(procEntry);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		if (Process32First(hSnap, &procEntry)) {
			do {
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procID = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procID;
}

uintptr_t GetModuleBaseAdress(DWORD procID, const wchar_t* modName) {
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = 0;

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,procID);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(hSnap, &modEntry)) {
			do {

				if (!_wcsicmp(modEntry.szModule, modName)) {
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}

			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t GetDMAAddy(HANDLE hSnap, uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t finalAddy = ptr;

	for (unsigned int i = 0; i< offsets.size(); ++i) {
		WriteProcessMemory(hSnap, (BYTE*)finalAddy, &finalAddy, sizeof(finalAddy), 0);
		std::cout <<std::hex << finalAddy << std::endl;
		finalAddy += offsets[i];
	}
	return finalAddy;
}