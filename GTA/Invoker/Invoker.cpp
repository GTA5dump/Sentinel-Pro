#include "CrossMap.hpp"
#include "Invoker.hpp"
#include "../../Pointers/Pointers.hpp"

extern "C" void	_call_asm(void* context, void* function, void* ret);
namespace Sentinel
{
    Invoker::NativeCallContext::NativeCallContext()
    {
        m_return_value = &m_ReturnStack[0];
        m_args = &m_ArgStack[0];
    }

    void Invoker::CacheHandlersImpl()
    {
        for (const auto [firstHash, currHash] : gCrossMap)
        {
            auto handler = Pointers::pGetNativeHandler(Pointers::pNativeRegistrationTable, currHash);

            m_HandlerCache.emplace(firstHash, handler);
        }

        m_HandlersCached = true;
    }

    void Invoker::BeginCallImpl()
    {
        m_CallContext.reset();
    }

    void Invoker::EndCallImpl(rage::scrNativeHash hash)
    {
        if (const auto& Iterator = m_HandlerCache.find(hash); Iterator != m_HandlerCache.end())
        {
            rage::scrNativeHandler Handler = Iterator->second;

            _call_asm(&m_CallContext, Handler, Pointers::pNativeReturnAddress);
            Pointers::pFixVectors(&m_CallContext);
        }
        else
        {
            [hash]() { LOG(WARNING) << "[Invoker] Failed to find " << HEX_TO_UPPER(hash) << " Native Handler"; }();
        }
    }
}
