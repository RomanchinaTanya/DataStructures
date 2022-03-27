#ifndef ARRAY_ARRAY_H
#define ARRAY_ARRAY_H

#include <iterator>
#include <stdexcept>

class ArrayOutOfRange : public std::out_of_range {
public:
    ArrayOutOfRange() : std::out_of_range("ArrayOutOfRange") {
    }
};

template <class ElemT, size_t ElemN>
struct Array {
    ElemT array[ElemN]{};

    Array() {
        for (size_t i = 0; i < ElemN; i++) {
            array[i] = 0;
        }
    }

    Array(std::initializer_list<ElemT> list) {
        std::copy(list.begin(), list.end(), array);
    }

    ElemT &operator[](size_t index) {
        return array[index];
    }

    const ElemT &operator[](size_t index) const {
        return array[index];
    }

    ElemT &At(size_t index) {
        if (index >= ElemN) {
            throw ArrayOutOfRange{};
        }
        return array[index];
    }

    const ElemT &At(size_t index) const {
        if (index >= ElemN) {
            throw ArrayOutOfRange{};
        }
        return array[index];
    }

    ElemT &Front() {
        return array[0];
    }

    const ElemT &Front() const {
        return array[0];
    }

    ElemT &Back() {
        return array[ElemN - 1];
    }

    const ElemT &Back() const {
        return array[ElemN - 1];
    }

    ElemT *Data() {
        return const_cast<ElemT *>(array);
    }

    const ElemT *Data() const {
        return array;
    }

    size_t Size() const {
        return ElemN;
    }

    bool Empty() const {
        bool flag = false;
        if (!ElemN) {
            flag = true;
        }
        return flag;
    }

    void Fill(const ElemT &value) {
        for (size_t i = 0; i < ElemN; i++) {
            array[i] = value;
        }
    }

    void Swap(Array<ElemT, ElemN> &other) {
        for (size_t i = 0; i < ElemN; i++) {
            std::swap(array[i], other.array[i]);
        }
    }

    bool operator<(const Array<ElemT, ElemN> &other) const {
        size_t i = 0;
        bool flag = false;
        while (i < ElemN - 1 && array[i] == other.array[i]) {
            i++;
        }
        if (array[i] < other.array[i]) {
            flag = true;
        }
        return flag;
    }

    bool operator>(const Array<ElemT, ElemN> &other) const {
        return (other < *this);
    }

    bool operator==(const Array<ElemT, ElemN> &other) const {
        return (!(*this < other || other < *this));
    }

    bool operator!=(const Array<ElemT, ElemN> &other) const {
        return (!(*this == other));
    }

    bool operator<=(const Array<ElemT, ElemN> &other) const {
        return (!(*this > other));
    }

    bool operator>=(const Array<ElemT, ElemN> &other) const {
        return (!(*this < other));
    }
};

template <class ElemT, size_t ElemN>
void swap(Array<ElemT, ElemN> &first, Array<ElemT, ElemN> &second) {  // NOLINT
    first.Swap(second);
}

#endif  // ARRAY_ARRAY_H