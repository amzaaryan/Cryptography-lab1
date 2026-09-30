#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include "BigInteger_Class.h"
#include "gcd.h"
#include "ExtendedGCD.h"
#include "ModularInverse.h"
#include "Exponentiation.h"
#include <chrono>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <boost/multiprecision/cpp_int.hpp>
#include "BoostReference.h"
#include "Random512.h"

using namespace std;
using boost::multiprecision::cpp_int;
const string RANDOM_TOKEN = "RAND512";

bool commandExists(const string& commandName)
{
    string command = "command -v " + commandName + " >/dev/null 2>&1";
    return system(command.c_str()) == 0;
}

string normalizeInputToken(const string& value)
{
    string normalized;
    normalized.reserve(value.size());

    for (char c : value)
        normalized.push_back(toupper(static_cast<unsigned char>(c)));

    return normalized;
}

string runCommand(const string& command)
{
    string result;
    char buffer[256];

    FILE* pipe = popen(command.c_str(), "r");

    if (!pipe)
        return "";

    while (fgets(buffer, sizeof(buffer), pipe))
        result += buffer;

    pclose(pipe);

    while (!result.empty() &&
           (result.back() == '\n' || result.back() == '\r'))
        result.pop_back();

    return result;
}

string shellEscape(const string& text)
{
    string result = "'";

    for (char c : text)
    {
        if (c == '\'')
            result += "'\\''";
        else
            result += c;
    }

    result += "'";
    return result;
}

void showMessage(const string& title, const string& message)
{
    cout << "\\n========== " << title << " ==========\\n";
    cout << message << endl;
    cout << "========================================\\n";

    const string filename = "/tmp/bigint_result.txt";
    ofstream file(filename.c_str());

    if (!file)
    {
        cerr << "Could not write result file: " << filename << endl;
        return;
    }

    file << message;
    file.close();

    string command =
        "zenity --text-info --title=" + shellEscape(title) +
        " --filename=" + shellEscape(filename) +
        " --width=900 --height=700";

    int status = system(command.c_str());

    if (status != 0)
        cerr << "Zenity result dialog failed. See results printed above." << endl;
}

void showError(const string& message)
{
    if (!commandExists("zenity"))
    {
        cerr << "Input Error: " << message << endl;
        return;
    }

    string command =
        "zenity --error --title='Input Error' --text=" +
        shellEscape(message);

    system(command.c_str());
}

bool getNumbers(string& a, string& b)
{
    string command =
        "zenity --forms "
        "--title='Big Integer Calculator' "
        "--text='Enter two integers. Type RAND512 in any field to generate a secure 512-bit value.' "
        "--width=550 "
        "--add-entry='Number 1' "
        "--add-entry='Number 2'";

    string input = runCommand(command);

    if (input.empty())
        return false;

    size_t separator = input.find('|');

    if (separator == string::npos)
    {
        showError("Please enter both numbers.");
        return false;
    }

    a = input.substr(0, separator);
    b = input.substr(separator + 1);

    if (a.empty() || b.empty())
    {
        showError("Input fields cannot be empty.");
        return false;
    }

    return true;
}

bool getThreeNumbers(string& a, string& e, string& m)
{
    string command =
        "zenity --forms "
        "--title='Modular Exponentiation' "
        "--text='Enter base, exponent and modulus. Type RAND512 in any field to generate a secure 512-bit value.' "
        "--width=550 "
        "--add-entry='Base (a)' "
        "--add-entry='Exponent (e)' "
        "--add-entry='Modulus (m)'";

    string input = runCommand(command);

    if (input.empty())
        return false;

    size_t first = input.find('|');

    if (first == string::npos)
        return false;

    size_t second = input.find('|', first + 1);

    if (second == string::npos)
        return false;

    a = input.substr(0, first);
    e = input.substr(first + 1, second - first - 1);
    m = input.substr(second + 1);

    if (a.empty() || e.empty() || m.empty())
    {
        showError("All fields are required.");
        return false;
    }

    return true;
}

bool replaceRandomToken(string& value)
{
    if (normalizeInputToken(value) != RANDOM_TOKEN)
        return true;

    string generatedValue;
    string errorMessage;

    if (!generateSecureRandom512Decimal(generatedValue, errorMessage))
    {
        showError(errorMessage);
        return false;
    }

    value = generatedValue;
    return true;
}

string formatSignedInteger(const SignedBigInteger& number)
{
    string value = number.value.toString();

    if (number.negative && value != "0")
        value = "-" + value;

    return value;
}

string signedBoostString(const SignedBigInteger& number)
{
    string value = number.value.toString();

    if (number.negative && value != "0")
        value = "-" + value;

    return value;
}

string formatTime(double milliseconds)
{
    ostringstream out;
    out << fixed << setprecision(6) << milliseconds << " ms";
    return out.str();
}

template <typename Function>
double measureTime(Function function)
{
    auto start = chrono::high_resolution_clock::now();

    function();

    auto stop = chrono::high_resolution_clock::now();

    return chrono::duration<double, milli>(stop - start).count();
}

void calculateGCD(const BigInteger& a, const BigInteger& b)
{
    BigInteger myResult = gcd(a, b);

    cpp_int ba(a.toString());
    cpp_int bb(b.toString());

    cpp_int boostResult = boostGCD(ba, bb);

    string myValue = myResult.toString();
    string boostValue = boostResult.str();

    string output =
        "YOUR BIGINTEGER GCD:\n" + myValue +
        "\n\nBOOST GCD:\n" + boostValue +
        "\n\nSTATUS: " +
        (myValue == boostValue ? "MATCH" : "MISMATCH");

    showMessage("GCD Comparison", output);
}

void calculateExtendedGCD(const BigInteger& a, const BigInteger& b)
{
    BigInteger gcdResult;
    SignedBigInteger x, y;

    extendedGCD(a, b, gcdResult, x, y);

    cpp_int ba(a.toString());
    cpp_int bb(b.toString());

    BoostExtendedResult boostResult = boostExtendedGCD(ba, bb);

    cpp_int myGcd(gcdResult.toString());
    cpp_int myX(signedBoostString(x));
    cpp_int myY(signedBoostString(y));

    bool gcdMatch = myGcd == boostResult.gcd;
    bool identityMatch = ba * myX + bb * myY == myGcd;

    string output =
        "YOUR BIGINTEGER RESULTS:\n"
        "GCD = " + gcdResult.toString() +
        "\nx = " + formatSignedInteger(x) +
        "\ny = " + formatSignedInteger(y) +

        "\n\nBOOST RESULTS:\n"
        "GCD = " + boostResult.gcd.str() +
        "\nx = " + boostResult.x.str() +
        "\ny = " + boostResult.y.str() +

        "\n\nGCD MATCH: " + string(gcdMatch ? "YES" : "NO") +
        "\nYOUR BEZOUT IDENTITY: " +
        string(identityMatch ? "VALID" : "INVALID");

    showMessage("Extended GCD Comparison", output);
}

void calculateModularInverse(const BigInteger& a, const BigInteger& m)
{
    BigInteger myResult;

    bool mySuccess = modularInverse(a, m, myResult);

    cpp_int ba(a.toString());
    cpp_int bm(m.toString());
    cpp_int boostResult;

    bool boostSuccess = boostModularInverse(ba, bm, boostResult);

    if (!mySuccess && !boostSuccess)
    {
        showMessage(
            "Modular Inverse Comparison",
            "Both implementations report that the inverse does not exist."
        );
        return;
    }

    if (mySuccess != boostSuccess)
    {
        showMessage(
            "Modular Inverse Comparison",
            "MISMATCH: Implementations disagree on whether an inverse exists."
        );
        return;
    }

    string myValue = myResult.toString();
    string boostValue = boostResult.str();

    string output =
        "YOUR MODULAR INVERSE:\n" + myValue +
        "\n\nBOOST MODULAR INVERSE:\n" + boostValue +
        "\n\nSTATUS: " +
        (myValue == boostValue ? "MATCH" : "MISMATCH");

    showMessage("Modular Inverse Comparison", output);
}

void calculateNaiveExponentiation(
    const BigInteger& base,
    const BigInteger& exponent,
    const BigInteger& modulus)
{
    BigInteger myResult;

    bool mySuccess = false;

    double myTime = measureTime([&]()
    {
        mySuccess = naiveExponentiation(
            base, exponent, modulus, myResult);
    });

    cpp_int ba(base.toString());
    cpp_int be(exponent.toString());
    cpp_int bm(modulus.toString());
    cpp_int boostResult;

    bool boostSuccess = false;

    double boostTime = measureTime([&]()
    {
        boostSuccess = boostNaiveExponentiation(
            ba, be, bm, boostResult);
    });

    if (!mySuccess)
    {
        showError("Your BigInteger exponentiation failed.");
        return;
    }

    string myValue = myResult.toString();
    string boostValue = boostResult.str();

    string output =
        "YOUR BIGINTEGER RESULT:\n" + myValue +
        "\n\nBOOST RESULT:\n" + boostValue +
        "\n\nSTATUS: " +
        (myValue == boostValue ? "MATCH" : "MISMATCH") +
        "\n\nYOUR EXECUTION TIME: " + formatTime(myTime) +
        "\nBOOST EXECUTION TIME: " + formatTime(boostTime);

    showMessage("Naive Exponentiation Comparison", output);
}

void calculateSquareAndMultiply(
    const BigInteger& base,
    const BigInteger& exponent,
    const BigInteger& modulus)
{
    BigInteger myResult;

    bool mySuccess = false;

    double myTime = measureTime([&]()
    {
        mySuccess = squareAndMultiply(
            base, exponent, modulus, myResult);
    });

    cpp_int ba(base.toString());
    cpp_int be(exponent.toString());
    cpp_int bm(modulus.toString());
    cpp_int boostResult;

    bool boostSuccess = false;

    double boostTime = measureTime([&]()
    {
        boostSuccess = boostSquareAndMultiply(
            ba, be, bm, boostResult);
    });

    if (!mySuccess)
    {
        showError("Your BigInteger exponentiation failed.");
        return;
    }

    string myValue = myResult.toString();
    string boostValue = boostResult.str();

    string output =
        "YOUR BIGINTEGER RESULT:\n" + myValue +
        "\n\nBOOST RESULT:\n" + boostValue +
        "\n\nSTATUS: " +
        (myValue == boostValue ? "MATCH" : "MISMATCH") +
        "\n\nYOUR EXECUTION TIME: " + formatTime(myTime) +
        "\nBOOST EXECUTION TIME: " + formatTime(boostTime);

    showMessage("Square-and-Multiply Comparison", output);
}

int main()
{
    if (!commandExists("zenity"))
    {
        cerr << "Zenity is required but was not found. Please install zenity and run again." << endl;
        return 1;
    }

    while (true)
    {
        string operation = runCommand(
            "zenity --list "
            "--title='Big Integer Calculator' "
            "--text='Select an operation' "
            "--column='Operation' "
            "GCD "
            "'Extended GCD' "
            "'Modular Inverse' "
            "'Naive Exponentiation' "
            "'Square-and-Multiply' "
            "--height=350 "
            "--width=450"
        );

        if (operation.empty())
            break;

        if (operation == "Naive Exponentiation" ||
            operation == "Square-and-Multiply")
        {
            string baseText, exponentText, modulusText;

            if (!getThreeNumbers(baseText, exponentText, modulusText))
                continue;

            if (!replaceRandomToken(baseText) ||
                !replaceRandomToken(exponentText) ||
                !replaceRandomToken(modulusText))
            {
                continue;
            }

            try
            {
                BigInteger base(baseText);
                BigInteger exponent(exponentText);
                BigInteger modulus(modulusText);

                if (operation == "Naive Exponentiation")
                {
                    calculateNaiveExponentiation(
                        base, exponent, modulus);
                }
                else
                {
                    calculateSquareAndMultiply(
                        base, exponent, modulus);
                }
            }
            catch (...)
            {
                showError("Invalid integer input.");
            }

            continue;
        }

        string aText, bText;

        if (!getNumbers(aText, bText))
            continue;

        if (!replaceRandomToken(aText) || !replaceRandomToken(bText))
            continue;

        try
        {
            BigInteger a(aText);
            BigInteger b(bText);

            if (operation == "GCD")
            {
                calculateGCD(a, b);
            }
            else if (operation == "Extended GCD")
            {
                calculateExtendedGCD(a, b);
            }
            else if (operation == "Modular Inverse")
            {
                calculateModularInverse(a, b);
            }
        }
        catch (...)
        {
            showError("Invalid input. Please enter valid integers.");
        }
    }

    return 0;
}