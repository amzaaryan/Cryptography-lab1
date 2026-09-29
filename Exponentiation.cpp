#include "Exponentiation.h"
#include "ModularArithmetic.h"

bool naiveExponentiation(
    const BigInteger& base,
    const BigInteger& exponent,
    const BigInteger& modulus,
    BigInteger& result)
{
    if (modulus.isZero())
        return false;

    BigInteger zero("0");
    BigInteger one("1");

    if (exponent < zero)
        return false;

    BigInteger power = exponent;
    BigInteger current("1");

    if (!modularMultiplication(current, one, modulus, current))
        return false;

    BigInteger reducedBase;

    if (!modularMultiplication(base, one, modulus, reducedBase))
        return false;

    while (!(power == zero))
    {
        if (!modularMultiplication(
                current, reducedBase, modulus, current))
            return false;

        power = power - one;
    }

    result = current;
    return true;
}

bool squareAndMultiply(
    const BigInteger& base,
    const BigInteger& exponent,
    const BigInteger& modulus,
    BigInteger& result)
{
    if (modulus.isZero())
        return false;

    BigInteger zero("0");
    BigInteger one("1");
    BigInteger two("2");

    if (exponent < zero)
        return false;

    BigInteger power = exponent;
    BigInteger current("1");

    if (!modularMultiplication(current, one, modulus, current))
        return false;

    BigInteger reducedBase;

    if (!modularMultiplication(base, one, modulus, reducedBase))
        return false;

    while (!(power == zero))
    {
        BigInteger half = power / two;
        BigInteger remainder = power - (half * two);

        if (!(remainder == zero))
        {
            if (!modularMultiplication(
                    current, reducedBase, modulus, current))
                return false;
        }

        power = half;

        if (!(power == zero))
        {
            if (!modularMultiplication(
                    reducedBase, reducedBase, modulus, reducedBase))
                return false;
        }
    }

    result = current;
    return true;
}