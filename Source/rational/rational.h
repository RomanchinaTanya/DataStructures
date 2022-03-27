#ifndef RATIONAL_RATIONAL_H
#define RATIONAL_RATIONAL_H

//#include <util/constants.h>
#include <iostream>
#include <numeric>
#include <cmath>

class RationalDivisionByZero : public std::runtime_error {
public:
    RationalDivisionByZero() : std::runtime_error("RationalDivisionByZero") {
    }
};

class Rational {
public:
    int64_t numerator;
    int64_t denominator;

    // CONSTRUCT ~ DECTRUCT

    Rational();
    Rational(int64_t number);
    Rational(int64_t numerator_, int64_t denominator_);
    Rational(const Rational& other);
    Rational& operator=(const Rational& other);

    // USEFUL FUNCTIONS

    void Swap(Rational& other);
    int64_t GetNumerator() const;
    int64_t GetDenominator() const;
    void SetNumerator(int64_t numerator_);
    void SetDenominator(int64_t denominator_);
    void Reduce();

    // BINARY ~ RATIONAL (+=, -=, *=, /=, +, -, *, /)

    Rational& operator+=(const Rational& other);
    Rational& operator-=(const Rational& other);
    Rational& operator*=(const Rational& other);
    Rational& operator/=(const Rational& other);
    Rational operator+(const Rational& other) const;
    Rational operator-(const Rational& other) const;
    Rational operator*(const Rational& other) const;
    Rational operator/(const Rational& other) const;

    // UNARY

    Rational operator+() const;
    Rational operator-() const;

    // INCREMENT ~ DECREMENT

    Rational& operator++();
    Rational& operator--();
    Rational operator++(int32_t);
    Rational operator--(int32_t);

    // COMPARISON

    bool operator<(const Rational& other) const;
    bool operator>(const Rational& other) const;
    bool operator==(const Rational& other) const;
    bool operator!=(const Rational& other) const;
    bool operator<=(const Rational& other) const;
    bool operator>=(const Rational& other) const;

    // COMPARISON

    bool operator<(int64_t other_) const;
    bool operator>(int64_t other_) const;
    bool operator==(int64_t other_) const;
    bool operator!=(int64_t other_) const;
    bool operator<=(int64_t other_) const;
    bool operator>=(int64_t other_) const;
    friend bool operator<(int64_t first, const Rational& second);
    friend bool operator>(int64_t first, const Rational& second);
    friend bool operator==(int64_t first, const Rational& second);
    friend bool operator!=(int64_t first, const Rational& second);
    friend bool operator<=(int64_t first, const Rational& second);
    friend bool operator>=(int64_t first, const Rational& second);

    // CIN ~ COUT

    friend std::istream& operator>>(std::istream& in, Rational& rational);
    friend std::ostream& operator<<(std::ostream& out, const Rational& rational);
};

#endif  // RATIONAL_RATIONAL_H