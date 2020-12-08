#ifndef NOTIMA_TSV_HPP
#define NOTIMA_TSV_HPP

#include <istream>
#include <sstream>
#include <string>
#include <tuple>

namespace notima
{
    template <typename T>
    void from_string(const std::string& p_str, T& p_slot);

    template <>
    void from_string<std::string>(const std::string& p_str, std::string& p_slot)
    {
        p_slot = p_str;
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

    template <typename... Ts>
    struct tsv
    {
        using tuple_type = std::tuple<Ts...>;

        tsv(std::istream& p_input, bool p_header = true)
            : m_input(p_input)
        {
            m_parts.resize(sizeof...(Ts));
        }

        bool next(tuple_type& p_tuple)
        {
            if (!std::getline(m_input, m_line))
            {
                return false;
            }
            std::istringstream iss(m_line);
            for (size_t i = 0; i < m_parts.size(); ++i)
            {
                if (!(iss >> m_parts[i]))
                {
                    return false;
                }
            }
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
}
// namespace notima

#endif // NOTIMA_TSV_HPP
