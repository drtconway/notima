#ifndef NOTIMA_FILES_HPP
#define NOTIMA_FILES_HPP

#include <istream>
#include <ostream>
#include <string>

#include "gzipf.hpp"

namespace notima
{
    struct input
    {
        template <typename F>
        static bool getline(F& p_in, std::string& p_line);

        template <typename F>
        static bool getline(typename std::enable_if<std::is_base_of<std::istream, F>::value, F>::type& p_in, std::string& p_line)
        {
            return static_cast<bool>(std::getline(p_in, p_line));
        }
    };

    template <>
    bool input::getline<gzip::input>(gzip::input& p_in, std::string& p_line)
    {
        return p_in.getline(p_line);
    }
}
// namespace notima

#endif // NOTIMA_FILES_HPP
