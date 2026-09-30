#ifndef RANDOM512_H
#define RANDOM512_H

#include <string>

bool generateSecureRandom512Decimal(std::string& decimalValue, std::string& errorMessage);
bool isPositiveDecimalWithBitLength(const std::string& decimalValue, unsigned int bitLength);

#endif
