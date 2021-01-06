#ifndef NOTIMA_BIT_ARRAY_HPP
#define NOTIMA_BIT_ARRAY_HPP

#include <iostream>
#include <vector>

namespace notima
{
    struct bit_array
    {
        size_t size_;
        std::vector<uint64_t> words;

        bit_array()
            : size_(0)
        {}

        size_t size() const
        {
            return size_;
        }

        bit_array& push_back(bool p_x)
        {
            size_t idx = size_;
            size_t w = idx / 64;
            size_t p = idx % 64;
            if (p == 0)
            {
                words.push_back(0);
            }
            words.back() |= static_cast<uint64_t>(p_x) << p;
            size_ += 1;
            return *this;
        }

        bit_array& push_back(size_t p_width, uint64_t p_x)
        {
            if (p_width == 0)
            {
                return *this;
            }

            const uint64_t M = (1ULL << p_width) - 1;
            size_t idx0 = size_;
            size_t idx1 = size_ + p_width;
            size_t w0 = idx0 / 64;
            size_t p0 = idx0 % 64;
            size_t w1 = idx1 / 64;
            size_t p1 = idx1 % 64;
            if (p0 == 0)
            {
                words.push_back(0);
            }
            p_x &= M;
            if (w0 == w1 || p1 == 0)
            {
                words.back() |= p_x << p0;
            }
            else
            {
                size_t b0 = 64 - p0;
                words.back() |= p_x << p0;
                words.push_back(p_x >> b0);
            }
            size_ += p_width;
            return *this;
        }

        uint64_t operator[](size_t p_idx) const
        {
            size_t w = p_idx / 64;
            size_t p = p_idx % 64;
            return (words[w] >> p) & 1;
        }

        uint64_t operator[](const std::tuple<size_t,size_t>& p_range) const
        {
            uint64_t idx0;
            uint64_t idx1;
            std::tie(idx0, idx1) = p_range;

            size_t w0 = idx0 / 64;
            size_t p0 = idx0 % 64;
            size_t w1 = idx1 / 64;
            size_t p1 = idx1 % 64;
            if (w0 == w1 || p1 == 0)
            {
                size_t b = idx1 - idx0;
                uint64_t m = (1ULL << b) - 1;
                return (words[w0] >> p0) & m;
            }
            else
            {
                size_t b0 = 64 - p0;
                size_t b1 = p1;
                uint64_t m1 = (1ULL << p1) - 1;
                return (words[w0] >> p0) | ((words[w1] & m1) << b0);
            }
        }

        static std::string hex(uint64_t x)
        {
            std::string rev;
            do
            {
                rev.push_back("0123456789abcdef"[x & 0xf]);
                x >>= 4;
            }
            while (x > 0);

            std::string res("0x");
            res.insert(res.end(), rev.rbegin(), rev.rend());
            return res;
        }
    };
}
// namespace notima

#endif // NOTIMA_BIT_ARRAY_HPP
