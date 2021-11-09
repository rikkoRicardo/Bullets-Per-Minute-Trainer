#include "stdafx.h"
#include "proc.h"
#include "mem.h"
#include <iostream>

#define EXIT_KEY VK_NUMPAD0
#define HEALTH_KEY VK_NUMPAD1
#define AMMO_KEY VK_NUMPAD2
#define COINS_KEY VK_NUMPAD6
#define HITKILL_KEY VK_NUMPAD4
#define SUPERJUMP_KEY VK_NUMPAD5
#define GRAVITY_KEY VK_NUMPAD6
#define WALKSPEED_PLUS_KEY VK_OEM_PLUS
#define WALKSPEED_MINUS_KEY VK_OEM_MINUS

int main() {

	bool b_health = 0, b_ammo = 0,
	b_abilitytime = 0, b_1hit = 0, 
	b_superjump = 0, b_gravity = 0,b_money = 0;

	uintptr_t MoneyOffset = 0, DynamicptrBaseAdress = 0;

	HANDLE hProc = 0;
	DWORD procId = GetProcId(L"BPMGame-Win64-Shipping.exe");;
	uintptr_t modBase = GetModuleBaseAdress(procId, L"BPMGame-Win64-Shipping.exe");

	if (procId) {
		hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
		DynamicptrBaseAdress = (uintptr_t)0x00007FF6079B0000;
		//MoneyOffset = 
		//Prefered imagebase = 00007FF6079B0000 
		std::cout <<"modBase  - 0x " << std::hex <<  modBase << std::endl;
		
		if (procId == NULL) {
			std::cout << "ProcID not Found! Click Anywhere to continue" << std::endl;
		}
		else {
			std::cout << "ProcID: " << std::dec <<  procId << std::endl;
		}

	}
	else {
		std::cout << "Process Not Found!" << std::endl;
		CloseHandle(hProc);
		std::cin.get();
	}

	DWORD dwExit = 0;

	while (GetExitCodeProcess(hProc, &dwExit) && dwExit == STILL_ACTIVE) {
		// bhealth = 0, bammo = 0, bcoins = 0, babilitytime = 0, b1hit = 0, bsuperjump, bgravity;

		if (GetAsyncKeyState(HEALTH_KEY) & 0x01)//Numpad1
		{
			BYTE* healthOffset = (BYTE*)modBase + 0xB54D6B;
			
			b_health = !b_health;
			
			if (b_health) 
			{
				//addss xmm3,xmm4
				mem::PatchEx(hProc, healthOffset, (BYTE*)"\xF3\x0F\x58\xDC",4);
				std::cout << "Health activated!" << std::endl;
			}
			else
			{
				//subss xmm3,xmm4
				mem::PatchEx(hProc, healthOffset, (BYTE*)"\xF3\x0F\x5C\xDC", 4);
				std::cout << "Health Dactivated!" << std::endl;
			}
		}

		if (GetAsyncKeyState(AMMO_KEY) & 0x01)//Numpad2
		{
			BYTE* AmmoOffset = (BYTE*)modBase + 0xB64533;

			b_ammo = !b_ammo;

			if (b_ammo)
			{ 
				mem::NopEx(hProc, AmmoOffset, 3);
				std::cout << "Ammo activated!" << std::endl;
			}
			else
			{
				//sub ecx,0x01         
				mem::PatchEx(hProc, AmmoOffset, (BYTE*)"\x83\xE9\x00", 3);
				std::cout << "Ammo Dactivated!" << std::endl;
			}
		}

		if (GetAsyncKeyState(COINS_KEY)&0x01)//Numpad3
		{           
			//---- When the game subtracts
			//"BPMGame-Win64-Shipping.exe"+B5BAA7 - Change from sub to nothing - 7FF60850BAA7 - 83 EA 00 
			mem::PatchEx(hProc, (BYTE*)modBase + 0xB5BAA7, (BYTE*)"\x83\xEA\x00", 3); // Coins
			//"BPMGame-Win64-Shipping.exe"+B42FBC - Change from sub to nothing 
			mem::PatchEx(hProc, (BYTE*)modBase + 0xB5BA9C, (BYTE*)"\x83\xE8\x00", 3); // Keys 
			std::cout << "Infinite Resources!" << std::endl;
			}
		}

		if (GetAsyncKeyState(HITKILL_KEY) & 0x01)//Numpad5
		{
			b_1hit = !b_1hit;
			if (b_1hit)
			{
				//mov [rsi+000004C8],00000000
				std::cout << "YES - 1hit!" << std::endl;
				mem::PatchEx(hProc, (BYTE*)modBase + 0xB14D47, (BYTE*)"\xC7\x86\xC8\x04\x00\x00\x00\x00\x00\x00", 11);
			}
			else
			{
				//movss [rsi+000004C8],xmm0
				std::cout << "NO - 1hit!" << std::endl;
				mem::PatchEx(hProc, (BYTE*)modBase + 0xB14D47, (BYTE*)"\xF3\x0F\x11\x86\xC8\x04\x00\x00", 8);
			}
		}
		if (GetAsyncKeyState(EXIT_KEY) & 0x01)//Numpad0
		{
			return 0;
		}
		Sleep(10);
	CloseHandle(hProc);
	return 0;
}
