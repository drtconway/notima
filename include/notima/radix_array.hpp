#ifndef NOTIMA_RADIX_ARRAY_HPP
#define NOTIMA_RADIX_ARRAY_HPP

#include <type_traits>

namespace notima
{
    namespace detail
    {
        template <size_t N>
        struct nonzero
        {
            static constexpr bool value = true;
        };

        template <>
        struct nonzero<0>
        {
            static constexpr bool value = false;
        };

        struct bits_from_value_type
        {
            template <typename ArrayType>
            static constexpr typename std::enable_if<nonzero<ArrayType::value_bits>::value, size_t>::type value()
            {
                return ArrayType::value_bits;
            }

            template <typename ArrayType>
            static constexpr typename std::enable_if<nonzero<sizeof(typename ArrayType::value_type)>::value, size_t>::type value()
            {
                return 8*sizeof(typename ArrayType::value_type);
            }
        };

        template <typename ArrayType>
        struct array_value_bits
        {
            static constexpr size_t value = bits_from_value_type::value<ArrayType>();

        };
    }
    // namespace detail

    template <typename... Arrays>
    struct radix_array
    {
    };

    template <typename Array>
    struct radix_array<Array>
    {
        Array self;

        size_t size() const
        {
            return self.size();
        }

        radix_array<Array>& push_back(uint64_t p_x)
        {
            self.push_back(p_x);
            return *this;
        }

        uint64_t operator[](size_t p_idx) const
        {
            return self[p_idx];
        }
    };

    template <typename Array1, typename Array2, typename... Arrays>
    struct radix_array<Array1, Array2, Arrays...>
    {
        static constexpr size_t bits1 = detail::array_value_bits<Array1>::value;
        static constexpr uint64_t mask1 = (1ULL << bits1) - 1;

        Array1 self;
        radix_array<Array2, Arrays...> rest;

        size_t size() const
        {
            return self.size();
        }

        radix_array<Array1, Array2, Arrays...>& push_back(uint64_t p_x)
        {
            self.push_back(p_x & mask1);
            rest.push_back(p_x >> bits1);
            return *this;
        }

        uint64_t operator[](size_t p_idx) const
        {
            return (rest[p_idx] << bits1) | self[p_idx];
        }
    };
}
// namespace notima

#endif // NOTIMA_RADIX_ARRAY_HPP
