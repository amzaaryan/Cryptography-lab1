# Cryptography Lab 1: Modular Arithmetic and Extended Euclidean Algorithm

This project implements the main number-theoretic operations used in introductory cryptography. It also provides a small Linux calculator application that compares the custom implementations with Boost.Multiprecision.

## Lab objective

The objective of this lab is to understand and implement fundamental number-theoretic operations used throughout cryptography:

- Divisibility and remainders
- GCD using the Euclidean algorithm
- Extended Euclidean algorithm
- Bézout coefficients `x` and `y` satisfying `ax + by = gcd(a, b)`
- Modular addition and multiplication
- Modular inverse
- Modular exponentiation using repeated squaring
- Verification against a trusted library
- Performance comparison of naive exponentiation and square-and-multiply
- A calculator application using inputs of at least 512 bits

## Features and functionality

### 1. Custom arbitrary-precision integers

`BigInteger` stores large non-negative integers as a vector of 64-bit chunks. This is necessary because normal C++ integer types cannot represent 512-bit values.

The class supports addition, subtraction, multiplication, division, comparison, and conversion between decimal strings and `BigInteger` values. Signed values are represented where needed by the extended Euclidean algorithm.

### 2. Euclidean GCD

The Euclidean algorithm repeatedly applies:

```text
r = a mod b
(a, b) = (b, r)
```

When the remainder becomes zero, the remaining non-zero value is `gcd(a, b)`.

Implementation: `gcd1.cpp`.

### 3. Extended Euclidean algorithm

The extended algorithm calculates the GCD and coefficients `x` and `y` such that:

```text
a*x + b*y = gcd(a, b)
```

These Bézout coefficients are also used to calculate modular inverses.

Implementation: `ExtendedGCD1.cpp`.

### 4. Modular addition and multiplication

The project implements:

```text
(a + b) mod m
(a * b) mod m
```

The normal operation is performed first, followed by division-based remainder reduction.

Implementation: `ModularArithmetic.cpp`.

### 5. Modular inverse

The modular inverse of `a` modulo `m` is a value `a⁻¹` satisfying:

```text
(a * a⁻¹) mod m = 1
```

The inverse exists only when `gcd(a, m) = 1`. The implementation uses the coefficient returned by the extended Euclidean algorithm and normalizes the result into `[0, m - 1]`.

Implementation: `ModularInverse.cpp`.

### 6. Modular exponentiation

The project includes two exponentiation methods.

#### Naive exponentiation

The result is multiplied by the base once for every unit in the exponent. This method is included as a baseline for the performance experiment.

#### Square-and-multiply

The exponent is repeatedly divided by two. The base is squared at every step, and it is multiplied into the result only when the current exponent bit is one. This requires far fewer operations for large exponents.

Implementations: `Exponentiation.cpp` and `ModularArithmetic.cpp`.

### 7. Secure 512-bit input generation

The calculator accepts the special value `RAND512` in any number field. When entered, the program generates a positive 512-bit decimal integer using the Linux secure random source. The most significant bit is set, ensuring that the value is exactly 512 bits rather than merely up to 512 bits.

This makes it easier to perform the required lab experiment with large inputs while preserving the option to enter numbers manually.

Implementation: `Random512.cpp` and `Random512.h`.

### 8. Calculator GUI

`gui.cpp` provides a Zenity-based Linux interface with these operations:

- GCD
- Extended GCD
- Modular inverse
- Naive exponentiation
- Square-and-multiply exponentiation

For each operation, the program displays the custom result, the corresponding Boost result, and whether they match. For exponentiation, execution times are also displayed.

### 9. Verification against Boost

`BoostReference.h` uses `boost::multiprecision::cpp_int` as an independent reference implementation. The application compares the custom results with Boost results to help detect arithmetic errors.

Boost is used as a trusted comparison implementation; the main arithmetic algorithms remain implemented by this project.

## Complexity analysis

Let `n` be the number of 64-bit chunks in a `BigInteger`, `L` the bit length, `e` the numerical value of the exponent, and `k` the number of Euclidean iterations.

| Operation | Time complexity | Explanation |
|---|---:|---|
| Comparison | `O(n)` | Compares chunks from most significant to least significant. |
| Addition | `O(n)` | Visits each chunk once and propagates carry. |
| Subtraction | `O(n)` | Visits each chunk once and propagates borrow. |
| Multiplication | `O(n²)` | Uses schoolbook multiplication over every pair of chunks. |
| Division | `O(n²)` | Processes chunks and uses binary search for each quotient chunk. |
| Decimal conversion | Approximately `O(n²)` | Repeatedly divides by 10 to generate decimal digits. |
| Modular addition | `O(n²)` | Addition followed by division-based reduction. |
| Modular multiplication | `O(n²)` | Multiplication followed by division-based reduction. |
| Euclidean GCD | `O(k·n²)` | Performs `k` iterations, each using division. |
| Extended GCD | `O(k·n²)` | Same division cost as GCD plus coefficient updates. |
| Modular inverse | `O(k·n²)` | Uses the extended Euclidean algorithm. |
| Naive exponentiation | `O(e·n²)` | Performs one modular multiplication for every unit in the exponent. |
| Square-and-multiply | `O(n² log e)` | Performs `O(log e)` modular squarings and multiplications. |

At the algorithm level, the key comparison is:

```text
Naive exponentiation:       O(e)
Square-and-multiply:        O(log e)
```

The `n²` factor appears because each step uses the current schoolbook big-integer arithmetic and division implementation. For a 512-bit value, `n` is approximately `512 / 64 = 8` chunks.

## Technologies used

### C++

C++ is used because the lab requires implementing the algorithms directly and provides control over data representation, arithmetic operations, timing, and memory usage.

### Custom `BigInteger`

The custom class demonstrates how arbitrary-precision integers can be represented and manipulated when built-in C++ types are too small for 512-bit inputs.

### Boost.Multiprecision

Boost.Multiprecision provides `cpp_int`, an arbitrary-precision integer type. It is used as a trusted reference implementation for checking custom results.

### Zenity

Zenity provides simple graphical dialogs from Linux shell commands. It allows the calculator to use selection lists, input forms, error dialogs, and result windows without requiring a large GUI framework.

### Linux secure random source

The random 512-bit values are generated using the operating system's secure random source rather than a predictable seed such as the current time. This is appropriate for cryptography-related experiments and produces unpredictable test values.

### C++ `<chrono>`

`std::chrono` is used to measure the running time of naive exponentiation and square-and-multiply.

## Requirements

### Ubuntu, Debian, or Linux Mint

```bash
sudo apt update
sudo apt install -y git g++ build-essential libboost-dev zenity
```

### Fedora

```bash
sudo dnf install -y git gcc-c++ boost-devel zenity
```

### Arch Linux

```bash
sudo pacman -S --needed git gcc boost zenity
```

## Build and run on Linux

Clone the repository:

```bash
git clone https://github.com/amzaaryan/Cryptography-lab1.git
cd Cryptography-lab1
```

Build the calculator:

```bash
g++ -std=c++17 -O2 -Wall -Wextra \\
    BigInteger.cpp gcd1.cpp ExtendedGCD1.cpp \\
    ModularArithmetic.cpp ModularInverse.cpp Exponentiation.cpp \\
    Random512.cpp gui.cpp -o cryptography_lab1
```

Run it from a graphical Linux desktop session:

```bash
./cryptography_lab1
```

Zenity requires an active graphical display. If the program is run through SSH, X11 forwarding or another graphical-session configuration may be required.

## Using the calculator

1. Start the program with `./cryptography_lab1`.
2. Select an operation from the menu.
3. Enter decimal integers manually, or enter `RAND512` in a number field.
4. For exponentiation, provide a base, exponent, and modulus.
5. Read the custom result and the Boost reference result.
6. Confirm that the status reports `MATCH`.
7. Compare the execution time of naive exponentiation and square-and-multiply.

A modular inverse exists only when the selected number and modulus are coprime. The modulus must not be zero.

## Suggested lab experiment

1. Run GCD with two large numbers and verify that the custom result matches Boost.
2. Run Extended GCD and check that `a*x + b*y = gcd(a, b)`.
3. Select values where `gcd(a, m) = 1` and verify the modular inverse.
4. Use `RAND512` to generate 512-bit base, exponent, and modulus values.
5. Run naive exponentiation and record its execution time.
6. Run square-and-multiply with the same inputs and record its execution time.
7. Repeat with increasingly large exponents.
8. Explain why square-and-multiply scales better: it uses logarithmically many exponentiation steps instead of one step per exponent unit.
9. Confirm that both methods produce the same result as the Boost reference implementation.

For a fair timing comparison, use the same base, exponent, and modulus for both exponentiation methods. Very large exponents can make naive exponentiation extremely slow, which is expected because of its linear dependence on the exponent value.

## Source-file overview

| File | Purpose |
|---|---|
| `BigInteger_Class.h` | Declaration of the custom arbitrary-precision integer class. |
| `BigInteger.cpp` | Big-integer representation and arithmetic operations. |
| `gcd.h`, `gcd1.cpp` | Euclidean GCD. |
| `ExtendedGCD.h`, `ExtendedGCD1.cpp` | Extended Euclidean algorithm and signed coefficients. |
| `ModularArithmetic.h`, `ModularArithmetic.cpp` | Modular addition, multiplication, and repeated-squaring helper. |
| `ModularInverse.h`, `ModularInverse.cpp` | Modular inverse calculation. |
| `Exponentiation.h`, `Exponentiation.cpp` | Naive and square-and-multiply exponentiation. |
| `Random512.h`, `Random512.cpp` | Secure 512-bit random number generation. |
| `Random512_test.cpp` | Focused checks for generated 512-bit values. |
| `BoostReference.h` | Boost-based reference implementations. |
| `gui.cpp` | Zenity calculator interface, verification, random-input handling, and timing. |

## Optional random-input helper test

```bash
g++ -std=c++17 -O2 -Wall -Wextra Random512.cpp Random512_test.cpp -o random512_test
./random512_test
```

The test checks that generated values are positive decimal numbers with a bit length of exactly 512.

## Important note

This is an educational implementation for a cryptography laboratory. It is useful for understanding algorithms and comparing performance, but it should not be treated as a production cryptographic library without further security review, testing, and side-channel analysis.
