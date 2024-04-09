#pragma once
#include <cstdint>
#include <string_view>

namespace Sentinel
{
    using JoaatType = std::uint32_t;
    using LongJoaatType = std::uint64_t;

    inline constexpr char ToLower(char c)
    {
        return c >= 'A' && c <= 'Z' ? c | 1 << 5 : c;
    }

    /**
     * @brief Generate a 32bit hash from a string
     */
    extern constexpr JoaatType Joaat(const std::string_view b);
    /**
     * @brief Generate a 64bit hash from a string
     */
    extern constexpr LongJoaatType LongJoaat(const std::string_view b);

    inline consteval JoaatType operator ""_J(const char* s, std::size_t n)
    {
        JoaatType result = 0;

        for (std::size_t i = 0; i < n; i++)
        {
            result += ToLower(s[i]);
            result += (result << 10);
            result ^= (result >> 6);
        }

        result += (result << 3);
        result ^= (result >> 11);
        result += (result << 15);

        return result;
    }
    inline consteval LongJoaatType operator ""_LJ(const char* s, std::size_t n)
    {
        LongJoaatType result = 0;

        for (std::size_t i = 0; i < n; i++)
        {
            result += ToLower(s[i]);
            result += (result << 20);
            result ^= (result >> 12);
        }

        result += (result << 6);
        result ^= (result >> 22);
        result += (result << 30);

        return result;
    }
}