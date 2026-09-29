#include "ExtendedGCD.h"

SignedBigInteger::SignedBigInteger()
{
    value = BigInteger();
    negative = false;
}

SignedBigInteger::SignedBigInteger(const BigInteger &v)
{
    value = v;
    negative = false;
}

SignedBigInteger signedSubtract(const SignedBigInteger &a, const SignedBigInteger &b)
{
    SignedBigInteger result;
    if (
        a.negative ==
        b.negative)
    {

        if (a.value < b.value)
        {

            result.value =
                b.value - a.value;

            result.negative =
                !a.negative;
        }
        else
        {

            result.value =
                a.value - b.value;

            result.negative =
                a.negative;
        }
    }
    else
    {

        result.value =
            a.value + b.value;

        result.negative =
            a.negative;
    }

    return result;
}

SignedBigInteger signedMultiply(const SignedBigInteger &a, const BigInteger &b)
{

    SignedBigInteger result;

    result.value =
        a.value * b;

    result.negative =
        a.negative;

    return result;
}

void extendedGCD(
    BigInteger a,
    BigInteger b,
    BigInteger &gcdResult,
    SignedBigInteger &x,
    SignedBigInteger &y)
{

    BigInteger old_r = a;
    BigInteger r = b;

    SignedBigInteger old_s(
        BigInteger(
            vector<uint64_t>{1}));

    SignedBigInteger s;

    SignedBigInteger old_t;

    SignedBigInteger t(
        BigInteger(
            vector<uint64_t>{1}));

    while (!r.isZero())
    {

        BigInteger q =
            old_r / r;

        BigInteger temp_r =
            old_r - q * r;

        old_r = r;
        r = temp_r;

        SignedBigInteger q_s =
            signedMultiply(
                s,
                q);

        SignedBigInteger temp_s =
            signedSubtract(
                old_s,
                q_s);

        old_s = s;
        s = temp_s;

        SignedBigInteger q_t =
            signedMultiply(
                t,
                q);

        SignedBigInteger temp_t =
            signedSubtract(
                old_t,
                q_t);

        old_t = t;
        t = temp_t;
    }

    gcdResult = old_r;
    x = old_s;
    y = old_t;
}