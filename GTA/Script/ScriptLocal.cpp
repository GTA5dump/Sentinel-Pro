#include "ScriptLocal.hpp"
#include <cstdint>

namespace Sentinel
{
    ScriptLocal::ScriptLocal(void* stack, std::size_t idx) :
        m_Stack(stack),
        m_Index(idx)
    {

    }

    ScriptLocal ScriptLocal::Add(std::ptrdiff_t offset)
    {
        return { m_Stack, m_Index + offset };
    }

    ScriptLocal ScriptLocal::At(std::ptrdiff_t idx, std::size_t size)
    {
        return { m_Stack, m_Index + 1 + (idx * size) };
    }

    void* ScriptLocal::Address() const
    {
        return reinterpret_cast<void*>(std::uintptr_t(m_Stack) + m_Index * sizeof(std::uintptr_t));
    }
}