#ifndef NOTIMA_INTERNAL_STATS_HPP
#define NOTIMA_INTERNAL_STATS_HPP

#include <nlohmann/json.hpp>

namespace notima
{
    namespace internal
    {
        template <typename T, typename U = void>
        struct gather
        {
            nlohmann::json operator()(const T& p_ob) const
            {
                throw std::logic_error("no specialization");
            }
        };

        struct stats
        {
            template <typename T>
            static nlohmann::json gather(const T& p_obj)
            {
                return notima::internal::gather<T>{}(p_obj);
            }
        };

        template <typename W>
        struct gather<std::vector<W>, typename std::enable_if<std::is_arithmetic<W>::value>::type>
        {
            nlohmann::json operator()(const std::vector<W>& p_obj) const
            {
                nlohmann::json s;
                s["vector"]["W"] = sizeof(W);
                s["vector"]["size"] = p_obj.size();
                s["vector"]["memory"] = p_obj.capacity()*sizeof(W);
                return s;
            }
        };

    }
    // namespace internal
}
// namespace notima

#endif // NOTIMA_INTERNAL_STATS_HPP
