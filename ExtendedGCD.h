#ifndef EXTENDEDGCD_H
#define EXTENDEDGCD_H

#include "BigInteger_Class.h"

struct SignedBigInteger
{
    BigInteger value;
    bool negative;

    SignedBigInteger();
    SignedBigInteger(const BigInteger &v);
};

SignedBigInteger signedSubtract(
    const SignedBigInteger &a,
    const SignedBigInteger &b);

SignedBigInteger signedMultiply(
    const SignedBigInteger &a,
    const BigInteger &b);

void extendedGCD(
    BigInteger a,
    BigInteger b,
    BigInteger &gcdResult,
    SignedBigInteger &x,
    SignedBigInteger &y);

#endif