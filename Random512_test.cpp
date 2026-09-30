#include "Random512.h"
#include <iostream>

using namespace std;

int main()
{
    for (int i = 0; i < 3; i++)
    {
        string decimalValue;
        string errorMessage;

        if (!generateSecureRandom512Decimal(decimalValue, errorMessage))
        {
            cerr << "Random generation failed: " << errorMessage << endl;
            return 1;
        }

        if (!isPositiveDecimalWithBitLength(decimalValue, 512))
        {
            cerr << "Generated value failed validation: " << decimalValue << endl;
            return 1;
        }
    }

    cout << "Random 512-bit generation checks passed." << endl;
    return 0;
}
