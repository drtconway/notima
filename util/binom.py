
def fac(n):
    r = 1
    while n > 1:
        r *= n
        n -= 1
    return r

def choose(n, k):
    if k == 0 or k == n:
        return 1
    return fac(n) // (fac(k) * fac(n - k))

def ind(i):
    return ' ' * (4*i)

def bit_width(x):
    n = 0
    while (x > 0):
        n += 1
        x >>= 1
    return n

N = 63

print(ind(2) + 'static constexpr uint64_t* binom[] =')
print(ind(3) + '{')
for n in range(0, N+1):
    row = []
    for k in range(0, n+1):
        row.append(choose(n,k))
    ss = [hex(x) + 'ull' for x in row]
    comma = ''
    if n < N:
        comma = ','
    print(ind(4) + '(uint64_t[]){' + ', '.join(ss) + '}' + comma)
print(ind(3) + '};')
print()
print(ind(2) + 'static constexpr uint8_t* binom_bits[] =')
print(ind(3) + '{')
for n in range(0, N+1):
    row = []
    for k in range(0, n+1):
        row.append(bit_width(choose(n,k) - 1))
    ss = [hex(x) + 'ull' for x in row]
    comma = ''
    if n < N:
        comma = ','
    print(ind(4) + '(uint8_t[]){' + ', '.join(ss) + '}' + comma)
print(ind(3) + '};')

