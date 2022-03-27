#ifndef DEQUE_CIRCULAR_BUFFER_H
#define DEQUE_CIRCULAR_BUFFER_H
#define BASE_SIZE 8

#include <iterator>
#include <stdexcept>
#include <cstring>
#include "cstddef"

template <class ElemT>
class CircularBuffer {
public:
    ElemT* buffer;
    size_t front;
    size_t back;
    size_t size;
    size_t capacity;

    CircularBuffer() {
        front = -1;
        back = -1;
        size = 0;
        capacity = BASE_SIZE;
        buffer = new ElemT[capacity + 1]{};
    }

    explicit CircularBuffer(size_t count) {
        front = -1;
        back = -1;
        size = 0;
        capacity = count;
        buffer = new ElemT[count + 1]{};
    }

    CircularBuffer& operator=(const CircularBuffer& other) {
        front = other.front;
        back = other.back;
        size = other.size;
        capacity = other.capacity;
        for (size_t i = 0; i < capacity; ++i) {
            buffer[i] = other.buffer[i];
        }
        return *this;
    }

    CircularBuffer(const CircularBuffer& other) {
        buffer = new ElemT[other.capacity + 1]{};
        *this = other;
    }

    CircularBuffer& operator=(CircularBuffer&& other) {
        front = other.front;
        back = other.back;
        size = other.size;
        capacity = other.capacity;
        buffer = std::move(other.buffer);
        other.buffer = nullptr;
        return *this;
    }

    CircularBuffer(CircularBuffer&& other) : CircularBuffer() {
        *this = other;
    }

    ~CircularBuffer() {
        delete[] buffer;
    }

    // void PrintBuffer() {  //  DEBUG
    //     std::cout << "RequiredBuffer : \n";
    //     for (size_t i = 0; i < size; i++) {
    //         std::cout << (*this)[i] << " ";
    //     }
    //     std::cout << "\n";
    //     std::cout << "RealBuffer : \n";
    //     for (size_t i = 0; i < capacity; i++) {
    //         std::cout << buffer[i] << " ";
    //     }
    //     std::cout << "\n";
    // }

    ElemT& operator[](size_t index) {
        return buffer[(front + index) % capacity];
    }

    const ElemT& operator[](size_t index) const {
        return buffer[(front + index) % capacity];
    }

    ElemT& Front() {
        return buffer[front];
    }

    const ElemT& Front() const {
        return buffer[front];
    }

    ElemT& Back() {
        return buffer[back];
    }

    const ElemT& Back() const {
        return buffer[back];
    }

    bool Empty() const {
        return (size == 0);
    }

    size_t Size() const {
        return size;
    }

    size_t Capacity() const {
        return capacity;
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity) {
            ReAlloc(new_capacity);
        }
    }

    void Swap(CircularBuffer& other) {
        std::swap(size, other.size);
        std::swap(capacity, other.capacity);
        std::swap(front, other.front);
        std::swap(back, other.back);
        std::swap(buffer, other.buffer);
    }

    void ReAlloc(size_t tmp_capacity) {
        auto* tmp_buffer = new ElemT[tmp_capacity + 1]{};
        if (back > front) {
            tmp_buffer[tmp_capacity - 1] = buffer[front];
            for (size_t i = 0; i < size - 1; i++) {
                tmp_buffer[i] = buffer[i + 1];
            }
        } else if (back < front) {
            size_t count = 0;
            tmp_buffer[tmp_capacity - 1] = buffer[front];
            for (size_t i = front + 1; i < capacity; i++) {
                tmp_buffer[count++] = buffer[i];
            }
            for (size_t i = 0; i <= back; i++) {
                tmp_buffer[count++] = buffer[i];
            }
        }
        back = size - 2;
        front = tmp_capacity - 1;
        capacity = tmp_capacity;
        std::swap(buffer, tmp_buffer);
        delete[] tmp_buffer;
    }

    void PushBack(const ElemT& value) {
        if (size == capacity) {
            size_t tmp_capacity = capacity ? capacity * 2 : 1;
            ReAlloc(tmp_capacity);
        }
        if (!size) {
            buffer[capacity - 1] = value;
            front = back = capacity - 1;
        } else {
            if (back == capacity - 1) {
                back = 0;
                buffer[back] = value;
            } else {
                buffer[++back] = value;
            }
        }
        size++;
    }

    void PushFront(const ElemT& value) {
        if (size == capacity) {
            size_t tmp_capacity = capacity ? capacity * 2 : 1;
            ReAlloc(tmp_capacity);
        }
        if (!size) {
            buffer[capacity - 1] = value;
            front = back = capacity - 1;
        } else {
            if (!front) {
                front = capacity - 1;
                buffer[front] = value;
            } else {
                buffer[--front] = value;
            }
        }
        size++;
    }

    void PopBack() {
        if (!back) {
            buffer[back] = static_cast<ElemT>(0);
            back = capacity - 1;
        } else {
            buffer[back--] = static_cast<ElemT>(0);
        }
        size--;
    }

    void PopFront() {
        if (front == capacity - 1) {
            buffer[front] = static_cast<ElemT>(0);
            front = 0;
        } else {
            buffer[front++] = static_cast<ElemT>(0);
        }
        size--;
    }

    void Clear() {
        size = 0;
    }
};

#endif