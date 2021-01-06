#ifndef NOTIMA_VBIT_ARRAY_HPP
#define NOTIMA_VBIT_ARRAY_HPP

#include <notima/bit_array.hpp>
#include <notima/poppy.hpp>
#include <notima/internal/stats.hpp>

//#include <bit>

namespace notima
{
    namespace detail
    {
        struct bit
        {
            static size_t bit_width(uint64_t x)
            {
                size_t n = 0;
                while (x > 0)
                {
                    n += 1;
                    x >>= 1;
                }
                return n;
            }
        };
    }
    // namespace detail

    struct vbit_array
    {
        notima::bit_array bits;
        notima::poppy index;

        vbit_array(const std::vector<uint64_t>& p_items)
            : index(make(bits, p_items))
        {
        }

        size_t size() const
        {
            return index.count() - 1;
        }

        uint64_t operator[](size_t p_idx) const
        {
            size_t r0 = index.rank0(index.select(p_idx));
            size_t r1 = index.rank0(index.select(p_idx + 1));
            return bits[{r0, r1}];
        }

        static std::vector<uint64_t> make(notima::bit_array& p_bits, const std::vector<uint64_t>& p_items)
        {
            notima::bit_array bitvec;

            bitvec.push_back(1);
            for (size_t i = 0; i < p_items.size(); ++i)
            {
                uint64_t x = p_items[i];
                size_t b = detail::bit::bit_width(x);
                bitvec.push_back(b, 0);
                bitvec.push_back(1);
                p_bits.push_back(b, x);
            }
            return bitvec.words;
        }
    };

    namespace internal
    {
        template <>
        struct gather<notima::vbit_array>
        {
            nlohmann::json operator()(const notima::vbit_array& p_obj) const
            {
                nlohmann::json s;
                s["vbit"]["size"] = p_obj.size();
                s["vbit"]["bits"] = notima::internal::stats::gather(p_obj.bits.words);
                s["vbit"]["index"] = notima::internal::stats::gather(p_obj.index);
                uint64_t m = 0;
                m += s["vbit"]["index"]["poppy"]["memory"].get<uint64_t>();
                m += s["vbit"]["bits"]["vector"]["memory"].get<uint64_t>();
                s["vbit"]["memory"] = m;
                return s;
            }
        };
    }
    // namespace internal
}
// namespace notima

#endif // NOTIMA_VBIT_ARRAY_HPP
