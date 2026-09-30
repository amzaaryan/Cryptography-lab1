#include "Random512.h"
#include <array>
#include <cctype>
#include <fstream>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

bool generateSecureRandom512Decimal(string& decimalValue, string& errorMessage)
{
    array<unsigned char, 64> randomBytes{};

    ifstream randomSource("/dev/urandom", ios::binary);

    if (!randomSource)
    {
        errorMessage = "Unable to open /dev/urandom for secure random data.";
        return false;
    }

    randomSource.read(reinterpret_cast<char*>(randomBytes.data()), randomBytes.size());

    if (randomSource.gcount() != static_cast<streamsize>(randomBytes.size()))
    {
        errorMessage = "Unable to read enough secure random data from /dev/urandom.";
        return false;
    }

    randomBytes[0] |= 0x80;

    cpp_int value = 0;

    for (unsigned char byte : randomBytes)
    {
        value <<= 8;
        value += byte;
    }

    decimalValue = value.str();
    return true;
}

bool isPositiveDecimalWithBitLength(const string& decimalValue, unsigned int bitLength)
{
    if (decimalValue.empty())
        return false;

    for (char c : decimalValue)
    {
        if (!isdigit(static_cast<unsigned char>(c)))
            return false;
    }

    cpp_int value(decimalValue);

    if (value <= 0)
        return false;

    unsigned int computedBitLength = 0;
    cpp_int temp = value;

    while (temp > 0)
    {
        temp >>= 1;
        computedBitLength++;
    }

    return computedBitLength == bitLength;
}
