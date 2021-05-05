#include <iostream>
#include <vector>
#include <Windows.h>
#include "stdafx.h"
#include "proc.h"
#include "mem.h"

int main()
{
 
    HANDLE hProcess = 0;
    uintptr_t moduleBase = 0, localPlayerPtr = 0, healthAddr = 0, moneyAddr = 0;
    bool bHealth = false, bExperience = false, bG = false, b1Hitk0 = false;

    const int newValue = 99999;
    const int maxMoney = 999999;

    DWORD procId = GetProcId(L"NieRAutomata.exe");

    //if game is running and found
    if (procId) 
    {
        //start reading and write to memory region
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

        //establish contact with program
        moduleBase = GetModuleBaseAddress(procId, L"NieRAutomata.exe");

        //local player
        localPlayerPtr = moduleBase + 0x16053F0;

        //offsets
        std::vector<unsigned int> healthOffset = { 0x10668 };
        std::vector<unsigned int> moneyOffset = { 0x10F6188 };
        
        //find the exact health addy
        healthAddr = FindDMAAddy(hProcess, localPlayerPtr, healthOffset);
        moneyAddr = FindDMAAddy(hProcess, localPlayerPtr, moneyOffset);
    }

    else
    {
        std::cout << "\nProcess not found, press enter to exit\n";
        getchar();
        return 0;
    }

    DWORD dwExit = 0;
    
    std::cout << "\n";
    std::cout << "********************Features****************\n";
    std::cout << "****   Numpad 1: Infinite Health Hack   ****\n";
    std::cout << "****   Numpad 2: 1HITK0 (NOT WORKING)   ****\n";
    std::cout << "****   Numpad 3: NOT IMPLEMENTED        ****\n";
    std::cout << "****   Numpad 4: Infinte Money Hack     ****\n";
    std::cout << "********************************************\n";

    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE) 
    {
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bHealth = !bHealth;
            std::cout << "\n\t\t\t***** INFINTE HEALTH HACK ACTIVATED! *****\n";
        }

      if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            b1Hitk0 = !b1Hitk0;
            std::cout << "\n\t\t\t***** ONE HIT K0 HACK ACTIVATED!     *****\n";

            if (b1Hitk0)
            {
                //mov [rdi+00000858],#0
                mem::PatchEx((BYTE*)(moduleBase + 0x34748A6), (BYTE*)"\xE9\x55\xB7\x1F\x03\x90", 7, hProcess);
            }
            else
            {
                //sub [rdi+00000858],esi
                mem::PatchEx((BYTE*)(moduleBase + 0x34748A6), (BYTE*)"\x29\xB7\x58\x08\x00\x00", 7, hProcess);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bExperience = !bExperience;
            
            if (bExperience)
            {
                std::cout << "\n\tExperience Hack Selected - *****NOT IMPLEMENTED YET******";
                break;
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD4) & 1)
        {
            bG = !bG;
            if (bG)
            {
                std::cout << "\n\t****  INFINTE MONEY HACK ACTIVATED! *****";
            }
        }

        //continuous write or freeze
        if (bHealth)
        {
            mem::PatchEx((BYTE*)healthAddr, (BYTE*)&newValue, sizeof(newValue), hProcess);
        }
        if (bG)
        {
            mem::PatchEx((BYTE*)moneyAddr, (BYTE*)&maxMoney, sizeof(maxMoney), hProcess);
        }

        Sleep(10);
    }

    std::cout << "Process not found, press enter to exit";
    getchar();
    return 0;

    ////Get ProcId of the Target Process
    //DWORD procId = GetProcId(L"NieRAutomata.exe");

    ////Get Module Base Address
    //uintptr_t moduleBase = GetModuleBaseAddress(procId, L"NieRAutomata.exe");

    ////Get Handle to Process
    //HANDLE hProcess = 0;
    //hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    ////Resolve Base Address of the Pointer Chain
    //uintptr_t dynamicPtrBaseAddr = moduleBase + 0x016AFD18;

    //std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

    ////Resolve our health
    //std::vector<unsigned int> healthOffsets = { 0x60, 0x578, 0x28, 0x8, 0x190, 0xb0, 0x548 };
    //uintptr_t healthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, healthOffsets);

    //std::cout << "HealthAddr = " << "0x" << std::hex << healthAddr << std::endl;

    ////Read health value
    //int healthValue = 0;

    //ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);
    //std::cout << "Current Health = " << std::dec << healthValue << std::endl;

    ////Write to It
    //int newHealth = 500;
    //WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newHealth, sizeof(newHealth), nullptr);

    ////Read Out Again
    //ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);

    //std::cout << "New Health: " << std::dec << healthValue << std::endl;
}