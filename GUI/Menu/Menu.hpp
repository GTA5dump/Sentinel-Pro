#pragma once
#include "../GUI.hpp"
#include "../../GTA/Script/Script.hpp"

namespace Sentinel::Menu
{
    // UI Variables
    enum eSubmenus
    {
        SubmenuSelf,
        SubmenuProtection,
        SubmenuPlayers,
        SubmenuNetwork,
        SubmenuRecovery,
        SubmenuWeapons,
        SubmenuVehicles,
        SubmenuSpawner,
        SubmenuTeleport,
        SubmenuMisc,
        SubmenuSettings
    };

    inline eSubmenus g_CurrentTab = SubmenuSelf;
    inline float g_PosX{ 250.f };
    inline float g_PosY{ 100.f };

    // Ticks
    extern void DXMain();
    extern void NativeMain();

    inline static void ScriptMain()
    {
        while (true)
        {
            NativeMain();
            Script::Current()->Yield();
        }
    }
}