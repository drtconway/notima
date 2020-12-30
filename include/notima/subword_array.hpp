#ifndef NOTIMA_SUBWORD_ARRAY_HPP
#define NOTIMA_SUBWORD_ARRAY_HPP

#include <vector>

namespace notima
{
    namespace detail
    {
        template <size_t Bits> struct word_sizes {};
        template <> struct word_sizes<8>  { using word_type = uint8_t; };
        template <> struct word_sizes<16> { using word_type = uint16_t; };
        template <> struct word_sizes<32> { using word_type = uint32_t; };
        template <> struct word_sizes<64> { using word_type = uint64_t; };

    }
    // namespace detail

    template <size_t ValueBits, size_t WordBits>
    struct subword_array
    {
        static_assert((WordBits % ValueBits) == 0);

        using word_type = typename detail::word_sizes<WordBits>::word_type;

        static constexpr size_t value_bits = ValueBits;
        static constexpr size_t ValsPerWord = WordBits / ValueBits;
        static constexpr uint64_t ValueMask = (1ULL << ValueBits) - 1;

        size_t value_count;
        std::vector<word_type> words;

        subword_array()
            : value_count(0)
        {
        }

        size_t size() const
        {
            return value_count;
        }

        subword_array& push_back(uint64_t p_x)
        {
            uint64_t w = size() / ValsPerWord;
            uint64_t b = size() % ValsPerWord;

            if (b == 0)
            {
                words.push_back(0);
            }
            words.back() |= (p_x & ValueMask) << (b * ValueBits);
            value_count += 1;
            return *this;
        }

        uint64_t operator[](size_t p_idx) const
        {
            uint64_t w = p_idx / ValsPerWord;
            uint64_t b = p_idx % ValsPerWord;

            return (words[w] >> (b*ValueBits)) & ValueMask;
        }
    };
}
// namespace notima

#endif // NOTIMA_SUBWORD_ARRAY_HPP
