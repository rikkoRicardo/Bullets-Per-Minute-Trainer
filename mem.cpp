#include "mem.h"
#include "stdafx.h"

void mem::PatchEx(HANDLE hProc, BYTE* dst, BYTE* src, unsigned int size) {
	DWORD oldprotection;
	VirtualProtectEx(hProc, dst, size, PAGE_EXECUTE_READWRITE, &oldprotection);
	WriteProcessMemory(hProc, dst, src, size, nullptr);
	VirtualProtectEx(hProc, dst, size, oldprotection, &oldprotection);
}

void mem::NopEx(HANDLE hProc, BYTE* dst, unsigned int size) {
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);

	PatchEx(hProc, dst, nopArray, size);
}