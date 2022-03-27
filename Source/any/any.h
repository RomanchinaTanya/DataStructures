#ifndef ANY_ANY_H
#define ANY_ANY_H

#include <iostream>
#include <memory>

class BadAnyCast : public std::bad_cast {
public:
    BadAnyCast() : std::bad_cast() {
    }
};

class IBase {
public:
    virtual IBase* Clone() = 0;

    virtual ~IBase() = default;
};

template <class ElemT>
class Derived : public IBase {
public:
    ElemT derived;

    Derived() : derived(static_cast<ElemT>(0)) {
    }

    Derived(const ElemT& value) {  // NOLINT
        *this = value;
    }

    Derived(ElemT&& value) : derived(std::move(value)) {  // NOLINT
    }

    Derived(const Derived& other) {
        *this = other;
    }

    Derived& operator=(const ElemT& value) {
        derived = value;
        return *this;
    }

    Derived& operator=(const Derived& other) {
        derived = other.derived;
        return *this;
    }

    ~Derived() override = default;

    Derived<ElemT>* Clone() override {
        return new Derived<ElemT>(derived);
    }
};

class Any {
public:
    IBase* any;

    Any() : any(nullptr) {
    }

    template <class ElemT>
    Any(const ElemT& value) : Any() {  // NOLINT
        *this = value;
    }

    Any(const Any& other) : Any() {  // NOLINT
        *this = other;
    }

    Any(Any&& other) : Any() {  // NOLINT
        *this = std::move(other);
    }

    template <class ElemT>
    Any& operator=(const ElemT& value) {
        Reset();
        auto* object = new Derived<ElemT>(value);
        any = object;
        return *this;
    }

    Any& operator=(const Any& other) {
        if (any != other.any) {
            Reset();
            any = other.any->Clone();
        }
        return *this;
    }

    Any& operator=(Any&& other) {
        Reset();
        any = std::move(other.any);
        other.any = nullptr;
        return *this;
    }

    ~Any() {
        Reset();
    }

    void Reset() {
        delete any;
        any = nullptr;
    }

    bool HasValue() {
        return any != nullptr;
    }

    void Swap(Any& other) {
        std::swap(any, other.any);
    }
};

template <class ElemT>
ElemT AnyCast(const Any& value) {
    auto* any_ptr = dynamic_cast<Derived<ElemT>*>(value.any);
    if (!any_ptr) {
        throw BadAnyCast{};
    }
    return any_ptr->derived;
}

template <class ElemT, class... Args>
Any MakeAny(Args&... args) {
    return Any(ElemT(std::forward<Args>(args)...));
}

#endif  // ANY_ANY_H