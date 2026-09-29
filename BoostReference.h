#ifndef BOOST_REFERENCE_H
#define BOOST_REFERENCE_H

#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::cpp_int;

struct BoostExtendedResult
{
    cpp_int gcd;
    cpp_int x;
    cpp_int y;
};

inline cpp_int boostGCD(cpp_int a, cpp_int b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;

    while (b != 0)
    {
        cpp_int r = a % b;
        a = b;
        b = r;
    }

    return a;
}

inline BoostExtendedResult boostExtendedGCD(cpp_int a, cpp_int b)
{
    cpp_int oldR = a, r = b;
    cpp_int oldS = 1, s = 0;
    cpp_int oldT = 0, t = 1;

    while (r != 0)
    {
        cpp_int q = oldR / r;

        cpp_int temp = oldR - q * r;
        oldR = r;
        r = temp;

        temp = oldS - q * s;
        oldS = s;
        s = temp;

        temp = oldT - q * t;
        oldT = t;
        t = temp;
    }

    if (oldR < 0)
    {
        oldR = -oldR;
        oldS = -oldS;
        oldT = -oldT;
    }

    return {oldR, oldS, oldT};
}

inline bool boostModularInverse(cpp_int a, cpp_int m, cpp_int& result)
{
    if (m <= 1)
        return false;

    BoostExtendedResult eg = boostExtendedGCD(a, m);

    if (eg.gcd != 1)
        return false;

    result = eg.x % m;

    if (result < 0)
        result += m;

    return true;
}

inline bool boostNaiveExponentiation(
    cpp_int base,
    cpp_int exponent,
    cpp_int modulus,
    cpp_int& result)
{
    if (modulus <= 0 || exponent < 0)
        return false;

    result = 1 % modulus;
    base %= modulus;

    for (cpp_int i = 0; i < exponent; ++i)
        result = (result * base) % modulus;

    return true;
}

inline bool boostSquareAndMultiply(
    cpp_int base,
    cpp_int exponent,
    cpp_int modulus,
    cpp_int& result)
{
    if (modulus <= 0 || exponent < 0)
        return false;

    result = 1 % modulus;
    base %= modulus;

    while (exponent > 0)
    {
        if (exponent % 2 != 0)
            result = (result * base) % modulus;

        base = (base * base) % modulus;
        exponent /= 2;
    }

    return true;
}

#endif