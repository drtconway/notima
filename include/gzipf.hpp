#ifndef NOTIMA_GZIPF_HPP
#define NOTIMA_GZIPF_HPP

#include <stdio.h>
#include <zlib.h>

namespace notima
{
    namespace gzip
    {
        struct input
        {
            static constexpr unsigned buffer_size = 1024*1024;

            gzFile m_z;
            std::string m_buf;
            std::string::const_iterator m_cur;

            input(const std::string& p_filename)
                : m_cur(m_buf.end())
            {
                m_z = gzopen(p_filename.c_str(), "rb");
                if (m_z == NULL)
                {
                    throw std::runtime_error(p_filename);
                }
                gzbuffer(m_z, buffer_size);
            }

            bool read(std::string& p_buf, int p_len = 65536)
            {
                if (p_buf.capacity() < p_len)
                {
                    p_buf.reserve(p_len);
                }
                p_buf.resize(p_len);
                int bytes_read = gzread(m_z, p_buf.data(), p_len);
                if (bytes_read <= 0)
                {
                    return false;
                }
                p_buf.resize(bytes_read);
                return true;
            }

            bool getline(std::string& p_line)
            {
                p_line.clear();
                while (true)
                {
                    if (m_cur == m_buf.end())
                    {
                        if (!read(m_buf))
                        {
                            return p_line.size() > 0;
                        }
                        m_cur = m_buf.begin();
                    }
                    auto itr = m_cur;
                    while (itr != m_buf.end() && *itr != '\n')
                    {
                        ++itr;
                    }
                    p_line.insert(p_line.end(), m_cur, itr);
                    m_cur = itr;
                    if (m_cur != m_buf.end())
                    {
                        ++m_cur;
                        return true;
                    }
                }
            }
        };

        struct output
        {
            gzFile m_z;

            output(const std::string& p_filename)
                : m_z(NULL)
            {
                m_z = gzopen(p_filename.c_str(), "wb9");
                if (m_z == NULL)
                {
                    throw std::runtime_error(p_filename);
                }
            }

            ~output()
            {
                close();
            }

            void write(const std::string& p_str)
            {
                gzwrite(m_z, p_str.data(), p_str.size());
            }

            void close()
            {
                if (m_z)
                {
                    gzclose(m_z);
                    m_z = NULL;
                }
            }
        };
    }
    // namespace gzip
}
// namespace notima

#endif // NOTIMA_GZIPF_HPP
