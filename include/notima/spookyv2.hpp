#ifndef NOTIMA_SPOOKYV2_HPP
#define NOTIMA_SPOOKYV2_HPP

namespace notima
{
    struct spooky
    {
        static uint64_t hash64(const uint64_t& p_seed, const uint64_t& p_x)
        {
            return hash64(&p_x, sizeof(p_x), p_seed);
        }

    private:
        static constexpr size_t sc_numVars = 12;
        static constexpr size_t sc_blockSize = sc_numVars*8;
        static constexpr size_t sc_bufSize = 2*sc_blockSize;
        static constexpr uint64_t sc_const = 0xdeadbeefdeadbeefLL;

        static uint32_t hash32(const void *message, size_t length, uint32_t seed)
        {
            uint64_t hash1 = seed, hash2 = seed;
            hash128(message, length, &hash1, &hash2);
            return (uint32_t)hash1;
        }

        static uint64_t hash64(const void *message, size_t length, uint64_t seed)
        {
            uint64_t hash1 = seed;
            hash128(message, length, &hash1, &seed);
            return hash1;
        }

        static void hash128(const void *message, size_t length, uint64_t *hash1, uint64_t *hash2)
        {
            if (length < sc_bufSize)
            {
                small(message, length, hash1, hash2);
                return;
            }

            uint64_t h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11;
            uint64_t buf[sc_numVars];
            uint64_t *end_;
            union 
            { 
                const uint8_t *p8; 
                uint64_t *p64; 
                size_t i; 
            } u;
            size_t remainder;
            
            h0=h3=h6=h9  = *hash1;
            h1=h4=h7=h10 = *hash2;
            h2=h5=h8=h11 = sc_const;
            
            u.p8 = reinterpret_cast<const uint8_t *>(message);
            end_ = u.p64 + (length/sc_blockSize)*sc_numVars;

            // handle all whole sc_blockSize blocks of bytes
            if ((u.i & 0x7) == 0)
            {
                while (u.p64 < end_)
                { 
                    mix(u.p64, h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
                    u.p64 += sc_numVars;
                }
            }
            else
            {
                while (u.p64 < end_)
                {
                    memcpy(buf, u.p64, sc_blockSize);
                    mix(buf, h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
                    u.p64 += sc_numVars;
                }
            }

            // handle the last partial block of sc_blockSize bytes
            remainder = length - (reinterpret_cast<const uint8_t *>(end_) - reinterpret_cast<const uint8_t *>(message));
            memcpy(buf, end_, remainder);
            memset(reinterpret_cast<uint8_t *>(buf)+remainder, 0, sc_blockSize-remainder);
            reinterpret_cast<uint8_t *>(buf)[sc_blockSize-1] = remainder;
            
            // do some final mixing 
            end(buf, h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
            *hash1 = h0;
            *hash2 = h1;
        }

        static uint64_t rot64(uint64_t x, int k)
        {
            return (x << k) | (x >> (64 - k));
        }

        static void mix(const uint64_t *data, 
            uint64_t &s0, uint64_t &s1, uint64_t &s2, uint64_t &s3,
            uint64_t &s4, uint64_t &s5, uint64_t &s6, uint64_t &s7,
            uint64_t &s8, uint64_t &s9, uint64_t &s10,uint64_t &s11)
        {
          s0 += data[0];    s2 ^= s10;    s11 ^= s0;    s0 = rot64(s0,11);    s11 += s1;
          s1 += data[1];    s3 ^= s11;    s0 ^= s1;    s1 = rot64(s1,32);    s0 += s2;
          s2 += data[2];    s4 ^= s0;    s1 ^= s2;    s2 = rot64(s2,43);    s1 += s3;
          s3 += data[3];    s5 ^= s1;    s2 ^= s3;    s3 = rot64(s3,31);    s2 += s4;
          s4 += data[4];    s6 ^= s2;    s3 ^= s4;    s4 = rot64(s4,17);    s3 += s5;
          s5 += data[5];    s7 ^= s3;    s4 ^= s5;    s5 = rot64(s5,28);    s4 += s6;
          s6 += data[6];    s8 ^= s4;    s5 ^= s6;    s6 = rot64(s6,39);    s5 += s7;
          s7 += data[7];    s9 ^= s5;    s6 ^= s7;    s7 = rot64(s7,57);    s6 += s8;
          s8 += data[8];    s10 ^= s6;    s7 ^= s8;    s8 = rot64(s8,55);    s7 += s9;
          s9 += data[9];    s11 ^= s7;    s8 ^= s9;    s9 = rot64(s9,54);    s8 += s10;
          s10 += data[10];    s0 ^= s8;    s9 ^= s10;    s10 = rot64(s10,22);    s9 += s11;
          s11 += data[11];    s1 ^= s9;    s10 ^= s11;    s11 = rot64(s11,46);    s10 += s0;
        }

        static void endPartial(
            uint64_t &h0, uint64_t &h1, uint64_t &h2, uint64_t &h3,
            uint64_t &h4, uint64_t &h5, uint64_t &h6, uint64_t &h7, 
            uint64_t &h8, uint64_t &h9, uint64_t &h10,uint64_t &h11)
        {
            h11+= h1;    h2 ^= h11;   h1 = rot64(h1,44);
            h0 += h2;    h3 ^= h0;    h2 = rot64(h2,15);
            h1 += h3;    h4 ^= h1;    h3 = rot64(h3,34);
            h2 += h4;    h5 ^= h2;    h4 = rot64(h4,21);
            h3 += h5;    h6 ^= h3;    h5 = rot64(h5,38);
            h4 += h6;    h7 ^= h4;    h6 = rot64(h6,33);
            h5 += h7;    h8 ^= h5;    h7 = rot64(h7,10);
            h6 += h8;    h9 ^= h6;    h8 = rot64(h8,13);
            h7 += h9;    h10^= h7;    h9 = rot64(h9,38);
            h8 += h10;   h11^= h8;    h10= rot64(h10,53);
            h9 += h11;   h0 ^= h9;    h11= rot64(h11,42);
            h10+= h0;    h1 ^= h10;   h0 = rot64(h0,54);
        }

        static void end(const uint64_t *data, 
            uint64_t &h0, uint64_t &h1, uint64_t &h2, uint64_t &h3,
            uint64_t &h4, uint64_t &h5, uint64_t &h6, uint64_t &h7, 
            uint64_t &h8, uint64_t &h9, uint64_t &h10,uint64_t &h11)
        {
            h0 += data[0];   h1 += data[1];   h2 += data[2];   h3 += data[3];
            h4 += data[4];   h5 += data[5];   h6 += data[6];   h7 += data[7];
            h8 += data[8];   h9 += data[9];   h10 += data[10]; h11 += data[11];
            endPartial(h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
            endPartial(h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
            endPartial(h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
        }

        static void shortMix(uint64_t &h0, uint64_t &h1, uint64_t &h2, uint64_t &h3)
        {
            h2 = rot64(h2,50);  h2 += h3;  h0 ^= h2;
            h3 = rot64(h3,52);  h3 += h0;  h1 ^= h3;
            h0 = rot64(h0,30);  h0 += h1;  h2 ^= h0;
            h1 = rot64(h1,41);  h1 += h2;  h3 ^= h1;
            h2 = rot64(h2,54);  h2 += h3;  h0 ^= h2;
            h3 = rot64(h3,48);  h3 += h0;  h1 ^= h3;
            h0 = rot64(h0,38);  h0 += h1;  h2 ^= h0;
            h1 = rot64(h1,37);  h1 += h2;  h3 ^= h1;
            h2 = rot64(h2,62);  h2 += h3;  h0 ^= h2;
            h3 = rot64(h3,34);  h3 += h0;  h1 ^= h3;
            h0 = rot64(h0,5);   h0 += h1;  h2 ^= h0;
            h1 = rot64(h1,36);  h1 += h2;  h3 ^= h1;
        }

        static void shortEnd(uint64_t &h0, uint64_t &h1, uint64_t &h2, uint64_t &h3)
        {
            h3 ^= h2;  h2 = rot64(h2,15);  h3 += h2;
            h0 ^= h3;  h3 = rot64(h3,52);  h0 += h3;
            h1 ^= h0;  h0 = rot64(h0,26);  h1 += h0;
            h2 ^= h1;  h1 = rot64(h1,51);  h2 += h1;
            h3 ^= h2;  h2 = rot64(h2,28);  h3 += h2;
            h0 ^= h3;  h3 = rot64(h3,9);   h0 += h3;
            h1 ^= h0;  h0 = rot64(h0,47);  h1 += h0;
            h2 ^= h1;  h1 = rot64(h1,54);  h2 += h1;
            h3 ^= h2;  h2 = rot64(h2,32);  h3 += h2;
            h0 ^= h3;  h3 = rot64(h3,25);  h0 += h3;
            h1 ^= h0;  h0 = rot64(h0,63);  h1 += h0;
        }
        
    private:

        static void small(const void *message, size_t length, uint64_t *hash1, uint64_t *hash2)
        {
            uint64_t buf[2*sc_numVars];
            union 
            { 
                const uint8_t *p8; 
                uint32_t *p32;
                uint64_t *p64; 
                size_t i; 
            } u;

            u.p8 = reinterpret_cast<const uint8_t *>(message);
            
            if (u.i & 0x7)
            {
                memcpy(buf, message, length);
                u.p64 = buf;
            }

            size_t remainder = length%32;
            uint64_t a=*hash1;
            uint64_t b=*hash2;
            uint64_t c=sc_const;
            uint64_t d=sc_const;

            if (length > 15)
            {
                const uint64_t *end_ = u.p64 + (length/32)*4;
                
                // handle all complete sets of 32 bytes
                for (; u.p64 < end_; u.p64 += 4)
                {
                    c += u.p64[0];
                    d += u.p64[1];
                    shortMix(a,b,c,d);
                    a += u.p64[2];
                    b += u.p64[3];
                }
                
                //Handle the case of 16+ remaining bytes.
                if (remainder >= 16)
                {
                    c += u.p64[0];
                    d += u.p64[1];
                    shortMix(a,b,c,d);
                    u.p64 += 2;
                    remainder -= 16;
                }
            }
            
            // Handle the last 0..15 bytes, and its length
            d += static_cast<uint64_t>(length) << 56;
            switch (remainder)
            {
            case 15:
            d += static_cast<uint64_t>(u.p8[14]) << 48;
            case 14:
                d += static_cast<uint64_t>(u.p8[13]) << 40;
            case 13:
                d += static_cast<uint64_t>(u.p8[12]) << 32;
            case 12:
                d += u.p32[2];
                c += u.p64[0];
                break;
            case 11:
                d += static_cast<uint64_t>(u.p8[10]) << 16;
            case 10:
                d += static_cast<uint64_t>(u.p8[9]) << 8;
            case 9:
                d += static_cast<uint64_t>(u.p8[8]);
            case 8:
                c += u.p64[0];
                break;
            case 7:
                c += static_cast<uint64_t>(u.p8[6]) << 48;
            case 6:
                c += static_cast<uint64_t>(u.p8[5]) << 40;
            case 5:
                c += static_cast<uint64_t>(u.p8[4]) << 32;
            case 4:
                c += u.p32[0];
                break;
            case 3:
                c += static_cast<uint64_t>(u.p8[2]) << 16;
            case 2:
                c += static_cast<uint64_t>(u.p8[1]) << 8;
            case 1:
                c += static_cast<uint64_t>(u.p8[0]);
                break;
            case 0:
                c += sc_const;
                d += sc_const;
            }
            shortEnd(a,b,c,d);
            *hash1 = a;
            *hash2 = b;
        }
    };
}
// namespace notima

#endif // NOTIMA_SPOOKYV2_HPP
