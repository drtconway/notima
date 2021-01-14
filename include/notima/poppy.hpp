#ifndef NOTIMA_POPPY_HPP
#define NOTIMA_POPPY_HPP

#include <type_traits>

#include <notima/stats.hpp>
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
            static constexpr size_t sample_bits = 13;
            static constexpr size_t sample_blok = 1ULL << sample_bits;
            static constexpr size_t sample_mask = (1ULL << sample_bits) - 1;

            struct index_block_address
            {
                const uint64_t word;

                index_block_address(uint64_t p_word)
                    : word(p_word)
                {
                }

                size_t index_block_num() const
                {
                    return word >> (block_shift + 2);
                }

                size_t sub_block_num() const
                {
                    return (word >> block_shift) & 3;
                }
            };

            struct index_block
            {
                const uint64_t word;

                index_block(uint64_t p_word)
                    : word(p_word)
                {
                }

                size_t lhs_count() const
                {
                    return word >> 32;
                }

                size_t sub_count(size_t p_idx) const
                {
                    return (word >> (10*p_idx)) & 1023;
                }

                nlohmann::json dump() const
                {
                    nlohmann::json j;
                    j.push_back(lhs_count());
                    j.push_back(sub_count(0));
                    j.push_back(sub_count(1));
                    j.push_back(sub_count(2));
                    return j;
                }
            };

            size_t num_basic_blocks;
            std::vector<uint64_t> index;
            std::vector<uint64_t> samples;

            uint64_t lhs_count(uint64_t p_x) const
            {
                const index_block_address addr(p_x);
                uint64_t ix_loc = addr.index_block_num();
                uint64_t ix_block = addr.sub_block_num();

                const index_block ix(index[ix_loc]);
                uint64_t r = ix.lhs_count();

                size_t block_num = ix_loc << 2;

                for (size_t i = 0; i < ix_block; ++i)
                {
                    r += ix.sub_count(i);
                }
                return r;
            }

            template <bool Paranoid = false>
            std::tuple<uint64_t,uint64_t> select_block(uint64_t p_r) const
            {
                // Locate the select sample.
                //
                uint64_t sam_blk = p_r >> sample_bits;
                uint64_t sam_x = samples[sam_blk];

                // Locate the sample's index block,
                // then scan forward to locate the index block for p_r.
                //
                const index_block_address addr(sam_x);
                uint64_t ix_loc = addr.index_block_num();

                while (ix_loc + 1 < index.size())
                {
                    const index_block ix(index[ix_loc + 1]);
                    if (ix.lhs_count() > p_r)
                    {
                        break;
                    }
                    ix_loc += 1;
                }

                const index_block ix(index[ix_loc]);
                uint64_t r0 = ix.lhs_count();
                uint64_t index_block_num = ix_loc << 2;
                uint64_t block_num = 0;
                while (block_num < 3
                    && index_block_num + block_num < num_basic_blocks
                    && r0 + ix.sub_count(block_num) <= p_r)
                {
                    r0 += ix.sub_count(block_num);
                    block_num += 1;
                }

                block_num += index_block_num;

                return {block_num, r0};
            }

            void make(const std::vector<uint64_t>& p_words)
            {
                uint64_t index_block_left_count = 0;
                uint64_t current_index_block_count = 0;
                uint64_t basic_block_count = 0;
                uint64_t basic_block_counts = 0;
                uint64_t total_bit_count = 0;

                num_basic_blocks = 1 + (p_words.size() + (block_words - 1)) / block_words;

                for (size_t i = 0; true; ++i)
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

                            if (i >= p_words.size())
                            {
                                break;
                            }
                        }
                    }

                    if (i >= p_words.size())
                    {
                        continue;
                    }

                    uint64_t w = p_words[i];
                    uint64_t c = notima::wordy::popcount(w);
                    basic_block_count += c;

                    if ((total_bit_count & sample_mask) == 0 && c > 0)
                    {
                        uint64_t j = notima::wordy::select(w, 0);
                        samples.push_back(64 * i + j);
                    }
                    else if (((total_bit_count + c) >> sample_bits) != (total_bit_count >> sample_bits)
                             && ((total_bit_count + c) & sample_mask) > 0)
                    {
                        uint64_t p = sample_blok - (total_bit_count & sample_mask);
                        // assert(0 <= p && p < sample_blok);
                        uint64_t j = notima::wordy::select(w, p);
                        samples.push_back(64 * i + j);
                    }
                    total_bit_count += c;

                }
            }

            static void dump_ix_word(uint64_t p_ixw)
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

        uint64_t select_(uint64_t p_r) const
        {
            uint64_t block_num, left_count;
            std::tie(block_num, left_count) = I.select_block(p_r);
            uint64_t block_start = block_num << block_shift;
            uint64_t block_start_word = block_start >> word_shift;
            return block_start + basic_block::select(words.begin() + block_start_word, p_r - left_count);
        }

        uint64_t select(uint64_t p_r) const
        {
            uint64_t block_num, left_count;
            std::tie(block_num, left_count) = J.select_block<>(p_r);
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
                s["poppy"]["words"] = notima::internal::stats::gather(p_obj.words);
                //s["poppy"]["naive"] = notima::internal::stats::gather(p_obj.I.index);
                s["poppy"]["poppy"] = notima::internal::stats::gather(p_obj.J.index);
                s["poppy"]["samples"] = notima::internal::stats::gather(p_obj.J.samples);
                uint64_t m = 0;
                m += s["poppy"]["poppy"]["vector"]["memory"].get<uint64_t>();;
                m += s["poppy"]["words"]["vector"]["memory"].get<uint64_t>();;
                m += s["poppy"]["samples"]["vector"]["memory"].get<uint64_t>();;
                s["poppy"]["memory"] = m;
                if (0)
                {
                    std::vector<uint64_t> d(65);
                    for (auto itr = p_obj.words.begin(); itr != p_obj.words.end(); ++itr)
                    {
                        uint64_t c = notima::wordy::popcount(*itr);
                        d[c] += 1;
                    }
                    s["poppy"]["density"]["average"] = double(p_obj.count())/double(p_obj.size());
                    for (uint64_t i = 0; i <= 64; ++i)
                    {
                        s["poppy"]["density"]["word"][i] = d[i];
                    }
                }
                return s;
            }
        };
    }
    // namespace internal
}
// namespace notima

#endif // NOTIMA_POPPY_HPP
