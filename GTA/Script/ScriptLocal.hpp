#pragma once
#include <cstddef>

namespace Sentinel
{
    class ScriptLocal
    {
    public:
        ScriptLocal(void* stack, std::size_t idx);

        /**
         * @brief Add an offset to a global
         *
         * @param offset Offset value
         * @return ScriptGlobal
         */
        ScriptLocal Add(std::ptrdiff_t offset);

        /**
         * @brief Get an idx in an array of a certain struct size
         *
         * @param idx Idx to access
         * @param size Size of the struct in question
         * @return ScriptGlobal
         */
        ScriptLocal At(std::ptrdiff_t idx, std::size_t size);

        /**
         * @brief Get the address of the global in question
         *
         * @return void*
         */
        void* Address() const;

        /**
         * @brief Cast the Script Global to any type
         *
         * @tparam T The object to cast the pointer to
         * @return T* The cast object pointer
         */
        template<typename T>
        T As() const
        {
            return reinterpret_cast<T>(Address());
        }

    private:
        void* m_Stack;
        std::size_t m_Index;

    };
}