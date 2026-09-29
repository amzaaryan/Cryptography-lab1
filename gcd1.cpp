#include "gcd.h"

BigInteger gcd(BigInteger a, BigInteger b)
{
    while (!b.isZero())
    {
        BigInteger q = a / b;
        BigInteger r = a - q * b;

        a = b;
        b = r;
    }

    return a;
}