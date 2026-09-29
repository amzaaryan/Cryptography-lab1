#ifndef MODULARARITHMETIC_H
#define MODULARARITHMETIC_H

#include "BigInteger_Class.h"

bool modularAddition(
    const BigInteger& a,
    const BigInteger& b,
    const BigInteger& m,
    BigInteger& result);

bool modularMultiplication(
    const BigInteger& a,
    const BigInteger& b,
    const BigInteger& m,
    BigInteger& result);

bool modularExponentiation(
    const BigInteger& base,
    const BigInteger& exponent,
    const BigInteger& m,
    BigInteger& result);

#endif
