#ifndef NOTIMA_FASTQ_HPP
#define NOTIMA_FASTQ_HPP

#include <istream>
#include <ostream>
#include <string>
#include <string_view>
#include <tuple>

#include "files.hpp"

namespace notima
{
    struct fastq : std::tuple<std::string,std::string,std::string,std::string>
    {
    private:
        std::string_view m_id;
        std::string_view m_seq;
        std::string_view m_qual;

    public:
        
        const std::string_view& id() const
        {
            return m_id;
        }

        const std::string_view& seq() const
        {
            return m_seq;
        }

        const std::string_view& qual() const
        {
            return m_qual;
        }

        template <typename F>
        bool read(F& p_in)
        {
            if (!input::getline<F>(p_in, std::get<0>(*this)))
            {
                return false;
            }
            const std::string& t_id = std::get<0>(*this);
            if (t_id.size() < 1 || t_id[0] != '@')
            {
                // throw?
                return false;
            }
            m_id = std::string_view(t_id.data() + 1, t_id.size() - 1);

            if (!input::getline<F>(p_in, std::get<1>(*this)))
            {
                return false;
            }
            const std::string& t_seq = std::get<1>(*this);
            m_seq = std::string_view(t_seq.data(), t_seq.size());

            if (!input::getline<F>(p_in, std::get<2>(*this)))
            {
                return false;
            }
            const std::string& t_id2 = std::get<2>(*this);
            if (t_id2.size() < 1 || t_id2[0] != '+')
            {
                // throw?
                return false;
            }

            if (!input::getline<F>(p_in, std::get<3>(*this)))
            {
                return false;
            }
            const std::string& t_qual = std::get<3>(*this);
            m_qual = std::string_view(t_qual.data(), t_qual.size());

            if (m_seq.size() != m_qual.size())
            {
                // throw?
                return false;
            }

            return true;
        }

        void write(std::ostream& p_out)
        {
            p_out << std::get<0>(*this) << '\n';
            p_out << std::get<1>(*this) << '\n';
            p_out << std::get<2>(*this) << '\n';
            p_out << std::get<3>(*this) << '\n';
        }
    };
}
// namespace notima

#endif // NOTIMA_FASTQ_HPP
