#ifndef NOTIMA_KMERS_HPP
#define NOTIMA_KMERS_HPP

//#include <bit>
#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

namespace notima
{
    using kmer = uint64_t;

    struct kmers
    {
        static std::string render(const size_t p_k, kmer p_x)
        {
            std::string res;
            render(p_k, p_x, res);
            return res;
        }

        static void render(const size_t p_k, kmer p_x, std::string& p_str)
        {
            p_str.clear();
            for (size_t i = 0; i < p_k; ++i)
            {
                p_str.push_back("ACGT"[p_x&3]);
                p_x >>= 2;
            }
            std::reverse(p_str.begin(), p_str.end());
        }

        static bool base(char p_b, kmer& p_x)
        {
            switch (p_b)
            {
                case 'a':
                case 'A':
                {
                    p_x = 0;
                    return true;
                }
                case 'c':
                case 'C':
                {
                    p_x = 1;
                    return true;
                }
                case 'g':
                case 'G':
                {
                    p_x = 2;
                    return true;
                }
                case 't':
                case 'T':
                case 'u':
                case 'U':
                {
                    p_x = 3;
                    return true;
                }
                default:
                {
                    return false;
                }
            }
        }

        static kmer rev(kmer p_x)
        {
            static constexpr kmer m1 = 0x5555555555555555ULL; // 01010101...
            static constexpr kmer m2 = 0x3333333333333333ULL; // 00110011...
            static constexpr kmer m3 = 0x0F0F0F0F0F0F0F0FULL; // 00001111...
            static constexpr kmer m4 = 0x00FF00FF00FF00FFULL; // {0^8}{1^8}...
            static constexpr kmer m5 = 0x0000FFFF0000FFFFULL; // {0^16}{1^16}...
            static constexpr kmer m6 = 0x00000000FFFFFFFFULL; // {0^32}{1^32}
            p_x = ((p_x >> 2) & m2) | ((p_x & m2) << 2);
            p_x = ((p_x >> 4) & m3) | ((p_x & m3) << 4);
            p_x = ((p_x >> 8) & m4) | ((p_x & m4) << 8);
            p_x = ((p_x >> 16) & m5) | ((p_x & m5) << 16);
            p_x = ((p_x >> 32) & m6) | ((p_x & m6) << 32);
            return p_x;
        }

        static kmer rc(const size_t p_k, kmer p_x)
        {
            return rev(~p_x) >> (64 - 2*p_k);
        }
        
        //static size_t ham(x, y)
        //{
        //    static constexpr kmer m1 = 0x5555555555555555ULL # 01010101...
        //    kmer z = x ^ y;
        //    kmer v = (z | (z >> 1)) & m1;
        //    return std::popcount(v)
        //}

        static void make(const size_t p_k,
                         const std::string& p_seq,
                         std::vector<kmer>& p_fwd, std::vector<kmer>& p_rev)
        {
            make(p_k, p_seq.begin(), p_seq.end(), [&](const kmer& x, const kmer& xb) {
                p_fwd.push_back(x);
                p_rev.push_back(xb);
            });
        }

        static void make(const size_t p_k,
                         const std::string_view& p_seq,
                         std::vector<kmer>& p_fwd, std::vector<kmer>& p_rev)
        {
            make(p_k, p_seq.begin(), p_seq.end(), [&](const kmer& x, const kmer& xb) {
                p_fwd.push_back(x);
                p_rev.push_back(xb);
            });
        }

        template <typename Iterator, typename Consumer>
        static void make(const size_t p_k,
                         Iterator p_begin, Iterator p_end,
                         Consumer p_consumer)
        {
            const kmer msk = (1ULL << (2*p_k)) - 1;
            const size_t s = 2*(p_k - 1);

            kmer b = 0;
            kmer x = 0;
            kmer xb = 0;
            size_t j = 0;
            size_t i = 0;
            size_t n = p_end - p_begin;
            while (i + p_k <= n)
            {
                while (i + j < n && j < p_k)
                {
                    if (base(*(p_begin + i + j), b))
                    {
                        x = (x << 2) | b;
                        xb = (xb >> 2) | ((3 -b) << s);
                        j += 1;
                    }
                    else
                    {
                        i += j + 1;
                        j = 0;
                        x = 0;
                        xb = 0;
                    }
                }
                if (j == p_k)
                {
                    x &= msk;
                    p_consumer(x, xb);
                    j -= 1;
                }
                i += 1;
            }
        }
    };
}
// namespace notima

#endif // NOTIMA_KMERS_HPP
