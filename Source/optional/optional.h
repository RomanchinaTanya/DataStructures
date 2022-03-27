#include <iostream>
#include <optional>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <new>

#ifndef OPTIONAL_OPTIONAL_H
#define OPTIONAL_OPTIONAL_H

class BadOptionalAccess : public std::bad_optional_access {
public:
    BadOptionalAccess() : std::bad_optional_access() {
    }
};

template <class TElem>
class Optional {
public:
    std::aligned_storage_t<sizeof(TElem), alignof(TElem)> memory;
    bool alive;

    Optional();
    Optional(const Optional& other);
    Optional(Optional&& other);
    explicit Optional(const TElem& value);
    explicit Optional(TElem&& value);
    ~Optional();
    Optional& operator=(const Optional& other);
    Optional& operator=(const TElem& value);
    Optional& operator=(Optional&& other);
    Optional& operator=(TElem&& value);
    void Swap(Optional& other);
    void Reset();
    bool HasValue() const;
    explicit operator bool() const;
    const TElem& Value() const;
    const TElem& operator*() const;
    const TElem* operator->() const;
    const TElem* Ptr() const;
    TElem& Value();
    TElem& operator*();
    TElem* operator->();
    TElem ValueOr(const TElem& alternative) const;
    TElem* Ptr();
    template <class... Args>
    TElem& Emplace(Args&&... args);
};

template <class TElem>
Optional<TElem>::Optional() : alive(false) {
}

template <class TElem>
Optional<TElem>::Optional(const Optional& other) : Optional() {
    if (other.alive) {
        new (&memory) TElem(*(other.Ptr()));
        alive = true;
    }
}

template <class TElem>
Optional<TElem>::Optional(Optional&& other) : Optional() {
    if (other.alive) {
        new (&memory) TElem(std::move(*(other.Ptr())));
        alive = true;
    }
}

template <class TElem>
Optional<TElem>::Optional(const TElem& value) : Optional() {
    new (&memory) TElem(value);
    alive = true;
}

template <class TElem>
Optional<TElem>::Optional(TElem&& value) : Optional() {
    new (&memory) TElem(std::move(value));
    alive = true;
}

template <class TElem>
Optional<TElem>::~Optional() {
    Reset();
}

template <class TElem>
Optional<TElem>& Optional<TElem>::operator=(const Optional& other) {
    if (!other.alive) {
        Reset();
    } else {
        *this = *(other.Ptr());
    }
    return *this;
}

template <class TElem>
Optional<TElem>& Optional<TElem>::operator=(const TElem& value) {
    if (alive) {
        *(Ptr()) = value;
    } else {
        new (&memory) TElem(value);
        alive = true;
    }
    return *this;
}

template <class TElem>
Optional<TElem>& Optional<TElem>::operator=(Optional&& other) {
    if (!other.alive) {
        Reset();
    } else {
        *this = std::move(*(other.Ptr()));
    }
    return *this;
}

template <class TElem>
Optional<TElem>& Optional<TElem>::operator=(TElem&& value) {
    if (alive) {
        *(Ptr()) = std::move(value);
    } else {
        new (&memory) TElem(std::move(value));
        alive = true;
    }
    return *this;
}

template <class TElem>
void Optional<TElem>::Swap(Optional& other) {
    std::swap(memory, other.memory);
    std::swap(alive, other.alive);
}

template <class TElem>
void Optional<TElem>::Reset() {
    if (alive) {
        Ptr()->~TElem();
        alive = false;
    }
}

template <class TElem>
bool Optional<TElem>::HasValue() const {
    return alive;
}

template <class TElem>
Optional<TElem>::operator bool() const {
    return alive;
}

template <class TElem>
const TElem& Optional<TElem>::Value() const {
    if (!alive) {
        throw BadOptionalAccess{};
    }
    return *(Ptr());
}

template <class TElem>
const TElem& Optional<TElem>::operator*() const {
    return Value();
}

template <class TElem>
const TElem* Optional<TElem>::operator->() const {
    return Ptr();
}

template <class TElem>
const TElem* Optional<TElem>::Ptr() const {
    return reinterpret_cast<const TElem*>(&memory);
}

template <class TElem>
TElem& Optional<TElem>::Value() {
    if (!alive) {
        throw BadOptionalAccess{};
    }
    return *(Ptr());
}

template <class TElem>
TElem& Optional<TElem>::operator*() {
    return Value();
}

template <class TElem>
TElem* Optional<TElem>::operator->() {
    return Ptr();
}

template <class TElem>
TElem Optional<TElem>::ValueOr(const TElem& alternative) const {
    if (alive) {
        return *(Ptr());
    }
    return alternative;
}

template <class TElem>
TElem* Optional<TElem>::Ptr() {
    return reinterpret_cast<TElem*>(&memory);
}

template <class TElem>
template <class... Args>
TElem& Optional<TElem>::Emplace(Args&&... args) {
    Reset();
    new (&memory) TElem(std::forward<Args>(args)...);
    alive = true;
    return Value();
}

#endif  // OPTIONAL_OPTIONAL_H