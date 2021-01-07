#ifndef NOTIMA_SPARSE_ARRAY_HPP
#define NOTIMA_SPARSE_ARRAY_HPP

#include <cmath>
#include <memory>
#include <notima/integer_array.hpp>
#include <notima/poppy.hpp>
#include <notima/internal/stats.hpp>

namespace notima
{
    namespace detail
    {
    }
    // namespace detail

    template <size_t B, size_t N>
    struct compute_d
    {
        static constexpr size_t D = compute_d<B - 1, (N >> 1)>::D;
    };

    template <size_t B>
    struct compute_d<B, 1>
    {
        static constexpr size_t D = B;
    };

    template <size_t D>
    struct sparse_array_d
    {
        static constexpr size_t hi_shift = D;
        static constexpr uint64_t lo_mask = (1ULL << D) - 1;

        const size_t B;
        const size_t N;

        notima::integer_array<D> lo_bits;
        notima::poppy hi_bits;

        sparse_array_d(const size_t p_B, const size_t p_N,
                       const std::vector<uint64_t>& p_items)
            : B(p_B), N(p_N), hi_bits(make(p_B, lo_bits, p_items.begin(), p_items.end()))
        {
        }

        template <typename Itr>
        sparse_array_d(const size_t p_B, const size_t p_N, Itr p_begin, Itr p_end)
            : B(p_B), N(p_N), hi_bits(make(p_B, lo_bits, p_begin, p_end))
        {
        }

        uint64_t size() const
        {
            return 1ULL << B;
        }

        size_t count() const
        {
            return N;
        }

        size_t rank(uint64_t p_x) const
        {
            uint64_t hi_part = p_x >> hi_shift;
            uint64_t lo_part = p_x & lo_mask;

            size_t hi_rnk_0 = hi_bits.rank0(hi_bits.select(hi_part));
            size_t hi_rnk_1 = hi_bits.rank0(hi_bits.select(hi_part + 1));

            size_t lo_pos = scan_lo_bits(hi_rnk_0, hi_rnk_1, lo_part);
            return lo_pos;
        }

        uint64_t select(size_t p_r) const
        {
            uint64_t hi_part = hi_bits.rank(hi_bits.select0(p_r)) - 1;
            return (hi_part << hi_shift) | lo_bits[p_r];
        }

        template <typename Itr>
        static std::vector<uint64_t> make(size_t p_B, notima::integer_array<D>& p_lo_bits, Itr p_begin, Itr p_end)
        {
            notima::integer_array<1> bitvec;

            const uint64_t max_hi_part = (1ULL << (p_B - hi_shift)) - 1;
            uint64_t curr_hi_part = 0;

            bitvec.push_back(1);

            for (auto itr = p_begin; itr != p_end; ++itr)
            {
                uint64_t x = *itr;
                uint64_t hi_part = x >> hi_shift;
                uint64_t lo_part = x & lo_mask;

                while (curr_hi_part < hi_part)
                {
                    bitvec.push_back(1);
                    curr_hi_part += 1;
                }

                bitvec.push_back(0);
                p_lo_bits.push_back(lo_part);
            }

            while (curr_hi_part <= max_hi_part)
            {
                bitvec.push_back(1);
                curr_hi_part += 1;
            }

            return bitvec.words;
        }

    private:
        size_t scan_lo_bits(size_t p_begin, size_t p_end, uint64_t p_x) const
        {
            if (p_end - p_begin > 16)
            {
                return lower_bound(p_begin, p_end, p_x);
            }

            for (size_t r = p_begin; r < p_end; ++r)
            {
                if (lo_bits[r] >= p_x)
                {
                    return r;
                }
            }
            return p_end;
        }

        size_t lower_bound(size_t p_begin, size_t p_end, uint64_t p_x) const
        {
            size_t lo = p_begin;
            size_t hi = p_end;

            while (lo < hi)
            {
                size_t mid = (lo + hi) / 2;
                if (lo_bits[mid] < p_x)
                {
                    lo = mid + 1;
                }
                else
                {
                    hi = mid;
                }
            }
            return lo;
        }
    };

    struct sparse_array
    {
        static size_t compute_d(size_t B, size_t N)
        {
            while (N > 1)
            {
                B -= 1;
                N >>= 1;
            }
            return B;
        }

        struct array_interface
        {
            virtual ~array_interface() {}

            virtual size_t rank(uint64_t p_x) const = 0;

            virtual uint64_t select(size_t p_r) const = 0;
        };
        using array_ptr = std::shared_ptr<array_interface>;

        template <size_t D>
        struct sparse_array_impl : array_interface
        {
            const sparse_array_d<D> arr;

            template <typename Itr>
            sparse_array_impl(const size_t p_B, const size_t p_N,
                              Itr p_begin, Itr p_end)
                : arr(p_B, p_N, p_begin, p_end)
            {
            }

            virtual size_t rank(uint64_t p_x) const
            {
                return arr.rank(p_x);
            }

            virtual uint64_t select(size_t p_r) const
            {
                return arr.select(p_r);
            }
        };

        sparse_array(const size_t p_B, const std::vector<uint64_t>& p_items)
            : B(p_B), N(p_items.size()), D(compute_d(B, N)),
              m_array(make<1,typename std::vector<uint64_t>::const_iterator>(B, N, D, p_items.begin(), p_items.end()))
        {
        }

        uint64_t size() const
        {
            return (1ULL << B);
        }

        size_t count() const
        {
            return N;
        }

        size_t rank(uint64_t p_x) const
        {
            return m_array->rank(p_x);
        }

        uint64_t select(size_t p_r) const
        {
            return m_array->select(p_r);
        }

        template <size_t D, typename Itr>
        static typename std::enable_if<(D < 64),array_ptr>::type
        make(size_t p_B, size_t p_N, size_t p_D, Itr p_begin, Itr p_end)
        {
            if (p_D == D)
            {
                return array_ptr(new sparse_array_impl<D>(p_B, p_N, p_begin, p_end));
            }
            else
            {
                return make<D+1,Itr>(p_B, p_N, p_D, p_begin, p_end);
            }
        }
        template <size_t D, typename Itr>
        static typename std::enable_if<(D >= 64),array_ptr>::type
        make(size_t p_B, size_t p_N, size_t p_D, Itr p_begin, Itr p_end)
        {
            throw std::runtime_error("Cannot create array");
        }


        template <typename X>
        void with(X p_func)
        {
            with<1>(*this, p_func);
        }

        template <size_t D, typename X>
        static
        typename std::enable_if<(D < 64), void>::type
        with(const sparse_array& p_arr, X p_func)
        {
            using impl = notima::sparse_array::sparse_array_impl<D>;
            const impl* ptr = dynamic_cast<const impl*>(p_arr.m_array.get());
            if (ptr)
            {
                p_func(ptr->arr);
            }
            else
            {
                with<D+1,X>(p_arr, p_func);
            }
        }
        template <size_t D, typename X>
        static
        typename std::enable_if<(D >= 64), void>::type
        with(const sparse_array& p_arr, X p_func)
        {
            throw std::runtime_error("gather_sparse: D out of range");
        }

        const size_t B;
        const size_t N;
        const size_t D;
        array_ptr m_array;
    };

    namespace internal
    {
        template <size_t D>
        struct gather<notima::sparse_array_d<D>>
        {
            nlohmann::json operator()(const notima::sparse_array_d<D>& p_obj) const
            {
                nlohmann::json s;
                s["sparse"]["D"] = D;
                s["sparse"]["size"] = p_obj.size();
                s["sparse"]["count"] = p_obj.count();
                s["sparse"]["hi"] = notima::internal::stats::gather(p_obj.hi_bits);
                s["sparse"]["lo"] = notima::internal::stats::gather(p_obj.lo_bits);
                uint64_t m = 0;
                m += s["sparse"]["hi"]["poppy"]["memory"].get<uint64_t>();
                m += s["sparse"]["lo"]["radix_array"]["memory"].get<uint64_t>();
                s["sparse"]["memory"] = m;
                return s;
            }
        };

        template <>
        struct gather<notima::sparse_array>
        {
            template <size_t D>
            nlohmann::json gather_sparse(const notima::sparse_array::array_interface& p_obj) const
            {
                using impl = notima::sparse_array::sparse_array_impl<D>;
                const impl* ptr = dynamic_cast<const impl*>(&p_obj);
                if (ptr)
                {
                    return notima::internal::stats::gather(ptr->arr);
                }
                else
                {
                    return gather_sparse<D+1>(p_obj);
                }
            }
            template <>
            nlohmann::json gather_sparse<64>(const notima::sparse_array::array_interface& p_obj) const
            {
                throw std::runtime_error("gather_sparse: D out of range");
            }

            nlohmann::json operator()(const notima::sparse_array& p_obj) const
            {
                return gather_sparse<1>(*p_obj.m_array);
            }
        };
    }
    // namespace internal
}
// namespace notima


#endif // NOTIMA_SPARSE_ARRAY_HPP
