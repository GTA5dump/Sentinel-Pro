#include "Joaat.hpp"

namespace Sentinel
{
    /**
     * @brief Generate a 32bit hash from a string
     */
    constexpr JoaatType Joaat(const std::string_view b)
    {
        JoaatType result = 0;

        for (auto c : b)
        {
            result += ToLower(c);
            result += (result << 10);
            result ^= (result >> 6);
        }

        result += (result << 3);
        result ^= (result >> 11);
        result += (result << 15);

        return result;
    }

    /**
     * @brief Generate a 64bit hash from a string
     */
    constexpr LongJoaatType LongJoaat(const std::string_view b)
    {
        LongJoaatType result = 0;

        for (auto c : b)
        {
            result += ToLower(c);
            result += (result << 20);
            result ^= (result >> 12);
        }

        result += (result << 6);
        result ^= (result >> 22);
        result += (result << 30);

        return result;
    }
}