#ifndef NOTIMA_TSV_HPP
#define NOTIMA_TSV_HPP

#include <istream>
#include <sstream>
#include <string>
#include <tuple>

namespace notima
{
    template <typename X>
    void split(const std::string& p_str, const char p_sep, X p_consumer)
    {
        auto itr = p_str.begin();
        while (true)
        {
            auto jtr = itr;
            while (itr != p_str.end() && *itr != p_sep)
            {
                ++itr;
            }
            p_consumer(jtr, itr);
            if (itr == p_str.end())
            {
                break;
            }
            else
            {
                ++itr;
            }
        }
    }

    template <typename T>
    void from_string(const std::string& p_str, T& p_slot);

    template <>
    void from_string<std::string>(const std::string& p_str, std::string& p_slot)
    {
        p_slot = p_str;
    }

    template <>
    void from_string<char>(const std::string& p_str, char& p_slot)
    {
        if (p_str.size() != 1)
        {
            throw std::runtime_error("bad conversion");
        }
        p_slot = p_str[0];
    }

    template <>
    void from_string<int>(const std::string& p_str, int& p_slot)
    {
        p_slot = std::stoi(p_str);
    }

    template <>
    void from_string<long>(const std::string& p_str, long& p_slot)
    {
        p_slot = std::stol(p_str);
    }

    template <>
    void from_string<long long>(const std::string& p_str, long long& p_slot)
    {
        p_slot = std::stoll(p_str);
    }

    template <>
    void from_string<unsigned int>(const std::string& p_str, unsigned int& p_slot)
    {
        p_slot = std::stoul(p_str);
    }

    template <>
    void from_string<unsigned long>(const std::string& p_str, unsigned long& p_slot)
    {
        p_slot = std::stoul(p_str);
    }

    template <>
    void from_string<unsigned long long>(const std::string& p_str, unsigned long long& p_slot)
    {
        p_slot = std::stoull(p_str);
    }

    template <>
    void from_string<float>(const std::string& p_str, float& p_slot)
    {
        p_slot = std::stof(p_str);
    }

    template <>
    void from_string<double>(const std::string& p_str, double& p_slot)
    {
        p_slot = std::stod(p_str);
    }

    template <>
    void from_string<long double>(const std::string& p_str, long double& p_slot)
    {
        p_slot = std::stold(p_str);
    }

    template <typename T, char Sep = ',', bool Prune = false>
    struct rep : std::vector<T>
    {
    };

    template <typename T, char Sep, bool Prune>
    void from_string(const std::string& p_str, rep<T,Sep,Prune>& p_slot)
    {
        p_slot.clear();
        std::string tmp;
        T ump;
        notima::split(p_str, Sep, [&](auto beg, auto end) {
            if (Prune && beg == end)
            {
                return;
            }
            tmp.clear();
            tmp.insert(tmp.end(), beg, end);
            notima::from_string(tmp, ump);
            p_slot.push_back(ump);
        });
    }

    template <typename... Ts>
    struct tsv
    {
        using tuple_type = std::tuple<Ts...>;

        //template <char Sep = '\t'>
        struct reader
        {
            std::vector<std::string> header;

            reader(std::istream& p_input, bool p_header = false)
                : m_input(p_input)
            {
                m_parts.resize(sizeof...(Ts));
                if (p_header)
                {
                    if (!std::getline(m_input, m_line))
                    {
                        throw std::runtime_error("no header line");
                    }
                    notima::split(m_line, '\t', [&](auto beg, auto end) {
                        header.push_back(std::string(beg, end));
                    });
                }
            }

            bool next(tuple_type& p_tuple)
            {
                if (!std::getline(m_input, m_line))
                {
                    return false;
                }
                size_t i = 0;
                notima::split(m_line, '\t', [&](auto beg, auto end) {
                    if (i < m_parts.size())
                    {
                        m_parts[i].clear();
                        m_parts[i].insert(m_parts[i].end(), beg, end);
                        i += 1;
                    }
                    else
                    {
                        throw std::runtime_error("incorect number of fields in line");
                    }
                });
                convert_tuple<>(p_tuple, m_parts);
                return true;
            }

            template <size_t I = 0>
            typename std::enable_if<I == sizeof...(Ts), void>::type
            convert_tuple(tuple_type& p_tuple, const std::vector<std::string>& p_strs)
            {
            }

            template <size_t I = 0>
            typename std::enable_if<I < sizeof...(Ts), void>::type
            convert_tuple(tuple_type& p_tuple, const std::vector<std::string>& p_strs)
            {
                notima::from_string(p_strs[I], std::get<I>(p_tuple));
                convert_tuple<I+1>(p_tuple, p_strs);
            }

        private:
            std::istream& m_input;
            std::string m_line;
            std::vector<std::string> m_parts;
        };
    };
}
// namespace notima

#endif // NOTIMA_TSV_HPP
