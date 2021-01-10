
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

N = 63

print((' ' * 4) + 'static constexpr uint64_t* binom[] =')
print((' ' * 8) + '{')
for n in range(1, N+1):
    row = []
    for k in range(0, n+1):
        row.append(choose(n,k))
    ss = [hex(x) + 'ull' for x in row]
    comma = ''
    if n < N:
        comma = ','

    print((' ' * 12) + '(uint64_t[]){' + ', '.join(ss) + '}' + comma)

print((' ' * 8) + '};')
