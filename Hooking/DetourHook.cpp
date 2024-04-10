#include "../Common.hpp"
#include "DetourHook.hpp"
#include "../Utility/MinHook.hpp"

namespace Sentinel
{
    DetourHook::DetourHook(const std::string_view name, void* target, void* detour) :
        m_Name(name),
        m_Target(target),
        m_Detour(detour),
        m_Original(nullptr),
        m_IsEnabled(false)
    {

        if (const auto result = MH_CreateHook(m_Target, m_Detour, &m_Original); result != MH_OK)
        {
            throw std::runtime_error("Failed to create hook");
        }
    }

    DetourHook::~DetourHook()
    {
        if (m_Target)
        {
            MH_RemoveHook(m_Target);
        }
    }

    const std::string_view DetourHook::Name() const
    {
        return m_Name;
    }

    bool DetourHook::Enable()
    {
        if (m_IsEnabled)
            return false;
        if (const auto Result = MH_QueueEnableHook(m_Target); Result != MH_OK)
        {
            LOG(WARNING) << "[Hooking] Failed to queue hook " << m_Name << " to be enabled";

            return false;
        }

        LOG(INFO) << "[Hooking] Hooked: " << m_Name;

        m_IsEnabled = true;
        return true;
    }

    bool DetourHook::Disable()
    {
        if (!m_IsEnabled)
            return false;
        if (const auto Result = MH_QueueDisableHook(m_Target); Result != MH_OK)
        {
            LOG(WARNING) << "[Hooking] Failed to queue hook " << m_Name << " to be disabled";

            return false;
        }

        LOG(INFO) << "[Hooking] Unhooked: " << m_Name;

        m_IsEnabled = false;
        return true;
    }

    bool DetourHook::EnableNow()
    {
        if (m_IsEnabled)
            return false;
        if (const auto Result = MH_EnableHook(m_Target); Result != MH_OK)
        {
            LOG(WARNING) << "[Hooking] Failed to enable hook " << m_Name;

            return false;
        }

        m_IsEnabled = true;
        return true;
    }

    bool DetourHook::DisableNow()
    {
        if (m_IsEnabled)
            return false;
        if (const auto Result = MH_DisableHook(m_Target); Result != MH_OK)
        {
            LOG(WARNING) << "[Hooking] Failed to disable hook " << m_Name;

            return false;
        }

        m_IsEnabled = true;
        return true;
    }

    bool DetourHook::IsEnabled() const
    {
        return m_IsEnabled;
    }
}