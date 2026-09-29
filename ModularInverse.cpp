#include "ModularInverse.h"

BigInteger modularInverseRemainder(BigInteger a, BigInteger m)
{
    BigInteger q = a / m;
    BigInteger r = a - q * m;
    return r;
}

bool modularInverse(BigInteger a, BigInteger m, BigInteger& result)
{
    if (m.isZero())
        return false;

    BigInteger gcdResult;
    SignedBigInteger x, y;

    extendedGCD(a, m, gcdResult, x, y);

    if (!(gcdResult == BigInteger("1")))
        return false;

    BigInteger r = modularInverseRemainder(x.value, m);

    if (x.negative && !r.isZero())
        result = m - r;
    else
        result = r;

    return true;
}