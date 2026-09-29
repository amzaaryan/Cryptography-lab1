#include "BigInteger_Class.h"
#include <algorithm>
#include <stdexcept>

BigInteger::BigInteger()
{
    chunks.push_back(0);
}

BigInteger::BigInteger(const vector<uint64_t> &values)
{
    chunks = values;

    if (chunks.empty())
        chunks.push_back(0);
}

BigInteger::BigInteger(const string &number)
{

    if (number.empty())
        throw invalid_argument("Empty number");

    for (size_t i = 0; i < number.size(); i++)
    {

        if (number[i] < '0' ||
            number[i] > '9')
        {

            throw invalid_argument("Invalid number");
        }
    }

    chunks.push_back(0);

    for (size_t i = 0; i < number.size(); i++)
    {

        uint64_t digit =
            number[i] - '0';

        uint64_t carry = digit;

        for (size_t j = 0;
             j < chunks.size();
             j++)
        {

            uint64_t low =
                chunks[j] & 0xFFFFFFFFULL;

            uint64_t high =
                chunks[j] >> 32;

            uint64_t lowProduct =
                low * 10 + carry;

            uint64_t newLow =
                lowProduct & 0xFFFFFFFFULL;

            uint64_t carryToHigh =
                lowProduct >> 32;

            uint64_t highProduct =
                high * 10 + carryToHigh;

            uint64_t newHigh =
                highProduct & 0xFFFFFFFFULL;

            carry =
                highProduct >> 32;

            chunks[j] =
                newLow |
                (newHigh << 32);
        }

        if (carry)
            chunks.push_back(carry);
    }
}

bool BigInteger::isZero() const
{
    return chunks.size() == 1 &&
           chunks[0] == 0;
}

bool BigInteger::operator<(const BigInteger &other) const
{

    if (chunks.size() !=
        other.chunks.size())
    {

        return chunks.size() <
               other.chunks.size();
    }

    for (size_t i = chunks.size();
         i-- > 0;)
    {

        if (chunks[i] !=
            other.chunks[i])
        {

            return chunks[i] <
                   other.chunks[i];
        }
    }

    return false;
}

bool BigInteger::operator==(const BigInteger& other) const
{
    if (isZero() && other.isZero())
        return true;

    if (negative != other.negative)
        return false;

    if (chunks.size() != other.chunks.size())
        return false;

    for (size_t i = 0; i < chunks.size(); i++)
    {
        if (chunks[i] != other.chunks[i])
            return false;
    }

    return true;
}

BigInteger BigInteger::operator+(const BigInteger &other) const
{
    BigInteger result;

    result.chunks.clear();

    size_t n = max(chunks.size(), other.chunks.size());

    uint64_t carry = 0;

    for (size_t i = 0; i < n; i++)
    {
        uint64_t a = (i < chunks.size()) ? chunks[i] : 0;
        uint64_t b = (i < other.chunks.size()) ? other.chunks[i] : 0;

        uint64_t sum1 = a + b;
        uint64_t carry1 = (sum1 < a);

        uint64_t sum2 = sum1 + carry;
        uint64_t carry2 = (sum2 < sum1);

        result.chunks.push_back(sum2);

        carry = carry1 || carry2;
    }

    if (carry)
        result.chunks.push_back(carry);

    return result;
}

BigInteger BigInteger::operator-(const BigInteger &other) const
{

    if (*this < other)
    {
        BigInteger result = other - *this;
        result.negative = true;
        return result;
    }

    BigInteger result;
    result.chunks.clear();

    size_t n = chunks.size();

    uint64_t borrow = 0;

    for (size_t i = 0; i < n; i++)
    {

        uint64_t a = chunks[i];

        uint64_t b = (i < other.chunks.size())
                         ? other.chunks[i]
                         : 0;

        uint64_t diff1 = a - b;

        uint64_t borrow1 = (a < b);

        uint64_t diff2 = diff1 - borrow;

        uint64_t borrow2 = (diff1 < borrow);

        result.chunks.push_back(diff2);

        borrow = borrow1 || borrow2;
    }

    while (result.chunks.size() > 1 &&
           result.chunks.back() == 0)
    {
        result.chunks.pop_back();
    }

    return result;
}

BigInteger BigInteger::operator*(const BigInteger &other) const
{

    BigInteger result;

    result.chunks.assign(
        chunks.size() + other.chunks.size() + 1, 0);

    for (size_t i = 0; i < chunks.size(); i++)
    {

        for (size_t j = 0; j < other.chunks.size(); j++)
        {

            uint64_t a = chunks[i];
            uint64_t b = other.chunks[j];

            uint64_t aLow = a & 0xFFFFFFFFULL;
            uint64_t aHigh = a >> 32;

            uint64_t bLow = b & 0xFFFFFFFFULL;
            uint64_t bHigh = b >> 32;

            uint64_t p0 = aLow * bLow;
            uint64_t p1 = aLow * bHigh;
            uint64_t p2 = aHigh * bLow;
            uint64_t p3 = aHigh * bHigh;

            uint64_t middle =
                (p0 >> 32) +
                (p1 & 0xFFFFFFFFULL) +
                (p2 & 0xFFFFFFFFULL);

            uint64_t low =
                (p0 & 0xFFFFFFFFULL) |
                ((middle & 0xFFFFFFFFULL) << 32);

            uint64_t high =
                p3 +
                (p1 >> 32) +
                (p2 >> 32) +
                (middle >> 32);

            size_t k = i + j;

            uint64_t old = result.chunks[k];

            result.chunks[k] += low;

            uint64_t carry = (result.chunks[k] < old);

            k++;

            old = result.chunks[k];

            result.chunks[k] += high;

            uint64_t carry2 = (result.chunks[k] < old);

            old = result.chunks[k];

            result.chunks[k] += carry;

            uint64_t carry3 = (result.chunks[k] < old);

            carry = carry2 || carry3;

            k++;

            while (carry)
            {

                old = result.chunks[k];

                result.chunks[k]++;

                carry = (result.chunks[k] < old);

                k++;
            }
        }
    }

    while (result.chunks.size() > 1 &&
           result.chunks.back() == 0)
    {
        result.chunks.pop_back();
    }

    return result;
}

BigInteger BigInteger::operator/(const BigInteger &other) const
{

    // Division by zero
    if (other.chunks.size() == 1 &&
        other.chunks[0] == 0)
    {

        throw runtime_error("Division by zero");
    }

    BigInteger quotient;
    quotient.chunks.assign(chunks.size(), 0);

    BigInteger remainder;
    remainder.chunks.clear();
    remainder.chunks.push_back(0);

    // Process from most significant chunk to least significant chunk
    for (size_t i = chunks.size(); i-- > 0;)
    {

        // remainder = remainder * 2^64 + chunks[i]
        remainder.chunks.insert(
            remainder.chunks.begin(),
            chunks[i]);

        // Remove unnecessary zero chunks
        while (remainder.chunks.size() > 1 &&
               remainder.chunks.back() == 0)
        {
            remainder.chunks.pop_back();
        }

        // Find the largest q such that:
        // other * q <= remainder

        uint64_t low = 0;
        uint64_t high = UINT64_MAX;
        uint64_t q = 0;

        while (low <= high)
        {

            uint64_t mid =
                low + (high - low) / 2;

            BigInteger midBig;
            midBig.chunks.clear();
            midBig.chunks.push_back(mid);

            BigInteger product = other * midBig;

            if (product < remainder ||
                product.chunks == remainder.chunks)
            {

                q = mid;
                low = mid + 1;
            }
            else
            {
                high = mid - 1;
            }
        }

        quotient.chunks[i] = q;

        // remainder = remainder - other * q
        BigInteger qBig;
        qBig.chunks.clear();
        qBig.chunks.push_back(q);

        BigInteger product = other * qBig;

        remainder = remainder - product;
    }

    // Remove leading zero chunks from quotient
    while (quotient.chunks.size() > 1 &&
           quotient.chunks.back() == 0)
    {
        quotient.chunks.pop_back();
    }

    return quotient;
}

string BigInteger::toString() const
{

    if (isZero())
        return "0";

    vector<uint64_t> temp =
        chunks;

    string result;

    while (
        !(temp.size() == 1 &&
          temp[0] == 0))
    {

        uint64_t remainder = 0;

        for (size_t i = temp.size();
             i-- > 0;)
        {

            uint64_t high =
                temp[i] >> 32;

            uint64_t low =
                temp[i] &
                0xFFFFFFFFULL;

            uint64_t n1 =
                (remainder << 32) +
                high;

            uint64_t qHigh =
                n1 / 10;

            remainder =
                n1 % 10;

            uint64_t n2 =
                (remainder << 32) +
                low;

            uint64_t qLow =
                n2 / 10;

            remainder =
                n2 % 10;

            temp[i] =
                (qHigh << 32) |
                qLow;
        }

        result +=
            char('0' + remainder);

        while (
            temp.size() > 1 &&
            temp.back() == 0)
        {

            temp.pop_back();
        }
    }

    reverse(
        result.begin(),
        result.end());

    if (
        negative &&
        result != "0")
    {

        result =
            "-" + result;
    }

    return result;
}

void BigInteger::printChunks() const
{
    cout << toString() << '\n';
}