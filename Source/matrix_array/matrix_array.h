#ifndef MATRIX_ARRAY_MATRIX_ARRAY_H
#define MATRIX_ARRAY_MATRIX_ARRAY_H
#ifndef MATRIX_ARRAY_SQUARE_MATRIX_IMPLEMENTED
#define MATRIX_ARRAY_SQUARE_MATRIX_IMPLEMENTED

#include <util/constants.h>
#include <iostream>
#include "rational.h"

class MatrixArrayIsDegenerateError : public std::runtime_error {
public:
    MatrixArrayIsDegenerateError() : std::runtime_error("MatrixArrayIsDegenerateError") {
    }
};

class MatrixArrayOutOfRange : public std::out_of_range {
public:
    MatrixArrayOutOfRange() : std::out_of_range("MatrixArrayOutOfRange") {
    }
};

template <class ElemT, size_t ElemM, size_t ElemN>
class MatrixArray {
public:
    ElemT array[ElemM][ElemN]{};

    size_t RowsNumber() const {
        return ElemM;
    }

    size_t ColumnsNumber() const {
        return ElemN;
    }

    bool CheckIfSquare() {
        static_assert(ElemM == ElemN, "ElemM != ElemN: matrix is not square!");
        return true;
    }

    const ElemT& operator()(size_t row, size_t column) const {
        return array[row][column];
    }

    ElemT& operator()(size_t row, size_t column) {
        return array[row][column];
    }

    const ElemT& At(size_t row, size_t column) const {
        if (row >= ElemM || column >= ElemN) {
            throw MatrixArrayOutOfRange{};
        }
        return array[row][column];
    }

    ElemT& At(size_t row, size_t column) {
        if (row >= ElemM || column >= ElemN) {
            throw MatrixArrayOutOfRange{};
        }
        return array[row][column];
    }

    void Transpose() {
        static_assert(ElemM == ElemN, "ElemM != ElemN: matrix is not square!");
        for (size_t i = 0; i < ElemM; i++) {
            for (size_t j = i + 1; j < ElemN; j++) {
                std::swap(array[i][j], array[j][i]);
            }
        }
    }

    MatrixArray<ElemT, ElemN, ElemM> GetTransposed() const {
        MatrixArray<ElemT, ElemN, ElemM> copy;
        for (size_t i = 0; i < ElemM; i++) {
            for (size_t j = 0; j < ElemN; j++) {
                copy.array[j][i] = array[i][j];
            }
        }
        return copy;
    }

    ElemT Trace() const {
        static_assert(ElemM == ElemN, "ElemM != ElemN: matrix is not square!");
        auto trace = static_cast<ElemT>(0);
        for (size_t i = 0; i < ElemM; i++) {
            trace += array[i][i];
        }
        return trace;
    }

    Rational Determinant() const {
        static_assert(ElemM == ElemN, "ElemM != ElemN: matrix is not square!");
        MatrixArray<Rational, ElemM, ElemN> copy;
        for (size_t i = 0; i < ElemM; i++) {
            for (size_t j = 0; j < ElemN; j++) {
                copy.array[i][j] = static_cast<Rational>(array[i][j]);
            }
        }
        Rational determinant = static_cast<ElemT>(1);
        for (size_t i = 0; i < ElemM; i++) {
            determinant *= copy.array[i][i];
            if (copy.array[i][i] == static_cast<ElemT>(0)) {
                break;
            }
            for (size_t j = i + 1; j < ElemM; j++) {
                if (copy.array[j][i] == static_cast<ElemT>(0)) {
                    break;
                }
                Rational mult = copy.array[j][i] / copy.array[i][i];
                for (size_t k = i; k < ElemM; k++) {
                    copy.array[j][k] -= mult * copy.array[i][k];
                }
            }
        }
        return determinant;
    }

    void Inverse() {
        static_assert(ElemM == ElemN, "ElemM != ElemN: matrix is not square!");
        if (Determinant() == static_cast<ElemT>(0)) {
            throw MatrixArrayIsDegenerateError{};
        }
        MatrixArray<ElemT, ElemM, ElemN> one;
        for (size_t i = 0; i < ElemM; i++) {
            one.array[i][i] = static_cast<ElemT>(1);
        }
        for (size_t i = 0; i < ElemM; i++) {
            ElemT divisor = array[i][i];
            for (size_t j = 0; j < i; j++) {
                one.array[i][j] /= divisor;
            }
            for (size_t j = i; j < ElemM; j++) {
                array[i][j] /= divisor;
                one.array[i][j] /= divisor;
            }
            for (size_t j = 0; j < ElemM; j++) {
                if (i != j && array[j][i] != static_cast<ElemT>(0)) {
                    ElemT mult = array[j][i];
                    for (size_t k = 0; k < i; k++) {
                        one.array[j][k] -= mult * one.array[i][k];
                    }
                    for (size_t k = i; k < ElemM; k++) {
                        array[j][k] -= mult * array[i][k];
                        one.array[j][k] -= mult * one.array[i][k];
                    }
                }
            }
        }
        *this = one;
    }

    MatrixArray<ElemT, ElemM, ElemN> GetInversed() const {
        MatrixArray copy;
        copy = *this;
        copy.Inverse();
        return copy;
    }

    template <size_t ElemP>
    friend MatrixArray<ElemT, ElemM, ElemP> SolveLinearSystem(const MatrixArray<ElemT, ElemM, ElemN>& matrix,
                                                              const MatrixArray<ElemT, ElemM, ElemP>& column) {
        if (matrix.Determinant() == static_cast<ElemT>(0)) {
            throw MatrixArrayIsDegenerateError{};
        }
        MatrixArray<ElemT, ElemM, ElemP> result;
        MatrixArray<ElemT, ElemM, ElemN> inversed;
        inversed = matrix;
        inversed.Inverse();
        result = inversed * column;
        return result;
    }

    MatrixArray& operator+=(const MatrixArray<ElemT, ElemM, ElemN>& other) {
        for (size_t i = 0; i < ElemM; i++) {
            for (size_t j = 0; j < ElemN; j++) {
                array[i][j] += other.array[i][j];
            }
        }
        return *this;
    }

    MatrixArray& operator-=(const MatrixArray<ElemT, ElemM, ElemN>& other) {
        for (size_t i = 0; i < ElemM; i++) {
            for (size_t j = 0; j < ElemN; j++) {
                array[i][j] -= other.array[i][j];
            }
        }
        return *this;
    }

    MatrixArray& operator*=(const MatrixArray<ElemT, ElemN, ElemN>& other) {
        *this = *this * other;
        return *this;
    }

    friend MatrixArray operator+(const MatrixArray<ElemT, ElemM, ElemN>& first,
                                 const MatrixArray<ElemT, ElemM, ElemN>& second) {
        MatrixArray result(first);
        result += second;
        return result;
    }

    friend MatrixArray operator-(const MatrixArray<ElemT, ElemM, ElemN>& first,
                                 const MatrixArray<ElemT, ElemM, ElemN>& second) {
        MatrixArray result(first);
        result -= second;
        return result;
    }

    template <size_t ElemP>
    MatrixArray<ElemT, ElemM, ElemP> operator*(const MatrixArray<ElemT, ElemN, ElemP>& second) const {
        MatrixArray<ElemT, ElemM, ElemP> result;
        for (size_t i = 0; i < ElemM; i++) {
            for (size_t j = 0; j < ElemP; j++) {
                for (size_t k = 0; k < ElemN; k++) {
                    result.array[i][j] += array[i][k] * second.array[k][j];
                }
            }
        }
        return result;
    }

    // ///////////////////////////////////////////////////////////////////////////////////

    MatrixArray& operator+=(int64_t elem) {
        for (size_t i = 0; i < ElemM; i++) {
            for (size_t j = 0; j < ElemN; j++) {
                array[i][j] += elem;
            }
        }
        return *this;
    }

    MatrixArray& operator-=(int64_t elem) {
        for (size_t i = 0; i < ElemM; i++) {
            for (size_t j = 0; j < ElemN; j++) {
                array[i][j] -= elem;
            }
        }
        return *this;
    }

    MatrixArray& operator*=(int64_t elem) {
        for (size_t i = 0; i < ElemM; i++) {
            for (size_t j = 0; j < ElemN; j++) {
                array[i][j] *= elem;
            }
        }
        return *this;
    }

    MatrixArray& operator/=(int64_t elem) {
        for (size_t i = 0; i < ElemM; i++) {
            for (size_t j = 0; j < ElemN; j++) {
                array[i][j] /= elem;
            }
        }
        return *this;
    }

    friend MatrixArray& operator+=(int64_t elem, MatrixArray& matrix) {
        matrix += elem;
        return matrix;
    }

    friend MatrixArray& operator-=(int64_t elem, MatrixArray& matrix) {
        matrix -= elem;
        return matrix;
    }

    friend MatrixArray& operator*=(int64_t elem, MatrixArray& matrix) {
        matrix *= elem;
        return matrix;
    }

    friend MatrixArray& operator/=(int64_t elem, MatrixArray& matrix) {
        matrix /= elem;
        return matrix;
    }

    // ///////////////////////////////////////////////////////////////////////////////////

    friend MatrixArray operator+(const MatrixArray& matrix, int64_t elem) {
        MatrixArray result(matrix);
        result += elem;
        return result;
    }

    friend MatrixArray operator-(const MatrixArray& matrix, int64_t elem) {
        MatrixArray result(matrix);
        result -= elem;
        return result;
    }

    friend MatrixArray operator*(const MatrixArray& matrix, int64_t elem) {
        MatrixArray result(matrix);
        result *= elem;
        return result;
    }

    friend MatrixArray operator/(const MatrixArray& matrix, int64_t elem) {
        MatrixArray result(matrix);
        result /= elem;
        return result;
    }

    friend MatrixArray operator+(int64_t elem, const MatrixArray& matrix) {
        return matrix + elem;
    }

    friend MatrixArray operator*(int64_t elem, const MatrixArray& matrix) {
        return matrix * elem;
    }

    // ////////////////////////////////////////////////////////////////////////////////////

    bool operator==(const MatrixArray<ElemT, ElemM, ElemN>& other) const {
        for (size_t i = 0; i < ElemM; i++) {
            for (size_t j = 0; j < ElemN; j++) {
                if (array[i][j] != other.array[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const MatrixArray<ElemT, ElemM, ElemN>& other) const {
        return !(*this == other);
    }

    // ///////////////////////////////////////////////////////////////////////////////////

    friend std::istream& operator>>(std::istream& in, MatrixArray<ElemT, ElemM, ElemN>& matrix) {
        for (size_t i = 0; i < ElemM; i++) {
            for (size_t j = 0; j < ElemN; j++) {
                in >> matrix.array[i][j];
            }
        }
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const MatrixArray<ElemT, ElemM, ElemN>& matrix) {
        for (size_t i = 0; i < ElemM; i++) {
            for (size_t j = 0; j < ElemN - 1; j++) {
                out << matrix.array[i][j] << " ";
            }
            out << matrix.array[i][ElemN - 1] << "\n";
        }
        return out;
    }
};

#endif  // MATRIX_ARRAY_MATRIX_ARRAY_H
#endif  // MATRIX_ARRAY_SQUARE_MATRIX_IMPLEMENTED
