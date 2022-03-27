#ifndef SHARED_PTR_SHARED_PTR_H
#define SHARED_PTR_SHARED_PTR_H

#ifndef MAKE_SHARED_IMPLEMENTED
#define MAKE_SHARED_IMPLEMENTED

#include <iostream>
#include <memory>

class BadWeakPtr : public std::runtime_error {
public:
    BadWeakPtr() : std::runtime_error("BadWeakPtr") {
    }
};

struct Counter {
    int64_t weak;
    int64_t shared;

    Counter() {
        weak = 0l;
        shared = 0l;
    }
};

template <class ElemT>
struct WeakPtr;

template <class ElemT>
struct SharedPtr;

template <class ElemT>
struct SharedPtr {
    ElemT* shared_ptr;
    Counter* counter;

    SharedPtr();
    explicit SharedPtr(ElemT* ptr);
    SharedPtr(std::nullptr_t);               //  NOLINT
    SharedPtr(const SharedPtr& other);       //  NOLINT
    SharedPtr(const WeakPtr<ElemT>& other);  //  NOLINT
    SharedPtr(SharedPtr<ElemT>&& other);     //  NOLINT
    ~SharedPtr();
    SharedPtr& operator=(ElemT* ptr);
    SharedPtr& operator=(const SharedPtr& other);
    SharedPtr& operator=(const WeakPtr<ElemT>& other);
    SharedPtr<ElemT>& operator=(SharedPtr<ElemT>&& other);
    void Reset(ElemT* ptr = nullptr);
    void Swap(SharedPtr& other);
    int64_t UseCount() const;
    ElemT* Get();
    ElemT& operator*();
    ElemT* operator->();
    const ElemT* Get() const;
    const ElemT& operator*() const;
    const ElemT* operator->() const;
    explicit operator bool() const;
};

template <class ElemT>
struct WeakPtr {
    ElemT* weak_ptr;
    Counter* counter;

    WeakPtr();
    WeakPtr(const WeakPtr<ElemT>& other);    //  NOLINT
    WeakPtr(WeakPtr<ElemT>&& other);         //  NOLINT
    WeakPtr(const SharedPtr<ElemT>& other);  //  NOLINT
    ~WeakPtr();
    WeakPtr& operator=(const WeakPtr<ElemT>& other);
    WeakPtr& operator=(WeakPtr<ElemT>&& other);
    WeakPtr& operator=(const SharedPtr<ElemT>& other);
    SharedPtr<ElemT> Lock();
    bool Expired() const;
    void Reset(ElemT* ptr = nullptr);
    void Swap(WeakPtr& other);
    int64_t UseCount();
};

////////////////////////////////////////

template <typename ElemT>
SharedPtr<ElemT>::SharedPtr() {
    shared_ptr = nullptr;
    counter = nullptr;
}

template <class ElemT>
SharedPtr<ElemT>::SharedPtr(ElemT* ptr) : SharedPtr() {
    *this = ptr;
}

template <class ElemT>
SharedPtr<ElemT>::SharedPtr(std::nullptr_t) : SharedPtr() {
    *this = nullptr;
}

template <class ElemT>
SharedPtr<ElemT>::SharedPtr(const SharedPtr<ElemT>& other) : SharedPtr() {
    *this = other;
}

template <class ElemT>
SharedPtr<ElemT>::SharedPtr(const WeakPtr<ElemT>& other) : SharedPtr() {
    *this = other;
}

template <class ElemT>
SharedPtr<ElemT>::SharedPtr(SharedPtr<ElemT>&& other) : SharedPtr() {
    *this = std::move(other);
}

template <class ElemT>
SharedPtr<ElemT>::~SharedPtr() {
    if (counter) {
        --(counter->shared);
    }
    if (shared_ptr && counter && !(counter->shared)) {
        delete shared_ptr;
        shared_ptr = nullptr;
        if (!(counter->weak)) {
            delete counter;
            counter = nullptr;
        }
    }
}

template <class ElemT>
SharedPtr<ElemT>& SharedPtr<ElemT>::operator=(ElemT* ptr) {
    if (shared_ptr) {
        (*this).~SharedPtr();
    }
    shared_ptr = ptr;
    counter = nullptr;
    if (ptr) {
        counter = new Counter();
        ++(counter->shared);
    }
    return *this;
}

template <class ElemT>
SharedPtr<ElemT>& SharedPtr<ElemT>::operator=(const SharedPtr<ElemT>& other) {
    if (shared_ptr != other.shared_ptr) {
        if (shared_ptr) {
            (*this).~SharedPtr();
        }
        shared_ptr = other.shared_ptr;
        counter = other.counter;
        if (counter) {
            ++(counter->shared);
        }
    }
    return *this;
}

template <class ElemT>
SharedPtr<ElemT>& SharedPtr<ElemT>::operator=(const WeakPtr<ElemT>& other) {
    if (other.Expired()) {
        throw BadWeakPtr{};
    }
    shared_ptr = other.weak_ptr;
    counter = other.counter;
    if (counter) {
        ++(counter->shared);
    }
    return *this;
}

template <class ElemT>
SharedPtr<ElemT>& SharedPtr<ElemT>::operator=(SharedPtr<ElemT>&& other) {
    if (shared_ptr) {
        (*this).~SharedPtr();
    }
    shared_ptr = std::move(other.shared_ptr);
    counter = std::move(other.counter);
    other.shared_ptr = nullptr;
    other.counter = nullptr;
    return *this;
}

template <class ElemT>
void SharedPtr<ElemT>::Reset(ElemT* ptr) {
    *this = ptr;
}

template <class ElemT>
void SharedPtr<ElemT>::Swap(SharedPtr& other) {
    std::swap(shared_ptr, other.shared_ptr);
    std::swap(counter, other.counter);
}

template <class ElemT>
const ElemT* SharedPtr<ElemT>::Get() const {
    return shared_ptr;
}

template <class ElemT>
ElemT* SharedPtr<ElemT>::Get() {
    return shared_ptr;
}

template <class ElemT>
int64_t SharedPtr<ElemT>::UseCount() const {
    if (!counter) {
        return 0l;
    }
    return counter->shared;
}

template <class ElemT>
const ElemT& SharedPtr<ElemT>::operator*() const {
    return *shared_ptr;
}

template <class ElemT>
ElemT& SharedPtr<ElemT>::operator*() {
    return *shared_ptr;
}

template <class ElemT>
const ElemT* SharedPtr<ElemT>::operator->() const {
    return shared_ptr;
}

template <class ElemT>
ElemT* SharedPtr<ElemT>::operator->() {
    return shared_ptr;
}

template <class ElemT>
SharedPtr<ElemT>::operator bool() const {
    return shared_ptr != nullptr;
}

////////////////////////////////////////

template <class ElemT>
WeakPtr<ElemT>::WeakPtr() {
    weak_ptr = nullptr;
    counter = nullptr;
}

template <class ElemT>
WeakPtr<ElemT>::WeakPtr(const WeakPtr<ElemT>& other) : WeakPtr() {
    *this = other;
}

template <class ElemT>
WeakPtr<ElemT>::WeakPtr(WeakPtr<ElemT>&& other) : WeakPtr() {
    *this = other;
}

template <class ElemT>
WeakPtr<ElemT>::WeakPtr(const SharedPtr<ElemT>& other) : WeakPtr() {
    *this = other;
}

template <class ElemT>
WeakPtr<ElemT>::~WeakPtr() {
    if (counter) {
        --(counter->weak);
        if (!(counter->weak) && !(counter->shared)) {
            delete counter;
            counter = nullptr;
        }
    }
}

template <class ElemT>
WeakPtr<ElemT>& WeakPtr<ElemT>::operator=(const WeakPtr<ElemT>& other) {
    weak_ptr = other.weak_ptr;
    if (weak_ptr) {
        counter = other.counter;
        ++(counter->weak);
    }
    return *this;
}

template <class ElemT>
WeakPtr<ElemT>& WeakPtr<ElemT>::operator=(WeakPtr<ElemT>&& other) {
    weak_ptr = other.weak_ptr;
    counter = other.counter;
    other.weak_ptr = nullptr;
    other.counter = nullptr;
    return *this;
}

template <class ElemT>
WeakPtr<ElemT>& WeakPtr<ElemT>::operator=(const SharedPtr<ElemT>& other) {
    weak_ptr = other.shared_ptr;
    counter = other.counter;
    if (counter) {
        ++(counter->weak);
    }
    return *this;
}

template <class ElemT>
void WeakPtr<ElemT>::Swap(WeakPtr& other) {
    std::swap(weak_ptr, other.weak_ptr);
    std::swap(counter, other.counter);
}

template <class ElemT>
void WeakPtr<ElemT>::Reset(ElemT* ptr) {
    if (weak_ptr) {
        (*this).~WeakPtr();
    }
    weak_ptr = ptr;
    counter = nullptr;
    if (ptr) {
        counter = new Counter();
        ++(counter->weak);
    }
}

template <class ElemT>
int64_t WeakPtr<ElemT>::UseCount() {
    if (!counter) {
        return 0l;
    }
    return counter->shared;
}

template <class ElemT>
bool WeakPtr<ElemT>::Expired() const {
    return (!counter || counter->shared == 0l);
}

template <class ElemT>
SharedPtr<ElemT> WeakPtr<ElemT>::Lock() {
    SharedPtr<ElemT> ptr;
    if (!Expired()) {
        ptr = *this;
    }
    return ptr;
}

template <class ElemT, class... Args>
SharedPtr<ElemT> MakeShared(Args&&... args) {
    return SharedPtr<ElemT>(new ElemT(std::forward<Args>(args)...));
}

#endif  // MAKE_SHARED_IMPLEMENTED

#endif  //  SHARED_PTR_SHARED_PTR_H