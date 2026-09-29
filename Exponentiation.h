#ifndef EXPONENTIATION_H
#define EXPONENTIATION_H

#include "BigInteger_Class.h"

bool naiveExponentiation(
    const BigInteger& base,
    const BigInteger& exponent,
    const BigInteger& modulus,
    BigInteger& result
);

bool squareAndMultiply(
    const BigInteger& base,
    const BigInteger& exponent,
    const BigInteger& modulus,
    BigInteger& result
);

#endif