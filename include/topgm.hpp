#ifndef NOTIMA_TOPGM_HPP
#define NOTIMA_TOPGM_HPP

#include "kmers.hpp"

#include <unordered_map>

namespace notima
{
    struct topgm
    {
        static void make(const size_t p_k, const std::vector<kmer>& p_kmers, std::ostream& p_out)
        {
            std::unordered_map<kmer,size_t> cts;
            size_t max_count = 0;
            for (auto itr = p_kmers.begin(); itr != p_kmers.end(); ++itr)
            {
                cts[*itr] += 1;
                max_count = std::max(max_count, cts[*itr]);
            }
            make(p_k, max_count, cts, p_out);
        }

        static void make(const size_t p_k, size_t p_max, const std::unordered_map<kmer,size_t>& p_kmers, std::ostream& p_out)
        {
            const kmer k2 = 1ULL << p_k;
            const kmer N = 1ULL << (2*p_k);
            p_out << "P2\n";
            p_out << k2 << ' ' << k2 << '\n';
            p_out << p_max << '\n';
            for (auto itr = p_kmers.begin(); itr != p_kmers.end(); ++itr)
            {
                p_out << "# " << kmers::render(p_k, itr->first) << '\t' << itr->second << '\n';
            }
            size_t j = 0;
            for (kmer x = 0; x < N; ++x, ++j)
            {
                j &= 31;
                auto itr = p_kmers.find(x);
                size_t c = 0;
                if (itr != p_kmers.end())
                {
                    c = itr->second;
                }
                p_out << (p_max - c) << ((j == 31) ? '\n' : ' '); 
            }
            p_out << '\n';
        }
    };
}
// namespace notima

#endif // NOTIMA_TOPGM_HPP
