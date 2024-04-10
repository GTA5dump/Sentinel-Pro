#include "Hooking.hpp"
#include "../Hooks/HookTable.hpp"
#include "../Pointers/Pointers.hpp"

namespace Sentinel
{
    Hooking::Hooking()
    {
        AddHook(std::make_unique<DetourHook>("SVM", (void*)Pointers::pScriptVM, (void*)GTA::ScriptVM));
        AddHook(std::make_unique<DetourHook>("INT", (void*)Pointers::pInitializeNativeTables, (void*)GTA::InitializeNativeTables));
        AddHook(std::make_unique<DetourHook>("REH", (void*)Pointers::pReceivedEventHandler, (void*)GTA::ReceivedEventHandler));
        AddHook(std::make_unique<DetourHook>("ATCP", (void*)Pointers::pAnimTaskCrashPatch, (void*)GTA::AnimTaskCrashPatch));
        AddHook(std::make_unique<DetourHook>("FCP", (void*)Pointers::pFragmentCrashPatch, (void*)GTA::FragmentCrashPatch));
        AddHook(std::make_unique<DetourHook>("FCP2", (void*)Pointers::pFragmentCrashPatch2, (void*)GTA::FragmentCrashPatch2));
        AddHook(std::make_unique<DetourHook>("IDCP", (void*)Pointers::pInvalidDecalCrashPatch, (void*)GTA::InvalidDecalCrashPatch));
        AddHook(std::make_unique<DetourHook>("ITCP", (void*)Pointers::pInfiniteTrainCrashPatch, (void*)GTA::InfiniteTrainCrashPatch));
        AddHook(std::make_unique<DetourHook>("BDSMCP", (void*)Pointers::pBDSMCrashPatch, (void*)GTA::BDSMCrashPatch));
        AddHook(std::make_unique<DetourHook>("KCP", (void*)Pointers::pKanyeCrashPatch, (void*)GTA::KanyeCrashPatch));
        AddHook(std::make_unique<DetourHook>("RPI", (void*)Pointers::pReceivePickup, (void*)GTA::ReceivePickup));
        AddHook(std::make_unique<DetourHook>("UPAI", (void*)Pointers::pUpdatePresenceAttributeInt, (void*)GTA::UpdatePresenceAttributeInt));
        AddHook(std::make_unique<DetourHook>("UPAS", (void*)Pointers::pUpdatePresenceAttributeString, (void*)GTA::UpdatePresenceAttributeString));
        AddHook(std::make_unique<DetourHook>("RCC", (void*)Pointers::pReceivedCloneCreate, (void*)GTA::ReceivedCloneCreate));
        AddHook(std::make_unique<DetourHook>("RCS", (void*)Pointers::pReceivedCloneSync, (void*)GTA::ReceivedCloneSync));
        AddHook(std::make_unique<DetourHook>("CAD", (void*)Pointers::pCanApplyData, (void*)GTA::CanApplyData));
        AddHook(std::make_unique<DetourHook>("API", (void*)Pointers::pAssignPhysicalIndex, (void*)GTA::AssignPhysicalIndex));
        AddHook(std::make_unique<DetourHook>("NPMI", (void*)Pointers::pNetPlayerMangerInit, (void*)GTA::NetworkPlayerManagerInitialize));
        AddHook(std::make_unique<DetourHook>("NPMD", (void*)Pointers::pNetPlayerMangerDest, (void*)GTA::NetworkPlayerManagerDestroy));
        AddHook(std::make_unique<DetourHook>("TJC", (void*)Pointers::pTaskJumpConstructor, (void*)GTA::TaskJumpConstructor));
        AddHook(std::make_unique<DetourHook>("FTC", (void*)Pointers::pFallTaskConstructor, (void*)GTA::FallTaskConstructor));
        AddHook(std::make_unique<DetourHook>("WPMDN", (void*)Pointers::pWritePedMovementDataNode, (void*)GTA::WritePedMovementDataNode));
        //AddHook(std::make_unique<DetourHook>("WPODN", (void*)Pointers::pWritePedOrientationDataNode, (void*)GTA::WritePedOrientationDataNode));
        AddHook(std::make_unique<DetourHook>("WPCDN", (void*)Pointers::pWritePedCreationDataNode, (void*)GTA::WritePedCreationDataNode));
        AddHook(std::make_unique<DetourHook>("SNE", (void*)Pointers::pSendNetworkEvent, (void*)GTA::SendNetworkEvent));
        AddHook(std::make_unique<DetourHook>("SCM", (void*)Pointers::pSendChatMessage, (void*)GTA::SendChatMessage));

        auto SwapChain = std::make_unique<VMTHook>("SC", reinterpret_cast<void***>(*Pointers::pSwapChain));
        SwapChain->Hook(SwapChain::VMTPresentIdx, (void*)SwapChain::Present);
        SwapChain->Hook(SwapChain::VMTResizeBuffersIdx, (void*)SwapChain::ResizeBuffers);
        AddHook(std::move(SwapChain));
        m_OriginalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(Pointers::pGameWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::WndProc)));
        LOG(INFO) << "[Hooking] Hooked: WD";
    }

    Hooking::~Hooking()
    {
        if (m_Enabled)
        {
            DestroyImpl();
        }
    }

    void Hooking::Destroy()
    {
        GetInstance().DestroyImpl();
    }

    void Hooking::DestroyImpl()
    {
        if (!m_Enabled)
            return;
        m_Enabled = false;

        SetWindowLongPtrW(Pointers::pGameWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_OriginalWndProc));
        LOG(INFO) << "[Hooking] Unhooked: WD";

        for (auto& hook : m_Hooks | std::views::values)
            hook->Disable();

        m_MinHook.ApplyQueued();
    }

    void Hooking::Initialize()
    {
        GetInstance().InitImpl();
    }

    void Hooking::InitImpl()
    {
        if (m_Enabled)
            return;
        m_Enabled = true;

        for (auto& hook : m_Hooks | std::views::values)
            hook->Enable();

        m_MinHook.ApplyQueued();
    }

    bool Hooking::AddHook(std::unique_ptr<IHook>&& hook)
    {
        return m_Hooks.insert({ Joaat(hook->Name()), std::move(hook) }).second;
    }
}