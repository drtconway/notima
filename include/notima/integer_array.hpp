#ifndef NOTIMA_INTEGER_ARRAY_HPP
#define NOTIMA_INTEGER_ARRAY_HPP

#include <notima/radix_array.hpp>
#include <notima/subword_array.hpp>

namespace notima
{
    namespace detail
    {
        template <size_t B>
        struct largest_storable
        {
            static constexpr bool exists = largest_storable<B-1>::exists;
            static constexpr size_t bits = largest_storable<B-1>::bits;
            using type = typename largest_storable<B-1>::type;
        };

        template <>
        struct largest_storable<0>
        {
            static constexpr bool exists = false;
            static constexpr size_t bits = 0;
            using type = void;
        };

        template <>
        struct largest_storable<1>
        {
            static constexpr bool exists = true;
            static constexpr size_t bits = 1;
            using type = notima::subword_array<1,64>;
        };

        template <>
        struct largest_storable<2>
        {
            static constexpr bool exists = true;
            static constexpr size_t bits = 2;
            using type = notima::subword_array<2,64>;
        };

        template <>
        struct largest_storable<4>
        {
            static constexpr bool exists = true;
            static constexpr size_t bits = 4;
            using type = notima::subword_array<4,64>;
        };

        template <>
        struct largest_storable<8>
        {
            static constexpr bool exists = true;
            static constexpr size_t bits = 8;
            using type = std::vector<uint8_t>;
        };

        template <>
        struct largest_storable<16>
        {
            static constexpr bool exists = true;
            static constexpr size_t bits = 16;
            using type = std::vector<uint16_t>;
        };

        template <>
        struct largest_storable<32>
        {
            static constexpr bool exists = true;
            static constexpr size_t bits = 32;
            using type = std::vector<uint32_t>;
        };

        template <>
        struct largest_storable<64>
        {
            static constexpr bool exists = true;
            static constexpr size_t bits = 64;
            using type = std::vector<uint64_t>;
        };
    }
    // namespace detail

    template <size_t Bits, typename Largest = detail::largest_storable<Bits>>
    struct integer_array
        : std::conditional<
            Largest::bits == Bits,
            typename Largest::type,
            radix_array<
                typename Largest::type,
                integer_array<Bits - Largest::bits>>>::type
    {
    };
}
// namespace notima

#endif // NOTIMA_INTEGER_ARRAY_HPP
