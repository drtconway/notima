#ifndef NOTIMA_INTERNAL_STATS_HPP
#define NOTIMA_INTERNAL_STATS_HPP

#include <string>
#include <nlohmann/json.hpp>

#include <notima/subword_array.hpp>
#include <notima/radix_array.hpp>
#include <notima/poppy.hpp>
#include <notima/sparse_array.hpp>

namespace notima
{
    namespace internal
    {
        struct stats
        {
            template <size_t B, size_t W>
            static
            nlohmann::json
            gather(const notima::subword_array<B,W>& p_obj)
            {
                nlohmann::json s;
                s["subword"]["B"] = B;
                s["subword"]["W"] = W;
                s["subword"]["size"] = p_obj.size();
                s["subword"]["memory"] = p_obj.words.capacity()*sizeof(W);
                return s;
            }

            template <typename W>
            static
            typename std::enable_if<std::is_arithmetic<W>::value, nlohmann::json>::type
            gather(const std::vector<W>& p_obj)
            {
                nlohmann::json s;
                s["vector"]["W"] = sizeof(W);
                s["vector"]["size"] = p_obj.size();
                s["vector"]["memory"] = p_obj.capacity()*sizeof(W);
                return s;
            }

            template <typename T>
            static
            void
            gather(const notima::radix_array<T>& p_obj, nlohmann::json& p_ss)
            {
                nlohmann::json s = gather(p_obj.self);
                p_ss.push_back(s);
            }

            template <typename T1, typename T2, typename... Ts>
            static void
            gather(const notima::radix_array<T1,T2,Ts...>& p_obj, nlohmann::json& p_ss)
            {
                nlohmann::json s = gather(p_obj.self);
                p_ss.push_back(s);
                gather(p_obj.rest, p_ss);
            }

            template <typename... Ts>
            static
            nlohmann::json
            gather(const notima::radix_array<Ts...>& p_obj)
            {
                nlohmann::json s;
                s["radix_array"]["depth"] = sizeof...(Ts);
                s["radix_array"]["size"] = sizeof...(Ts);
                s["radix_array"]["parts"] = nlohmann::json::array();
                gather(p_obj, s["radix_array"]["parts"]);
                uint64_t m = 0;
                for (size_t i = 0; i < s["radix_array"]["parts"].size(); ++i)
                {
                    const nlohmann::json& p = s["radix_array"]["parts"][i];

                    if (p.contains("vector"))
                    {
                        m += p["vector"]["memory"].get<uint64_t>();
                    }
                    else
                    {
                        m += p["subword"]["memory"].get<uint64_t>();
                    }
                }
                s["radix_array"]["memory"] = m;
                return s;
            }

            static
            nlohmann::json
            gather(const notima::poppy& p_obj)
            {
                nlohmann::json s;
                s["poppy"]["size"] = p_obj.size();
                s["poppy"]["count"] = p_obj.count();
                s["poppy"]["naive"] = gather(p_obj.I.index);
                s["poppy"]["poppy"] = gather(p_obj.J.index);
                s["poppy"]["memory"] = s["poppy"]["poppy"]["vector"]["memory"];
                return s;
            }

            template <size_t D>
            static
            nlohmann::json
            gather(const notima::sparse_array_d<D>& p_obj)
            {
                nlohmann::json s;
                s["sparse"]["D"] = D;
                s["sparse"]["size"] = p_obj.size();
                s["sparse"]["count"] = p_obj.count();
                s["sparse"]["hi"] = gather(p_obj.hi_bits);
                s["sparse"]["lo"] = gather(p_obj.lo_bits);
                uint64_t m = 0;
                m += s["sparse"]["hi"]["poppy"]["memory"].get<uint64_t>();
                m += s["sparse"]["lo"]["radix_array"]["memory"].get<uint64_t>();
                s["sparse"]["memory"] = m;
                return s;
            }

            template <size_t D>
            static
            typename std::enable_if<(D < 64), nlohmann::json>::type
            gather_sparse(const notima::sparse_array::array_interface& p_obj)
            {
                using impl = notima::sparse_array::sparse_array_impl<D>;
                const impl* ptr = dynamic_cast<const impl*>(&p_obj);
                if (ptr)
                {
                    return gather(ptr->arr);
                }
                else
                {
                    return gather_sparse<D+1>(p_obj);
                }
            }
            template <size_t D>
            static
            typename std::enable_if<(D >= 64), nlohmann::json>::type
            gather_sparse(const notima::sparse_array::array_interface& p_obj)
            {
                throw std::runtime_error("gather_sparse: D out of range");
            }

            static
            nlohmann::json
            gather(const notima::sparse_array& p_obj)
            {
                return gather_sparse<1>(*p_obj.m_array);
            }
        };
    }
    // namespace internal
}
// namespace notima

#endif // NOTIMA_INTERNAL_STATS_HPP
