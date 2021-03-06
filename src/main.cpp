#include <fastq.hpp>
#include <gzipf.hpp>
#include <kmers.hpp>
#include <topgm.hpp>
#include <notima/codec8.hpp>
#include <notima/tsv.hpp>
#include <notima/sparse_array.hpp>
#include <notima/vbit_array.hpp>
#include <notima/internal/stats.hpp>

#include <deque>
#include <map>
#include <iostream>
#include <fstream>
#include <memory>
#include <random>

#include <nlohmann/json.hpp>

using namespace std;
using namespace notima;

namespace // anonymous
{
    string human(size_t x)
    {
        size_t y = x;
        size_t i = 0;
        while (y > 1024)
        {
            i += 1;
            y >>= 10;
        }
        string s = to_string(y);
        if (i > 0)
        {
            s.push_back(".KMGTPE"[i]);
        }
        return s;
    }

    struct accumulator
    {
        using run = vector<uint8_t>;
        using run_ptr = shared_ptr<run>;
        using layer = deque<run_ptr>;
        using layer_ptr = shared_ptr<layer>;

        static constexpr size_t MaxBuf = 1ULL << 24;

        vector<kmer> buffer;
        vector<layer_ptr> layers;
        size_t J;
        mt19937& rng;

        accumulator(mt19937& p_rng)
            : J(2), rng(p_rng)
        {
            layers.push_back(layer_ptr(new layer()));
        }

        template <typename Collection>
        void add(const Collection& p_kmers)
        {
            buffer.insert(buffer.end(), p_kmers.begin(), p_kmers.end());
            if (buffer.size() >= MaxBuf)
            {
                flush();
            }
        }

        void done()
        {
            if (buffer.size() > 0)
            {
                flush();
            }
            ripple(true);
        }

        template <typename X>
        void visit(X p_visitor)
        {
            done();
            if (layers.back()->size() > 0)
            {
                visit_run(*layers.back()->front(), p_visitor);
            }
        }

        void flush()
        {
            std::sort(buffer.begin(), buffer.end());

            run_ptr rp = run_ptr(new run);
            layers[0]->push_back(rp);

            compress_run(buffer, *rp);

            buffer.clear();

            ripple(false);
        }

        void ripple(bool p_finish = false)
        {
            const size_t L = layers.size();
            for (size_t i = 0; i < L; ++i)
            {
                layer& l = *layers[i];
                if (!p_finish && l.size() < J)
                {
                    break;
                }
                if (i + 1 == L)
                {
                    layers.push_back(layer_ptr(new layer()));
                }
                layer& l1 = *layers[i+1];
                while (l.size() >= 2)
                {
                    run_ptr lhs = l.front();
                    l.pop_front();
                    run_ptr rhs = l.front();
                    l.pop_front();

                    run_ptr rp = run_ptr(new run);
                    l1.push_back(rp);
                    merge_runs(*lhs, *rhs, *rp);
                }
                if (p_finish && l.size() == 1)
                {
                    run_ptr rp = l.front();
                    l.pop_front();
                    l1.push_back(rp);
                }
            }
        }

        void compress_run(const vector<kmer>& p_kmers, vector<uint8_t>& p_run)
        {
            kmer px = 0;
            kmer x = 0;
            size_t c = 0;
            for (auto itr = p_kmers.begin(); itr != p_kmers.end(); ++itr)
            {
                if (*itr != x)
                {
                    if (c > 0)
                    {
                        kmer dx = x - px;
                        codec8::encode(dx, p_run);
                        codec8::encode(c, p_run);
                    }
                    px = x;
                    x = *itr;
                    c = 0;
                }
                c += 1;
            }
            if (c > 0)
            {
                kmer dx = x - px;
                codec8::encode(dx, p_run);
                codec8::encode(c, p_run);
            }
            cerr << human(p_kmers.size() * sizeof(kmer)) << " -> " << human(p_run.size()) << endl;
        }

        void merge_runs(const vector<uint8_t>& p_lhs, const vector<uint8_t>& p_rhs, vector<uint8_t>& p_res)
        {
            kmer x = 0;
            size_t xc = 0;
            kmer y = 0;
            size_t yc = 0;

            auto xitr = p_lhs.begin();
            auto xend = p_lhs.end();

            auto yitr = p_rhs.begin();
            auto yend = p_rhs.end();

            if (xitr == xend)
            {
                p_res.insert(p_res.end(), yitr, yend);
                return;
            }
            if (yitr == yend)
            {
                p_res.insert(p_res.end(), xitr, xend);
                return;
            }

            next(xitr, x, xc);
            next(yitr, y, yc);

            kmer pz = 0;
            while (true)
            {
                if (x < y)
                {
                    kmer dz = x - pz;
                    codec8::encode(dz, p_res);
                    codec8::encode(xc, p_res);
                    pz = x;

                    if (xitr != xend)
                    {
                        next(xitr, x, xc);
                    }
                    else
                    {
                        kmer dz = y - pz;
                        codec8::encode(dz, p_res);
                        codec8::encode(yc, p_res);
                        pz = y;
                        break;
                    }

                    continue;
                }

                if (y < x)
                {
                    kmer dz = y - pz;
                    codec8::encode(dz, p_res);
                    codec8::encode(yc, p_res);
                    pz = y;

                    if (yitr != yend)
                    {
                        next(yitr, y, yc);
                    }
                    else
                    {
                        kmer dz = x - pz;
                        codec8::encode(dz, p_res);
                        codec8::encode(xc, p_res);
                        pz = x;
                        break;
                    }

                    continue;
                }

                kmer dz = x - pz;
                codec8::encode(dz, p_res);
                codec8::encode(xc + yc, p_res);
                pz = x;

                if (xitr == xend || yitr == yend)
                {
                    break;
                }
                next(xitr, x, xc);
                next(yitr, y, yc);
            }

            while (xitr != xend)
            {
                next(xitr, x, xc);
                kmer dz = x - pz;
                codec8::encode(dz, p_res);
                codec8::encode(xc, p_res);
                pz = x;
            }
            while (yitr != yend)
            {
                next(yitr, y, yc);
                kmer dz = y - pz;
                codec8::encode(dz, p_res);
                codec8::encode(yc, p_res);
                pz = y;
            }
            cerr << human(p_lhs.size()) << " (+) " << human(p_rhs.size())  << " -> " << human(p_res.size()) << endl;
        }

        static void next(vector<uint8_t>::const_iterator& p_itr, kmer& p_x, size_t& p_c)
        {
            p_x += codec8::decode(p_itr);
            p_c = codec8::decode(p_itr);
        }

        template <typename X>
        static void visit_run(const vector<uint8_t>& p_run, X p_visitor)
        {
            kmer x = 0;
            size_t xc = 0;
            auto xitr = p_run.begin();
            while (xitr != p_run.end())
            {
                next(xitr, x, xc);
                p_visitor(x, xc);
            }
        }
    };

    template <typename Itr>
    struct kmers_iterator
    {
        Itr cur;
        Itr end;

        kmers_iterator(Itr p_begin, Itr p_end)
            : cur(p_begin), end(p_end)
        {
        }

        uint64_t operator*() const
        {
            return *cur;
        }

        kmers_iterator& operator++()
        {
            if (cur == end)
            {
                return *this;
            }
            uint64_t x = *cur;
            while (cur != end && *cur == x)
            {
                ++cur;
            }
            return *this;
        }

        bool operator==(const kmers_iterator& other) const
        {
            return cur == other.cur && end == other.end;
        }

        bool operator!=(const kmers_iterator& other) const
        {
            return cur != other.cur || end != other.end;
        }
    };

    template <typename Itr>
    struct counts_iterator
    {
        Itr cur;
        Itr end;
        Itr nxt;
        size_t cnt;

        counts_iterator(Itr p_begin, Itr p_end)
            : cur(p_begin), end(p_end), nxt(p_begin), cnt(0)
        {
            update();
        }

        size_t operator*() const
        {
            return cnt - 1;
        }

        counts_iterator& operator++()
        {
            update();
            return *this;
        }

        bool operator==(const counts_iterator& other) const
        {
            return cur == other.cur && end == other.end;
        }

        bool operator!=(const counts_iterator& other) const
        {
            return cur != other.cur || end != other.end;
        }

        void update()
        {
            cur = nxt;
            if (cur == end)
            {
                return;
            }
            cnt = 0;
            while (nxt != end && *nxt == *cur)
            {
                ++cnt;
                ++nxt;
            }
        }
    };

    void doit0(const string& p_str)
    {
        random_device rnd_dev;
        mt19937 rng(rnd_dev());

        const size_t K = 11;
        gzip::input inp(p_str);

        fastq rd;
        vector<kmer> fwd;
        vector<kmer> rev;

        accumulator acc(rng);

        size_t rn = 0;
        while (rd.read(inp))
        {
            ++rn;
            fwd.clear();
            rev.clear();
            kmers::make(K, rd.seq(), fwd, rev);
            acc.add(fwd);
            acc.add(rev);
            if ((rn & ((1ULL << 20) - 1)) == 0)
            {
                cerr << "#" << rn << endl;
            }
        }
        map<size_t,size_t> h;
        acc.visit([&](const kmer& x, const size_t& c) mutable {
            h[c] += 1;
        });
        for (auto itr = h.begin(); itr != h.end(); ++itr)
        {
            cout << itr->first << '\t' << itr->second << endl;
        }
        return;
    }

    template <typename Vis>
    void kmer_and_count(vector<kmer>& p_kmers, Vis p_visitor)
    {
        kmer x = 0;
        size_t c = 0;
        for (auto itr = p_kmers.begin(); itr != p_kmers.end(); ++itr)
        {
            if (*itr != x)
            {
                if (c > 0)
                {
                    p_visitor(x, c);
                }
                x = *itr;
                c = 0;
            }
            c += 1;
        }
        if (c > 0)
        {
            p_visitor(x, c);
        }
    }

    void doit(const string& p_str)
    {
        const size_t K = 25;
        gzip::input inp(p_str);

        fastq rd;
        vector<kmer> fwd;
        vector<kmer> rev;

        std::vector<kmer> all;

        size_t rn = 0;
        while (rd.read(inp))
        {
            ++rn;
            fwd.clear();
            rev.clear();
            kmers::make(K, rd.seq(), fwd, rev);
            all.insert(all.end(), fwd.begin(), fwd.end());
            all.insert(all.end(), rev.begin(), rev.end());
            if (all.size() > (1ULL << 25))
            {
                break;
            }
        }

        std::sort(all.begin(), all.end());

        using itr_type = std::vector<kmer>::const_iterator;
        using cnt_type = counts_iterator<itr_type>;
        using kmr_type = kmers_iterator<itr_type>;

        if (0)
        {
            cnt_type b(all.begin(), all.end());
            cnt_type e(all.end(), all.end());
            uint64_t t = 0;
            std::map<size_t,size_t> H;
            for (auto itr = b; itr != e; ++itr)
            {
                H[*itr] += 1;
                t += 1;
            }
            for (auto itr = H.begin(); itr != H.end(); ++itr)
            {
                std::cout << itr->first << '\t' << itr->second << std::endl;
            }
            std::cout << all.size() << '\t' << t << std::endl;
        }
        notima::vbit_array A(cnt_type(all.begin(), all.end()), cnt_type(all.end(), all.end()));
        std::cout << notima::internal::stats::gather(A) << std::endl;
        size_t N = A.size();
        notima::sparse_array X(2*K, N, kmr_type(all.begin(), all.end()), kmr_type(all.end(), all.end()));
        std::cout << notima::internal::stats::gather(X) << std::endl;
        //X.with([](const auto& arr) {
        //    std::cout << arr.size() << std::endl;
        //    const std::vector<uint64_t>& ws = arr.hi_bits.words;
        //    for (size_t i = 0; i < ws.size(); ++i)
        //    {
        //        std::cout << i << '\t' << notima::wordy::popcount(ws[i]) << std::endl;
        //    }
        //});
    }

}
// namespace anonymous

int main(int argc, const char* argv[])
{
    doit(argv[1]);
    return 0;
}
