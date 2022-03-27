#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H
#define MAX_SIZE 1000000

#include <iterator>
#include <stdexcept>
#include <cstring>

class VectorOutOfRange : public std::out_of_range {
public:
    VectorOutOfRange() : std::out_of_range("VectorOutOfRange") {
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
class Vector {
public:
    size_t size;
    size_t capacity;
    ElemT* vector;

    Vector() {
        size = 0;
        capacity = 0;
        vector = nullptr;
    }

    explicit Vector(size_t new_size) : Vector() {
        Resize(new_size);
    }

    Vector(size_t new_size, ElemT value) : Vector() {
        Resize(new_size, value);
    }

    Vector(const Vector<ElemT>& other) {
        size = other.size;
        capacity = other.capacity;
        vector = new ElemT[capacity + 1]{};
        for (size_t i = 0; i < size; ++i) {
            vector[i] = other.vector[i];
        }
    }

    ~Vector() {
        delete[] vector;
    }

    void Print() const {
        printf("Size = %ld:\n", size);
        for (size_t i = 0; i < size; i++) {
            printf("%d ", vector[i]);
        }
        printf("\n");
    }

    void Clear() {
        size = 0;
    }

    void PushBack(ElemT value) {
        if (size == capacity) {
            capacity = (!capacity) ? 1 : capacity * 2;
            auto* new_vector = new ElemT[capacity + 1]{};
            for (size_t i = 0; i < size; ++i) {
                new_vector[i] = vector[i];
            }
            std::swap(vector, new_vector);
            if (new_vector) {
                delete[] new_vector;
            }
        }
        vector[size] = value;
        ++size;
    }

    void PopBack() {
        if (size && vector) {
            --size;
        }
    }

    void Resize(size_t new_size) {
        if (new_size < size) {
            size = new_size;
        } else {
            if (capacity < new_size) {
                capacity = new_size;
                auto* new_vector = new ElemT[capacity + 1]{};
                for (size_t i = 0; i < size; ++i) {
                    new_vector[i] = vector[i];
                }
                std::swap(vector, new_vector);
                if (new_vector) {
                    delete[] new_vector;
                }
            }
            size = new_size;
        }
    }

    void Resize(size_t new_size, ElemT value) {
        size_t old_size = size;
        Resize(new_size);
        if (new_size > old_size) {
            for (size_t i = old_size; i < new_size; ++i) {
                vector[i] = value;
            }
        }
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity) {
            capacity = new_capacity;
            auto* new_vector = new ElemT[capacity + 1]{};
            for (size_t i = 0; i < size; ++i) {
                new_vector[i] = vector[i];
            }
            std::swap(vector, new_vector);
            delete[] new_vector;
        }
    }

    Vector<ElemT>& operator=(const Vector<ElemT>& other) {
        Reserve(other.capacity);
        Resize(other.size);
        for (size_t i = 0; i < size; ++i) {
            vector[i] = other.vector[i];
        }
        return *this;
    }

    void ShrinkToFit() {
        if (!size) {
            delete[] vector;
            capacity = 0;
        }
        if (capacity > size) {
            capacity = size;
            auto* new_vector = new ElemT[capacity + 1]{};
            for (size_t i = 0; i < size; ++i) {
                new_vector[i] = vector[i];
            }
            std::swap(vector, new_vector);
            if (new_vector) {
                delete[] new_vector;
            }
        }
    }

    void Swap(Vector& other) {
        std::swap(vector, other.vector);
        std::swap(size, other.size);
        std::swap(capacity, other.capacity);
    }

    ElemT& operator[](const size_t& index) {
        return vector[index];
    }

    const ElemT& operator[](const size_t& index) const {
        return vector[index];
    }

    ElemT& At(const size_t& index) {
        if (index >= size) {
            throw VectorOutOfRange{};
        }
        return vector[index];
    }

    const ElemT& At(const size_t& index) const {
        if (index >= size) {
            throw VectorOutOfRange{};
        }
        return vector[index];
    }

    ElemT& Front() {
        return vector[0];
    }

    const ElemT& Front() const {
        return vector[0];
    }

    ElemT& Back() {
        return vector[size - 1];
    }

    const ElemT& Back() const {
        return vector[size - 1];
    }

    bool Empty() const {
        bool flag = false;
        if (!size) {
            flag = true;
        }
        return flag;
    }

    size_t Size() const {
        return size;
    }

    size_t Capacity() const {
        return capacity;
    }

    ElemT* Data() {
        return vector;
    }

    const ElemT* Data() const {
        return vector;
    }

    bool operator>(const Vector<ElemT>& other) const {
        size_t act_size = Min(size, other.size);
        for (size_t i = 0; i < act_size; ++i) {
            if (vector[i] != other.vector[i]) {
                return (vector[i] > other.vector[i]);
            }
        }
        return (size > other.size);
    }

    bool operator<(const Vector<ElemT>& other) const {
        size_t act_size = Min(size, other.size);
        for (size_t i = 0; i < act_size; ++i) {
            if (vector[i] != other.vector[i]) {
                return (vector[i] < other.vector[i]);
            }
        }
        return (size < other.size);
    }

    bool operator==(const Vector<ElemT>& other) const {
        return (!(*this > other) && !(*this < other));
    }

    bool operator!=(const Vector<ElemT>& other) const {
        return (!(*this == other));
    }

    bool operator>=(const Vector<ElemT>& other) const {
        return (!(*this < other));
    }

    bool operator<=(const Vector<ElemT>& other) const {
        return (!(*this > other));
    }
};

#endif
