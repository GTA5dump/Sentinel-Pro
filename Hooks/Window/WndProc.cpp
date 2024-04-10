#include "../HookTable.hpp"
#include "../../Renderer/Renderer.hpp"

namespace Sentinel
{
    LRESULT Window::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
    {
        if (gRunning) 
        {
            Renderer::WndProc(hwnd, umsg, wparam, lparam);
        }

        if (umsg == WM_KEYUP && wparam == VK_DELETE) // Fallback to unload
            gRunning = false;

        return CallWindowProcW(Hooking::m_OriginalWndProc, hwnd, umsg, wparam, lparam);
    }
}