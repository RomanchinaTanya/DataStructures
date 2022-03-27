#include "rational.h"
#include <cstring>

#define BASE_SIZE 1000

Rational::Rational() {
    numerator = 0;
    denominator = 1;
}

Rational::Rational(int64_t number) {  // NOLINT
    numerator = number;
    denominator = 1;
}

Rational::Rational(int64_t new_numerator, int64_t new_denominator) {
    if (!new_denominator) {
        throw RationalDivisionByZero{};
    }
    numerator = new_numerator;
    denominator = new_denominator;
    Reduce();
}

Rational::Rational(const Rational& other) {
    *this = other;
}

Rational& Rational::operator=(const Rational& other) {  // NOLINT
    numerator = other.numerator;
    denominator = other.denominator;
    return *this;
}

// USEFUL FUNCTIONS

void Rational::Swap(Rational& other) {
    std::swap(numerator, other.numerator);
    std::swap(denominator, other.denominator);
}

int64_t Rational::GetNumerator() const {
    return numerator;
}

int64_t Rational::GetDenominator() const {
    return denominator;
}

void Rational::SetNumerator(int64_t new_numerator) {
    numerator = new_numerator;
    Reduce();
}

void Rational::SetDenominator(int64_t new_denominator) {
    if (!new_denominator) {
        throw RationalDivisionByZero();
    }
    denominator = new_denominator;
    Reduce();
}

void Rational::Reduce() {
    if (denominator < 0) {
        numerator *= -1l;
        denominator *= -1l;
    }
    int64_t new_numerator = numerator;
    if (numerator < 0) {
        new_numerator *= -1l;
    }
    int64_t gcd = std::gcd(new_numerator, denominator);
    numerator /= gcd;
    denominator /= gcd;
    if (!numerator) {
        denominator = 1;
    }
}

// BINARY ~ RATIONAL (+=, -=, *=, /=, +, -, *, /)

Rational& Rational::operator+=(const Rational& other) {
    int64_t lcm = std::lcm(denominator, other.denominator);
    int64_t first_mult = lcm / denominator;
    int64_t second_mult = lcm / other.denominator;
    numerator = numerator * first_mult + other.numerator * second_mult;
    denominator = lcm;
    Reduce();
    return *this;
}

Rational& Rational::operator-=(const Rational& other) {
    int64_t lcm = std::lcm(denominator, other.denominator);
    int64_t first_mult = lcm / denominator;
    int64_t second_mult = lcm / other.denominator;
    numerator = numerator * first_mult - other.numerator * second_mult;
    denominator = lcm;
    Reduce();
    return *this;
}

Rational& Rational::operator*=(const Rational& other) {
    numerator *= other.numerator;
    denominator *= other.denominator;
    Reduce();
    return *this;
}

Rational& Rational::operator/=(const Rational& other) {
    if (!other.numerator) {
        throw RationalDivisionByZero{};
    }
    int64_t new_numerator = other.numerator;
    int64_t new_denominator = other.denominator;
    numerator *= new_denominator;
    denominator *= new_numerator;
    Reduce();
    return *this;
}

Rational Rational::operator+(const Rational& other) const {
    Rational result(*this);
    result += other;
    return result;
}

Rational Rational::operator-(const Rational& other) const {
    Rational result(*this);
    result -= other;
    return result;
}

Rational Rational::operator*(const Rational& other) const {
    Rational result(*this);
    result *= other;
    return result;
}

Rational Rational::operator/(const Rational& other) const {
    Rational result(*this);
    result /= other;
    return result;
}

// UNARY

Rational Rational::operator+() const {
    Rational result(*this);
    return result;
}

Rational Rational::operator-() const {
    Rational result(*this);
    result.numerator = -result.numerator;
    return result;
}

// INCREMENT ~ DECREMENT

Rational& Rational::operator++() {
    numerator += denominator;
    Reduce();
    return *this;
}

Rational& Rational::operator--() {
    numerator -= denominator;
    Reduce();
    return *this;
}

Rational Rational::operator++(int32_t) {
    Rational result(*this);
    ++(*this);
    return result;
}

Rational Rational::operator--(int32_t) {
    Rational result(*this);
    --(*this);
    return result;
}

// COMPARISON

bool Rational::operator<(const Rational& other) const {
    if (numerator < 0 && other.numerator > 0) {
        return true;
    }
    if (numerator * other.numerator >= 0) {
        int64_t lcm = std::lcm(denominator, other.denominator);
        int64_t new_numerator = numerator * (lcm / denominator);
        int64_t new_other_numerator = other.numerator * (lcm / other.denominator);
        return new_numerator < new_other_numerator;
    }
    return false;
}

bool Rational::operator>(const Rational& other) const {
    return other < *this;
}

bool Rational::operator==(const Rational& other) const {
    return !(other < *this || *this < other);
}

bool Rational::operator!=(const Rational& other) const {
    return !(*this == other);
}

bool Rational::operator<=(const Rational& other) const {
    return !(*this > other);
}

bool Rational::operator>=(const Rational& other) const {
    return !(*this < other);
}

// COMPARISON

bool Rational::operator<(int64_t other_other) const {
    Rational other(other_other);
    return *this < other;
}

bool Rational::operator>(int64_t other_other) const {
    Rational other(other_other);
    return *this > other;
}

bool Rational::operator==(int64_t other_other) const {
    Rational other(other_other);
    return *this == other;
}

bool Rational::operator!=(int64_t other_other) const {
    Rational other(other_other);
    return *this != other;
}

bool Rational::operator<=(int64_t other_other) const {
    Rational other(other_other);
    return *this <= other;
}

bool Rational::operator>=(int64_t other_other) const {
    Rational other(other_other);
    return *this >= other;
}

bool operator<(int64_t first, const Rational& second) {
    return second > first;
}

bool operator>(int64_t first, const Rational& second) {
    return second < first;
}

bool operator==(int64_t first, const Rational& second) {
    return second == first;
}

bool operator!=(int64_t first, const Rational& second) {
    return second != first;
}

bool operator<=(int64_t first, const Rational& second) {
    return second >= first;
}

bool operator>=(int64_t first, const Rational& second) {
    return second <= first;
}

// CIN ~ COUT

std::istream& operator>>(std::istream& in, Rational& rational) {
    char fraction = 0;
    rational.denominator = 1;
    in >> rational.numerator;
    if (in.peek() == '/') {
        in >> fraction >> rational.denominator;
    }
    if (!rational.denominator) {
        throw RationalDivisionByZero{};
    }
    rational.Reduce();
    return in;
}

std::ostream& operator<<(std::ostream& out, const Rational& rational) {
    out << rational.numerator;
    if (rational.denominator != 1) {
        out << '/' << rational.denominator;
    }
    return out;
}