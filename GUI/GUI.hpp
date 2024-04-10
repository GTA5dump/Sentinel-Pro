#pragma once
#include "../Common.hpp"

namespace Sentinel
{
    class GUI final
    {
    private:
        GUI();

    public:
        ~GUI();

        GUI(const GUI&) = delete;
        GUI(GUI&&) noexcept = delete;
        GUI& operator=(const GUI&) = delete;
        GUI& operator=(GUI&&) noexcept = delete;

        static void Initialize()
        {
            GetInstance();
        }

        static bool IsOpen()
        {
            return GetInstance().m_IsOpen;
        }

        static bool IsTyping()
        {
            return GetInstance().m_IsTyping;
        }

        static void SetTyping()
        {
            GetInstance().m_IsTyping = true;
        }

        static void SetDoneTyping()
        {
            GetInstance().m_IsTyping = false;
        }

    private:
        void WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

        static void ToggleMouse();
        static GUI& GetInstance()
        {
            static GUI i{};

            return i;
        }

    private:
        bool m_IsOpen;
        bool m_IsTyping;

    };
}