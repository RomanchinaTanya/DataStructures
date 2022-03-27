#ifndef UNIQUE_PTR_UNIQUE_PTR_H
#define UNIQUE_PTR_UNIQUE_PTR_H

#ifndef MAKE_UNIQUE_IMPLEMENTED
#define MAKE_UNIQUE_IMPLEMENTED

#include <iostream>
#include <memory>

template <class ElemT>
class UniquePtr {
public:
    ElemT* unique_ptr;

    UniquePtr() {
        unique_ptr = nullptr;
    }

    explicit UniquePtr(ElemT* ptr) {
        unique_ptr = ptr;
    }

    UniquePtr& operator=(const UniquePtr& ptr) = delete;

    UniquePtr(const UniquePtr& ptr) = delete;

    UniquePtr& operator=(UniquePtr&& ptr) {
        Clear();
        unique_ptr = ptr.unique_ptr;
        ptr.unique_ptr = nullptr;
        return *this;
    }

    UniquePtr(UniquePtr&& ptr) {
        unique_ptr = ptr.unique_ptr;
        ptr.unique_ptr = nullptr;
    }

    ~UniquePtr() {
        Clear();
    }

    ElemT* Release() {
        ElemT* ptr = unique_ptr;
        unique_ptr = nullptr;
        return ptr;
    }

    void Reset(ElemT* ptr = nullptr) {
        Clear();
        unique_ptr = ptr;
    }

    void Swap(UniquePtr& ptr) {
        std::swap(ptr.unique_ptr, unique_ptr);
    }

    ElemT* Get() {
        return unique_ptr;
    }

    const ElemT* operator->() const {
        return unique_ptr;
    }

    ElemT* operator->() {
        return unique_ptr;
    }

    const ElemT& operator*() const {
        return *unique_ptr;
    }

    ElemT& operator*() {
        return *unique_ptr;
    }

    void Clear() {
        if (unique_ptr) {
            delete unique_ptr;
        }
        unique_ptr = nullptr;
    }

    explicit operator bool() const {
        return unique_ptr != nullptr;
    }
};

template <class ElemT, class... Args>
UniquePtr<ElemT> MakeUnique(Args&&... args) {
    return UniquePtr<ElemT>(new ElemT(std::forward<Args>(args)...));
}

#endif  // MAKE_UNIQUE_IMPLEMENTED

#endif  // UNIQUE_PTR_UNIQUE_PTR_H