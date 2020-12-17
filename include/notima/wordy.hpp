#ifndef NOTIMA_WORDY_HPP
#define NOTIMA_WORDY_HPP

namespace notima
{
    namespace detail
    {
        template <int k, int j>
        struct Lbits
        {
            static constexpr uint64_t value = (Lbits<k,j+k>::value << k) | 0x0000000000000001ULL;
        };

        template <int k>
        struct Lbits<k,64>
        {
            static constexpr uint64_t value = 0ULL;
        };
    }
    // namespace detail

    struct wordy
    {
        template <int k> static constexpr uint64_t L = detail::Lbits<k,0>::value;
        template <int k> static constexpr uint64_t H = L<k> << (k - 1);

        template <int k>
        static uint64_t lt(uint64_t x, uint64_t y)
        {
            return (((x | H<k>) - (y & ~H<k>)) ^ x ^ ~y) & H<k>;
        }

        template <int k>
        static uint64_t le(uint64_t x, uint64_t y)
        {
            return (((y | H<k>) - (x & ~H<k>)) ^ x ^ y) & H<8>;
        }

        static uint64_t popcount(uint64_t x)
        {
            x = x - ((x >> 1) & 0x5555555555555555ull);
            x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
            x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
            return x * L<8> >> 56;
        }

        static uint64_t rank(uint64_t x, uint64_t i)
        {
            // do some fiddling to handle i == 64
            // because the C & C++ standards allow undefined results
            // for shifting by >= word size.
            uint64_t w = (i & 64) >> 6;
            uint64_t z = 1 - w;
            uint64_t y = z * (1ULL << i);
            return popcount(x & (y - 1));
        }

        static uint64_t select(uint64_t x, uint64_t r)
        {
            uint64_t s0 = x - ((x & 0xAAAAAAAAAAAAAAAAULL) >> 1);
            uint64_t s1 = (s0 & 0x3333333333333333ULL) + ((s0 >> 2) & 0x3333333333333333ULL);
            uint64_t s2 = ((s1 + (s1 >> 4)) & 0x0F0F0F0F0F0F0F0FULL) * L<8>;
            uint64_t b  = (((le<8>(s2, r * L<8>) >> 7) * L<8>) >> 53) & ~0x7ULL;
            uint64_t l  = r - (((s2 << 8) >> b) & 0xFF);
            uint64_t s3 = (lt<8>(0x0, ((x >> b & 0xFF) * L<8>) & 0x8040201008040201ULL) >> 0x7) * L<8>;
            return b + (((le<8>(s3, l * L<8>) >> 7) * L<8>) >> 56);
        }

    };
}
// namespace notima

#endif // NOTIMA_WORDY_HPP
