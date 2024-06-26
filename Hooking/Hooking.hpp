#pragma once
#include "../Common.hpp"
#include "DetourHook.hpp"
#include "IATHook.hpp"
#include "VMTHook.hpp"
#include "../Utility/Joaat.hpp"
#include "../Utility/MinHook.hpp"

namespace Sentinel
{
    class Hooking final
    {
    private:
        Hooking();

    public:
        ~Hooking();

        Hooking(const Hooking&) = delete;
        Hooking(Hooking&&) noexcept = delete;
        Hooking& operator=(const Hooking&) = delete;
        Hooking& operator=(Hooking&&) noexcept = delete;

        /**
         * @brief Disable all hooks and do our cleanup
         *
         */
        static void Destroy();
        /**
         * @brief Create and enable our hooks
         *
         */
        static void Initialize();

        /**
         * @brief Get the Hook object
         *
         * @tparam T Detour Type
         * @param name Identifying name of the hook
         * @return T Hook instance
         */
        template<typename T>
        static T* GetHook(const std::string_view name);

        inline static WNDPROC m_OriginalWndProc;
    private:
        bool AddHook(std::unique_ptr<IHook>&& hook);

        void DestroyImpl();
        void InitImpl();

        static Hooking& GetInstance()
        {
            static Hooking i{};

            return i;
        }

    private:
        std::map<JoaatType, std::unique_ptr<IHook>> m_Hooks;

        bool m_Enabled;

        MinHook m_MinHook;

    };

    template<typename T>
    inline T* Hooking::GetHook(const std::string_view name)
    {
        const auto& i = GetInstance();
        const auto hash = Joaat(name);

        if (const auto& it = i.m_Hooks.find(hash); it != i.m_Hooks.end())
            return reinterpret_cast<T*>(it->second.get());
        return nullptr;
    }
}