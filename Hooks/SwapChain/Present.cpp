#include "../HookTable.hpp"
#include "../../Renderer/Renderer.hpp"

namespace Sentinel
{
    HRESULT SwapChain::Present(IDXGISwapChain* that, UINT syncInterval, UINT flags)
    {
        if (gRunning)
            Renderer::OnPresent();
        return Hooking::GetHook<VMTHook>("SC")->Original<decltype(&Present)>(SwapChain::VMTPresentIdx)(that, syncInterval, flags);
    }
}