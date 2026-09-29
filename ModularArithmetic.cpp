#include "ModularArithmetic.h"

bool modularAddition(
    const BigInteger& a,
    const BigInteger& b,
    const BigInteger& m,
    BigInteger& result)
{
    if (m.isZero())
        return false;

    BigInteger sum = a + b;
    BigInteger quotient = sum / m;

    result = sum - quotient * m;

    return true;
}

bool modularMultiplication(
    const BigInteger& a,
    const BigInteger& b,
    const BigInteger& m,
    BigInteger& result)
{
    if (m.isZero())
        return false;

    BigInteger product = a * b;
    BigInteger quotient = product / m;

    result = product - quotient * m;

    return true;
}

bool modularExponentiation(
    const BigInteger& base,
    const BigInteger& exponent,
    const BigInteger& m,
    BigInteger& result)
{
    if (m.isZero())
        return false;

    BigInteger currentBase = base;
    BigInteger currentExponent = exponent;
    BigInteger one("1");
    BigInteger zero("0");
    BigInteger two("2");

    BigInteger quotient = currentBase / m;
    currentBase = currentBase - quotient * m;

    result = one;

    while (zero < currentExponent)
    {
        BigInteger quotient = currentExponent / two;
        BigInteger remainder = currentExponent - quotient * two;

        if (remainder == one)
        {
            BigInteger temp;
            modularMultiplication(result, currentBase, m, temp);
            result = temp;
        }

        BigInteger temp;
        modularMultiplication(currentBase, currentBase, m, temp);
        currentBase = temp;

        currentExponent = quotient;
    }

    return true;
}