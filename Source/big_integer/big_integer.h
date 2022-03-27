#ifndef BIG_INTEGER_BIG_INTEGER_H
#define BIG_INTEGER_BIG_INTEGER_H
#ifndef BIG_INTEGER_DIVISION_IMPLEMENTED
#define BIG_INTEGER_DIVISION_IMPLEMENTED
#define MAX_SIZE 40000
#define MODULE 10000

#include <iterator>
#include <stdexcept>
#include <cstring>
#include <iostream>

class BigIntegerOverflow : public std::runtime_error {
public:
    BigIntegerOverflow() : std::runtime_error("BigIntegerOverflow") {
    }
};

class BigIntegerDivisionByZero : public std::runtime_error {
public:
    BigIntegerDivisionByZero() : std::runtime_error("BigIntegerDivisionByZero") {
    }
};

template <class ElemT>
ElemT Max(ElemT first, ElemT second) {
    return first > second ? first : second;
}

template <class ElemT>
ElemT Min(ElemT first, ElemT second) {
    return first < second ? first : second;
}

template <class ElemT>
bool More(ElemT first, ElemT second) {
    return first > second;
}

template <class ElemT>
bool Less(ElemT first, ElemT second) {
    return first < second;
}

template <int32_t NDigits = MAX_SIZE>
class BigInteger {
public:
    bool sign;
    int32_t size;
    int32_t* number;

    //  CONSTRUCT ~ DESTRUCT

    BigInteger() {
        size = 0;
        sign = false;
        number = new int32_t[MAX_SIZE + 1]{};
    }

    BigInteger<NDigits>(int64_t new_number) {  //  NOLINT
        number = new int32_t[MAX_SIZE + 1]{};
        *this = new_number;
        if (size >= NDigits) {
            throw BigIntegerOverflow{};
        }
    }

    BigInteger<NDigits>(int32_t new_number) {  //  NOLINT
        number = new int32_t[MAX_SIZE + 1]{};
        *this = new_number;
        if (size >= NDigits) {
            throw BigIntegerOverflow{};
        }
    }

    BigInteger<NDigits>& ToBigInteger(const char* new_number) {
        auto* temp = new int32_t[MAX_SIZE + 1]{};
        int32_t step = 0;
        if (new_number[0] == '-') {
            sign = true;
            ++step;
        }
        if (new_number[0] == '+') {
            ++step;
        }
        size = strlen(new_number);
        for (int32_t i = 0; i < size - 1; ++i) {
            temp[i] = new_number[size - i - 1] - '0';
        }
        if (step) {
            --size;
        } else {
            temp[size - 1] = new_number[0] - '0';
        }
        int32_t i = 0;
        int32_t j = 0;
        while (i < size) {
            int32_t module = 1;
            int32_t k = i + 4;
            while (i < k && i < size) {
                number[j] += temp[i] * module;
                module *= 10;
                i++;
            }
            j++;
        }
        size = j;
        delete[] temp;
        return *this;
    }

    BigInteger<NDigits>(const char* new_number) {  //  NOLINT
        size = 0;
        sign = false;
        number = new int32_t[MAX_SIZE + 1]{};
        ToBigInteger(new_number);
        if (size >= NDigits) {
            throw BigIntegerOverflow{};
        }
    }

    BigInteger<NDigits>& operator=(const BigInteger& other) {
        size = other.size;
        sign = other.sign;
        memcpy(number, other.number, size * sizeof(int32_t));
        return *this;
    }

    template <class ElemT>
    BigInteger<NDigits>& operator=(ElemT new_number) {
        size = 0;
        sign = false;
        if (new_number < 0) {
            sign = true;
            new_number = -new_number;
        }
        if (!new_number) {
            size = 1;
            number[0] = 0;
        }
        while (new_number) {
            number[size++] = static_cast<int32_t>(new_number % MODULE);
            new_number /= MODULE;
        }
        if (size > NDigits) {
            throw BigIntegerOverflow{};
        }
        return *this;
    }

    BigInteger<NDigits>(const BigInteger& other) {
        number = new int32_t[MAX_SIZE + 1]{};
        *this = other;
    }

    ~BigInteger() {
        delete[] number;
    }

    //  USEFUL FUNCTIONS

    void Swap(BigInteger& other) {
        std::swap(sign, other.sign);
        std::swap(size, other.size);
        std::swap(number, other.number);
    }

    bool IsNegative() {
        return sign;
    }

    bool Operatorbool() const {
        BigInteger<1> zero(0);
        return *this != zero;
    }

    //  UNARY

    BigInteger<NDigits> operator-() const {
        BigInteger result(*this);
        result.sign = !sign;
        return result;
    }

    BigInteger<NDigits> operator+() const {
        BigInteger result(*this);
        return result;
    }

    //  INCREMENT ~ DECREMENT

    BigInteger<NDigits>& operator--() {
        if (size == 1 && !number[0]) {
            ++(*this);
            sign = true;
        } else if (sign) {
            sign = false;
            ++(*this);
            sign = true;
        } else {
            int32_t i = 0;
            --number[0];
            while (number[i] < 0 && i < size - 1) {
                number[i + 1]--;
                number[i] += MODULE;
                ++i;
            }
            while ((size - 1) && !number[size - 1]) {
                size--;
            }
        }
        return *this;
    }

    BigInteger<NDigits>& operator++() {
        if (size == 1 && number[0] == 1 && sign) {
            sign = false;
            --(*this);
        } else if (sign) {
            sign = false;
            --(*this);
            sign = true;
        } else {
            int32_t i = 0;
            ++number[0];
            ++size;
            while (number[i] > MODULE - 1 && i < size - 1) {
                number[i + 1] += number[i] / MODULE;
                number[i] %= MODULE;
                ++i;
            }
            while ((size - 1) && !number[size - 1]) {
                --size;
            }
        }
        return *this;
    }

    BigInteger<NDigits> operator--(int32_t) {
        BigInteger copy(*this);
        --(*this);
        return copy;
    }

    BigInteger<NDigits> operator++(int32_t) {
        BigInteger copy(*this);
        ++(*this);
        return copy;
    }

    //  BINARY ~ BIGINTEGER (-=, +=, *=, /=, %=)

    BigInteger<NDigits>& operator-=(const BigInteger& other) {
        if (sign != other.sign) {
            sign = !sign;
            *this += other;
            sign = !sign;
        } else if (!sign && !other.sign) {
            if (!(Compare(other, Less))) {
                for (int32_t i = 0; i < other.size; ++i) {
                    while (number[i] < other.number[i]) {
                        number[i + 1]--;
                        number[i] += MODULE;
                    }
                    number[i] -= other.number[i];
                }
                while ((size - 1) && number[size - 1] == 0) {
                    --size;
                }
            } else {
                sign = true;
                *this += other;
                sign = true;
            }
        } else {
            if (Compare(other, More)) {
                sign = false;
                *this += other;
                sign = true;
            } else {
                sign = false;
                *this += other;
                sign = false;
            }
        }
        return *this;
    }

    BigInteger<NDigits>& operator+=(const BigInteger& other) {
        if (sign == other.sign) {  //  FIRST + SECOND : STATIC SIGN
            size = Max(size, other.size) + 1;
            for (int32_t i = 0; i < other.size; ++i) {
                number[i] += other.number[i];
                number[i + 1] += number[i] / MODULE;
                number[i] %= MODULE;
            }
            for (int32_t i = other.size; i < size; ++i) {
                number[i + 1] += number[i] / MODULE;
                number[i] %= MODULE;
            }
            while ((size - 1) && number[size - 1] == 0) {
                --size;
            }
        } else if (!sign && other.sign) {  //  FIRST - SECOND
            if (!(Compare(other, Less))) {
                for (int32_t i = 0; i < size; ++i) {
                    while (number[i] < other.number[i]) {
                        number[i + 1]--;
                        number[i] += MODULE;
                    }
                    number[i] -= other.number[i];
                }
                while ((size - 1) && number[size - 1] == 0) {
                    --size;
                }
            } else {
                BigInteger copy_other(other);
                copy_other.sign = false;
                sign = true;
                copy_other += *this;
                *this = copy_other;
                sign = true;
            }
        } else {  //  - FIRST + SECOND
            BigInteger copy_other(other);
            if (Compare(other, More)) {
                copy_other.sign = true;
                sign = false;
                *this += copy_other;
                sign = true;
            } else {
                copy_other += *this;
                *this = copy_other;
                sign = false;
            }
        }
        if (size * 3 >= NDigits) {
            throw BigIntegerOverflow{};
        }
        return *this;
    }

    BigInteger<NDigits>& operator*=(const BigInteger& other) {
        BigInteger result;
        result.size = size + other.size;
        for (int32_t i = 0; i < other.size; ++i) {
            for (int32_t j = 0; j < size; ++j) {
                result.number[i + j] += number[j] * other.number[i];
                result.number[i + j + 1] += result.number[i + j] / MODULE;
                result.number[i + j] %= MODULE;
            }
        }
        while ((result.size - 1) && result.number[result.size - 1] == 0) {
            --result.size;
        }
        result.sign = sign ^ other.sign;
        if (result.size == 1 && !result.number[0]) {
            result.sign = false;
        }
        *this = result;
        if (size * 3 >= NDigits) {
            throw BigIntegerOverflow{};
        }
        return *this;
    }

    BigInteger<NDigits>& operator/=(const BigInteger& other) {
        if (other.size == 1 && !(other.number[0])) {
            throw BigIntegerDivisionByZero{};
        }
        if (Compare(other, Less)) {
            *this = BigInteger(0);
        } else if (other.size == 1 && other.number[0] == 1) {
            sign = sign ^ other.sign;
        } else {
            if (!sign && !other.sign) {
                bool flag1 = false;
                bool flag2 = false;
                bool flag3 = false;
                bool flag4 = false;
                BigInteger left;
                BigInteger right;
                BigInteger middle;
                BigInteger temp1;
                BigInteger temp2;
                left.size = 1;
                left.number[0] = 1;
                right.size = size - other.size + 2;
                right.number[size - other.size + 1] = 1;
                while (true) {
                    middle = right + left;
                    middle.DivideBy(2);
                    temp1 = middle * other;
                    flag1 = temp1 > *this;
                    flag2 = temp1 < *this;
                    temp2 = *this - temp1;
                    flag3 = temp2 > other;
                    flag4 = temp2 < other;
                    if (flag1) {
                        right = middle;
                    } else if (flag2) {
                        if (flag3) {
                            left = middle;
                        } else if (flag4) {
                            break;
                        } else {
                            ++middle;
                            break;
                        }
                    } else {
                        break;
                    }
                }
                *this = middle;
            } else if (sign && other.sign) {
                sign = false;
                *this /= -other;
            } else if (!sign && other.sign) {
                *this /= -other;
                sign = true;
            } else {
                sign = false;
                *this /= other;
                sign = true;
            }
        }
        if (size == 1 && !number[0]) {
            sign = false;
        }
        return *this;
    }

    BigInteger<NDigits>& operator%=(const BigInteger& other) {
        BigInteger remainder(*this);
        remainder /= other;
        remainder *= other;
        *this -= remainder;
        return *this;
    }

    //  BINARY ~ BIGINTEGER (-, +, *, /, %)

    friend BigInteger operator-(const BigInteger& first, const BigInteger& second) {
        BigInteger result(first);
        result -= second;
        return result;
    }

    friend BigInteger operator+(const BigInteger& first, const BigInteger& second) {
        BigInteger result(first);
        result += second;
        if (result.size * 3 >= NDigits) {
            throw BigIntegerOverflow{};
        }
        return result;
    }

    friend BigInteger operator*(const BigInteger& first, const BigInteger& second) {
        BigInteger result(first);
        result *= second;
        if (result.size * 3 >= NDigits) {
            throw BigIntegerOverflow{};
        }
        return result;
    }

    friend BigInteger operator/(const BigInteger& first, const BigInteger& second) {
        BigInteger result(first);
        result /= second;
        return result;
    }

    friend BigInteger operator%(const BigInteger& first, const BigInteger& second) {
        BigInteger result(first);
        result %= second;
        return result;
    }

    //  BINARY ~ INT32_T (-=, +=, *=, /=, %=)

    BigInteger<NDigits>& DivideBy(const int32_t& divisor) {
        int32_t remainder = 0;
        int32_t current = 0;
        for (int32_t i = size - 1; i >= 0; --i) {
            current = remainder * MODULE + number[i];
            number[i] = current / divisor;
            remainder = current % divisor;
        }
        while ((size - 1) && number[size - 1] == 0) {
            --size;
        }
        return *this;
    }

    BigInteger<NDigits>& operator-=(const int32_t& new_other) {
        BigInteger other(new_other);
        *this -= other;
        return *this;
    }

    BigInteger<NDigits>& operator+=(const int32_t& new_other) {
        BigInteger other(new_other);
        *this += other;
        return *this;
    }

    BigInteger<NDigits>& operator*=(const int32_t& new_other) {
        BigInteger other(new_other);
        *this *= other;
        if (size * 4 >= NDigits) {
            throw BigIntegerOverflow{};
        }
        return *this;
    }

    BigInteger<NDigits>& operator/=(const int32_t& new_other) {
        BigInteger other(new_other);
        *this /= other;
        return *this;
    }

    BigInteger<NDigits>& operator%=(const int32_t& new_other) {
        BigInteger other(new_other);
        *this %= other;
        return *this;
    }

    //  BINARY ~ INT32_T (-, +, *, /, %)

    friend BigInteger operator-(const BigInteger& first, const int32_t& second) {
        BigInteger result(first);
        result -= second;
        return result;
    }

    friend BigInteger operator+(const BigInteger& first, const int32_t& second) {
        BigInteger result(first);
        result += second;
        return result;
    }

    friend BigInteger operator*(const BigInteger& first, const int32_t& second) {
        BigInteger result(first);
        result *= second;
        return result;
    }

    friend BigInteger operator/(const BigInteger& first, const int32_t& second) {
        BigInteger result(first);
        result /= second;
        return result;
    }

    friend BigInteger operator%(const BigInteger& first, const int32_t& second) {
        BigInteger result(first);
        result %= second;
        return result;
    }

    //  COMPARING

    bool Compare(const BigInteger& other, bool (*comparator)(int32_t, int32_t)) const {
        bool flag = false;
        if (comparator(size, other.size)) {
            flag = true;
        } else {
            int32_t i = size - 1;
            while (i > 0 && number[i] == other.number[i]) {
                --i;
            }
            if (comparator(number[i], other.number[i])) {
                flag = true;
            }
        }
        return flag;
    }

    bool operator<(const BigInteger& other) const {
        bool flag = false;
        if (sign && !other.sign) {
            flag = true;
        } else if (sign && other.sign) {
            flag = Compare(other, More);
        } else if (!sign && !other.sign) {
            flag = Compare(other, Less);
        }
        return flag;
    }

    bool operator>(const BigInteger& other) const {
        bool flag = false;
        if (!sign && other.sign) {
            flag = true;
        } else if (!sign && !other.sign) {
            flag = Compare(other, More);
        } else if (sign && other.sign) {
            flag = Compare(other, Less);
        }
        return flag;
    }

    bool operator==(const BigInteger& other) const {
        return (!(*this < other || *this > other) && sign == other.sign);
    }

    bool operator!=(const BigInteger& other) const {
        return !(*this == other);
    }

    bool operator<=(const BigInteger& other) const {
        return !(*this > other);
    }

    bool operator>=(const BigInteger& other) const {
        return !(*this < other);
    }

    //  OSTREAM ~ ISTREAM

    friend std::istream& operator>>(std::istream& in, BigInteger& integer) {
        char* new_number = new char[MAX_SIZE]{};
        in >> new_number;
        integer.ToBigInteger(new_number);
        delete[] new_number;
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const BigInteger& integer) {
        if (!integer.size) {
            out << "0";
        } else {
            if (integer.sign) {
                out << "-";
            }
            out << integer.number[integer.size - 1];
            for (int32_t i = integer.size - 2; i >= 0; --i) {
                if (!integer.number[i]) {
                    out << "0000";
                } else if (integer.number[i] < 10) {
                    out << "000" << integer.number[i];
                } else if (integer.number[i] < 100) {
                    out << "00" << integer.number[i];
                } else if (integer.number[i] < 1000) {
                    out << "0" << integer.number[i];
                } else {
                    out << integer.number[i];
                }
            }
        }
        return out;
    }
};

#endif  //  BIG_INTEGER_BIG_INTEGER_H
#endif  //  BIG_INTEGER_DIVISION_IMPLEMENTED