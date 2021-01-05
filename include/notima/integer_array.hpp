#ifndef NOTIMA_INTEGER_ARRAY_HPP
#define NOTIMA_INTEGER_ARRAY_HPP

#include <notima/radix_array.hpp>
#include <notima/subword_array.hpp>

namespace notima
{
    namespace detail
    {
        template <size_t... Xs>
        struct sizes
        {
        };

        template <size_t B>
        struct biggest
        {
            static constexpr size_t value = biggest<B-1>::value;
            using type = typename biggest<B-1>::type;
        };

        template <>
        struct biggest<1>
        {
            static constexpr size_t value = 1;
            using type = notima::subword_array<1,64>;
        };
        template <>
        struct biggest<2>
        {
            static constexpr size_t value = 2;
            using type = notima::subword_array<2,64>;
        };
        template <>
        struct biggest<4>
        {
            static constexpr size_t value = 4;
            using type = notima::subword_array<4,64>;
        };
        template <>
        struct biggest<8>
        {
            static constexpr size_t value = 8;
            using type = std::vector<uint8_t>;
        };
        template <>
        struct biggest<16>
        {
            static constexpr size_t value = 16;
            using type = std::vector<uint16_t>;
        };
        template <>
        struct biggest<32>
        {
            static constexpr size_t value = 32;
            using type = std::vector<uint32_t>;
        };
        template <>
        struct biggest<64>
        {
            static constexpr size_t value = 64;
            using type = std::vector<uint64_t>;
        };

        template <typename T, typename U>
        struct compose
        {
        };

        template <typename T, typename... Ts>
        struct compose<T, std::tuple<Ts...>>
        {
            using type = std::tuple<T, Ts...>;
        };

        template <size_t B>
        struct repack
        {
            static constexpr size_t S = biggest<B>::value;
            static constexpr size_t R = B - S;

            using self = typename biggest<B>::type;
            using rest = typename repack<R>::type;
            using type = typename compose<self, rest>::type;
        };

        template <>
        struct repack<0>
        {
            using type = std::tuple<>;
        };

        template <typename T>
        struct tuple_to_radix
        {
        };

        template <typename T>
        struct tuple_to_radix<std::tuple<T>>
        {
            using type = T;
        };

        template <typename T1, typename T2, typename... Ts>
        struct tuple_to_radix<std::tuple<T1,T2,Ts...>>
        {
            using type = notima::radix_array<T1,T2,Ts...>;
        };

        template <size_t B>
        struct make_radix_array_type
        {
            using tuple_type = typename repack<B>::type;
            using type = typename tuple_to_radix<tuple_type>::type;
        };
    }
    // namespace detail

    template <size_t Bits,
              typename ArrayType = typename detail::make_radix_array_type<Bits>::type>
    struct integer_array : ArrayType
    {
    };

}
// namespace notima

#endif // NOTIMA_INTEGER_ARRAY_HPP
