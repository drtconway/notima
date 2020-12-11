#ifndef NOTIMA_STATS_HPP
#define NOTIMA_STATS_HPP

#include <cmath>
#include <vector>

namespace notima
{
    struct stats
    {
        template <typename T = double>
        struct empirical_distribution
        {
            using value_type = T;

            std::vector<value_type> samples;

            template <typename Iterator>
            empirical_distribution(Iterator p_begin, Iterator p_end)
                : samples(p_begin, p_end)
            {
                std::sort(samples.begin(), samples.end());
                if (samples.size() == 0)
                {
                    throw std::runtime_error("no samples");
                }
            }

            size_t size() const
            {
                return samples.size();
            }

            value_type min() const
            {
                return samples.front();
            }

            value_type max() const
            {
                return samples.back();
            }

            value_type operator[](const size_t& p_idx) const
            {
                return samples[p_idx];
            }
        };

        template <typename T = double>
        struct uniform_distribution
        {
            using value_type = T;
            uniform_distribution(const value_type& p_min, const value_type& p_max)
                : m_min(p_min), m_max(p_max)
            {
            }

            value_type min() const
            {
                return m_min;
            }

            value_type max() const
            {
                return m_max;
            }

            double cdf(const value_type& p_x) const
            {
                if (std::is_integral<T>::value)
                {
                    return static_cast<double>(p_x - min())/static_cast<double>(max() - min() + 1);
                }
                else
                {
                    return static_cast<double>(p_x - min())/static_cast<double>(max() - min());
                }
            }

        private:
            value_type m_min;
            value_type m_max;
        };

        template <typename T = double>
        struct normal_distribution
        {
            using value_type = T;
            normal_distribution(const value_type& p_mu, const value_type& p_sigma)
                : m_mu(p_mu), m_sigma(p_sigma)
            {
            }

            double cdf(const value_type& p_x) const
            {
                static const double rootTwo = std::sqrt(2.0);
                double u = (p_x - m_mu)/(m_sigma * rootTwo);
                return 0.5 * (1.0 + std::erf(u));
            }

        private:
            value_type m_mu;
            value_type m_sigma;
        };

        template <typename Dist1, typename Dist2>
        static double kolmogorov_smirnov(const Dist1& p_subj, const Dist2& p_ref)
        {
            static_assert(std::is_same<typename Dist1::value_type, typename Dist2::value_type>::value);
            using value_type = typename Dist1::value_type;

            const size_t n = p_subj.size();
            double dP = 0;
            double dM = 0;
            for (size_t i = 0; i < n; ++i)
            {
                value_type x = p_subj[i];
                double p = static_cast<double>(i + 1)/static_cast<double>(n);
                double q = p_ref.cdf(x);
                dP = std::max(dP, p - q);
                dM = std::max(dM, q - p);
            }
            return std::max(dP, dM);
        }
    };
}
// namespace notima

#endif // NOTIMA_STATS_HPP
