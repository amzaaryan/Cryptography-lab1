#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <string>

using namespace std;

class BigInteger
{
private:
    vector<uint64_t> chunks;
    bool negative = false;

public:
    BigInteger();
    BigInteger(const vector<uint64_t> &values);
    BigInteger(const string &number);

    bool operator<(const BigInteger &other) const;
    bool operator==(const BigInteger& other) const;

    bool isZero() const;

    BigInteger operator+(const BigInteger &other) const;
    BigInteger operator-(const BigInteger &other) const;
    BigInteger operator*(const BigInteger &other) const;
    BigInteger operator/(const BigInteger &other) const;

    string toString() const;
    void printChunks() const;
};

#endif