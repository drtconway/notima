#ifndef NOTIMA_POPPY_HPP
#define NOTIMA_POPPY_HPP

#include <type_traits>

#include <notima/wordy.hpp>
#include <notima/internal/stats.hpp>

#include <iostream>

namespace notima
{
    struct poppy
    {
        using word_type = uint64_t;

        static constexpr size_t block_words = 8;
        static constexpr size_t word_bits = 8*sizeof(word_type);
        static constexpr size_t block_bits = block_words*word_bits;
        static constexpr size_t word_shift = 6;
        static constexpr size_t word_mask = (1ULL << word_shift) - 1;
        static constexpr size_t block_shift = 9;
        static constexpr size_t block_word_shift = 3;
        static constexpr size_t block_mask = (1ULL << block_shift) - 1;

        struct naive_index
        {
            std::vector<uint64_t> index;

            uint64_t lhs_count(uint64_t p_x) const
            {
                uint64_t ix_part = p_x >> block_shift;
                return index[ix_part];
            }

            std::tuple<uint64_t,uint64_t> select_block(uint64_t p_r) const
            {
                auto itr = std::upper_bound(index.begin(), index.end(), p_r);
                uint64_t d = itr - index.begin();
                if (d > 0 && d < index.size() && index[d] > p_r)
                {
                    return {d - 1, index[d - 1]}; 
                }
                else
                {
                    return {d, index[d]}; 
                }
            }

            void make(const std::vector<uint64_t>& p_words)
            {
                uint64_t r = 0;
                for (size_t i = 0; i < p_words.size(); ++i)
                {
                    if ((i % block_words) == 0)
                    {
                        index.push_back(r);
                    }
                    uint64_t w = p_words[i];
                    uint64_t c = notima::wordy::popcount(w);
                    r += c;
                }
                index.push_back(r);
            }
        };

        struct rank_index
        {
            std::vector<uint64_t> index;

            uint64_t lhs_count(uint64_t p_x) const
            {
                uint64_t ix_part = p_x >> block_shift;
                uint64_t ix_loc = ix_part >> 2;
                uint64_t ix_block = ix_part & 3;

                uint64_t ix_word = index[ix_loc];
                uint64_t r = ix_word >> 32;

                for (size_t i = 0; i < ix_block; ++i)
                {
                    r += ix_word & 1023;
                    ix_word >>= 10;
                }
                return r;
            }

            void make(const std::vector<uint64_t>& p_words)
            {
                uint64_t index_block_left_count = 0;
                uint64_t current_index_block_count = 0;
                uint64_t basic_block_count = 0;
                uint64_t basic_block_counts = 0;
                for (size_t i = 0; i < p_words.size(); ++i)
                {
                    if (i > 0 && (i % block_words) == 0)
                    {
                        // flush old basic block
                        //
                        basic_block_counts = (basic_block_counts << 10) | basic_block_count;
                        current_index_block_count += basic_block_count;
                        basic_block_count = 0;

                        if ((i >> block_word_shift) > 0 && ((i >> block_word_shift) & 3) == 0)
                        {
                            // flush old index block
                            //
                            uint64_t ix = 0;
                            for (size_t j = 1; j < 4; ++j)
                            {
                                basic_block_counts >>= 10;
                                ix = (ix << 10) | (basic_block_counts & 1023);
                            }
                            ix |= index_block_left_count << 32;
                            index.push_back(ix);

                            index_block_left_count += current_index_block_count;
                            current_index_block_count = 0;
                        }
                    }

                    uint64_t w = p_words[i];
                    uint64_t c = notima::wordy::popcount(w);
                    basic_block_count += c;
                }

                // Pad out the block
                //
                for (size_t i = p_words.size(); ((i >> block_word_shift) & 3) != 0; ++i)
                {
                        if (i > 0 && (i % block_words) == 0)
                        {
                            // flush old basic block
                            //
                            basic_block_counts = (basic_block_counts << 10) | basic_block_count;
                            current_index_block_count += basic_block_count;
                            basic_block_count = 0;
                        }
                }

                // flush last index block
                //
                uint64_t ix = 0;
                for (size_t j = 1; j < 4; ++j)
                {
                    ix = (ix << 10) | (basic_block_counts & 1023);
                    basic_block_counts >>= 10;
                }
                ix |= index_block_left_count << 32;
                index.push_back(ix);
            }

            void dump_ix_word(uint64_t p_ixw)
            {
                std::cout << (p_ixw >> 32);
                for (size_t i = 0; i < 3; ++i)
                {
                    std::cout << '\t' << ((p_ixw >> (10*i)) & 1023);
                }
                std::cout << std::endl;
            }

        };

        struct basic_block
        {
            template <typename BlockIter>
            static uint64_t rank(const BlockIter& p_begin, uint64_t p_x)
            {
                uint64_t n = p_x >> word_shift;
                uint64_t m = p_x & word_mask;
                uint64_t r = 0;
                for (size_t i = 0; i < n; ++i)
                {
                    uint64_t w = *(p_begin + i);
                    r += notima::wordy::popcount(w);
                }
                uint64_t w = *(p_begin + n);
                r += notima::wordy::rank(w, m);
                return r;
            }

            template <typename BlockIter>
            static uint64_t select(const BlockIter& p_begin, uint64_t p_r)
            {
                uint64_t r = p_r;
                for (size_t i = 0; i < block_words; ++i)
                {
                    uint64_t w = *(p_begin + i);
                    uint64_t p = notima::wordy::popcount(w);
                    if (p > r)
                    {
                        return (i*word_bits) + notima::wordy::select(w, r);
                    }
                    r -= p;
                }
                // error!
                return 0xFFFFFFFFFFFFFFFFULL;
            }
        };

        std::vector<word_type> words;
        naive_index I;
        rank_index J;

        poppy(std::vector<uint64_t>&& p_words)
            : words(p_words)
        {
            I.make(words);
            J.make(words);
        }

        uint64_t size() const
        {
            return 64*words.size();
        }

        uint64_t count() const
        {
            return (I.index.size() ? I.index.back() : 0);
        }

        uint64_t rank_(uint64_t p_x) const
        {
            uint64_t block_num = p_x >> block_shift;
            uint64_t block_start = block_num << block_shift;
            uint64_t block_start_word = block_start >> word_shift;
            uint64_t r0 = I.lhs_count(p_x);
            uint64_t r1 = basic_block::rank(words.begin() + block_start_word, p_x & block_mask);
            return r0 + r1;
        }

        uint64_t rank(uint64_t p_x) const
        {
            uint64_t block_num = p_x >> block_shift;
            uint64_t block_start = block_num << block_shift;
            uint64_t block_start_word = block_start >> word_shift;
            uint64_t r0 = J.lhs_count(p_x);
            uint64_t r1 = basic_block::rank(words.begin() + block_start_word, p_x & block_mask);
            return r0 + r1;
        }

        uint64_t select(uint64_t p_r) const
        {
            uint64_t block_num, left_count;
            std::tie(block_num, left_count) = I.select_block(p_r);
            uint64_t block_start = block_num << block_shift;
            uint64_t block_start_word = block_start >> word_shift;
            return block_start + basic_block::select(words.begin() + block_start_word, p_r - left_count);
        }

        uint64_t rank0(uint64_t p_x) const
        {
            return p_x - rank(p_x);
        }

        uint64_t select0(uint64_t p_r) const
        {
            // naive select0.

            uint64_t lo = 0;
            uint64_t hi = size();

            while (lo < hi)
            {
                uint64_t mid = (lo + hi) / 2;
                //std::cout << "lb"  << '(' << p_r << ')'
                //    << '\t' << lo << '(' << rank0(lo) << ')'
                //    << '\t' << mid << '(' << rank0(mid) << ')'
                //    << '\t' << hi << '(' << rank0(hi) << ')'
                //    << std::endl;
                size_t r0 = rank0(mid);
                if (r0 <= p_r)
                {
                    lo = mid + 1;
                }
                else
                {
                    hi = mid;
                }
            }
            if (lo > 0)
            {
                return lo - 1;
            }
            return lo;
        }
    };

    namespace internal
    {
        template <>
        struct gather<notima::poppy>
        {
            nlohmann::json operator()(const notima::poppy& p_obj) const
            {
                nlohmann::json s;
                s["poppy"]["size"] = p_obj.size();
                s["poppy"]["count"] = p_obj.count();
                s["poppy"]["naive"] = gather<std::vector<uint64_t>>{}(p_obj.I.index);
                s["poppy"]["poppy"] = gather<std::vector<uint64_t>>{}(p_obj.J.index);
                s["poppy"]["memory"] = s["poppy"]["poppy"]["vector"]["memory"];
                return s;
            }
        };
    }
    // namespace internal
}
// namespace notima

#endif // NOTIMA_POPPY_HPP
