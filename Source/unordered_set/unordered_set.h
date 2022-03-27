#ifndef UNORDERED_SET_UNORDERED_SET_H
#define UNORDERED_SET_UNORDERED_SET_H

#define YANDEX_CONTEST_REMINDER

#include <list>
#include <iterator>
#include <algorithm>

template <class KeyType, class HashType = std::hash<KeyType>, class EqualType = std::equal_to<KeyType>>
class UnorderedSet {
public:
    std::list<KeyType>* set;
    std::size_t size;
    std::size_t bucket_count;

    UnorderedSet();
    explicit UnorderedSet(const std::size_t& count);
    template <class InputIterator>
    UnorderedSet(InputIterator begin, InputIterator end);
    template <class InputIterator>
    UnorderedSet(InputIterator begin, InputIterator end, const std::size_t& count);
    ~UnorderedSet();
    UnorderedSet(const UnorderedSet& other);
    UnorderedSet(UnorderedSet&& other);
    UnorderedSet& operator=(const UnorderedSet& other);
    UnorderedSet& operator=(UnorderedSet&& other);
    std::size_t Size() const;
    bool Empty() const;
    void Clear();
    void Insert(const KeyType& key);
    void Insert(KeyType&& key);
    template <class InputIterator>
    void Insert(InputIterator begin, InputIterator end);
    void Erase(const KeyType& key);
    bool Find(const KeyType& key) const;
    void Rehash(const std::size_t& new_bucket_count);
    void Reserve(const std::size_t& new_bucket_count);
    std::size_t BucketCount() const;
    std::size_t BucketSize(const std::size_t& id) const;
    std::size_t Bucket(const KeyType& key) const;
    long double LoadFactor() const;
};

template <class KeyType, class HashType, class EqualType>
UnorderedSet<KeyType, HashType, EqualType>::UnorderedSet() : set(nullptr), size(0l), bucket_count(0l) {
}

template <class KeyType, class HashType, class EqualType>
UnorderedSet<KeyType, HashType, EqualType>::UnorderedSet(const std::size_t& count) : UnorderedSet() {
    set = new std::list<KeyType>[count + 1] {};
    bucket_count = count;
}

template <class KeyType, class HashType, class EqualType>
template <class InputIterator>
UnorderedSet<KeyType, HashType, EqualType>::UnorderedSet(InputIterator begin, InputIterator end) : UnorderedSet() {
    Insert(begin, end);
}

template <class KeyType, class HashType, class EqualType>
template <class InputIterator>
UnorderedSet<KeyType, HashType, EqualType>::UnorderedSet(InputIterator begin, InputIterator end,
                                                         const std::size_t& count)
    : UnorderedSet(count) {
    Insert(begin, end);
    if (count > size) {
        Reserve(count);
    }
}

template <class KeyType, class HashType, class EqualType>
UnorderedSet<KeyType, HashType, EqualType>::~UnorderedSet() {
    delete[] set;
}

template <class KeyType, class HashType, class EqualType>
UnorderedSet<KeyType, HashType, EqualType>::UnorderedSet(const UnorderedSet& other) : UnorderedSet() {
    *this = other;
}

template <class KeyType, class HashType, class EqualType>
UnorderedSet<KeyType, HashType, EqualType>::UnorderedSet(UnorderedSet&& other) : UnorderedSet() {
    *this = std::move(other);
}

template <class KeyType, class HashType, class EqualType>
UnorderedSet<KeyType, HashType, EqualType>& UnorderedSet<KeyType, HashType, EqualType>::operator=(
    const UnorderedSet& other) {
    Clear();
    size = other.size;
    bucket_count = other.bucket_count;
    set = new std::list<KeyType>[bucket_count + 1] {};
    for (std::size_t i = 0l; i < size; i++) {
        set[i] = other.set[i];
    }
    return *this;
}

template <class KeyType, class HashType, class EqualType>
UnorderedSet<KeyType, HashType, EqualType>& UnorderedSet<KeyType, HashType, EqualType>::operator=(
    UnorderedSet&& other) {
    size = std::move(other.size);
    bucket_count = std::move(other.bucket_count);
    std::swap(set, other.set);
    other.Clear();
    return *this;
}

template <class KeyType, class HashType, class EqualType>
std::size_t UnorderedSet<KeyType, HashType, EqualType>::Size() const {
    return size;
}

template <class KeyType, class HashType, class EqualType>
bool UnorderedSet<KeyType, HashType, EqualType>::Empty() const {
    return size == 0l;
}

template <class KeyType, class HashType, class EqualType>
void UnorderedSet<KeyType, HashType, EqualType>::Clear() {
    delete[] set;
    set = nullptr;
    size = 0l;
    bucket_count = 0l;
}

template <class KeyType, class HashType, class EqualType>
void UnorderedSet<KeyType, HashType, EqualType>::Insert(const KeyType& key) {
    if (size == bucket_count) {
        std::size_t new_bucket_count = bucket_count ? bucket_count * 2l : 1;
        Rehash(new_bucket_count);
    }
    if (!Find(key)) {
        std::size_t hash = HashType{}(key) % bucket_count;
        set[hash].push_back(key);
        ++size;
    }
}

template <class KeyType, class HashType, class EqualType>
void UnorderedSet<KeyType, HashType, EqualType>::Insert(KeyType&& key) {
    if (size == bucket_count) {
        std::size_t new_bucket_count = bucket_count ? bucket_count * 2l : 1;
        Rehash(new_bucket_count);
    }
    if (!Find(key)) {
        std::size_t hash = HashType{}(key) % bucket_count;
        set[hash].push_back(key);
        ++size;
    }
}

template <class KeyType, class HashType, class EqualType>
template <class InputIterator>
void UnorderedSet<KeyType, HashType, EqualType>::Insert(InputIterator begin, InputIterator end) {
    for (auto it = begin; it != end; ++it) {
        Insert(*it);
    }
}

template <class KeyType, class HashType, class EqualType>
void UnorderedSet<KeyType, HashType, EqualType>::Erase(const KeyType& key) {
    if (!Find(key)) {
        return;
    }
    std::size_t hash = HashType{}(key) % bucket_count;
    auto position = set[hash].begin();
    while (position != set[hash].end()) {
        if (*position == key) {
            break;
        }
        ++position;
    }
    if (position != set[hash].end()) {
        set[hash].erase(position);
        --size;
    }
}

template <class KeyType, class HashType, class EqualType>
bool UnorderedSet<KeyType, HashType, EqualType>::Find(const KeyType& key) const {
    if (!set) {
        return false;
    }
    std::size_t hash = HashType{}(key) % bucket_count;
    auto position = set[hash].begin();
    while (position != set[hash].end()) {
        if (EqualType{}(*position, key)) {
            break;
        }
        ++position;
    }
    return position != set[hash].end();
}

template <class KeyType, class HashType, class EqualType>
void UnorderedSet<KeyType, HashType, EqualType>::Rehash(const std::size_t& new_bucket_count) {
    if (new_bucket_count < size) {
        return;
    }
    std::size_t hash = 0l;
    auto* new_set = new std::list<KeyType>[new_bucket_count + 1] {};
    for (std::size_t i = 0l; i < bucket_count; i++) {
        for (auto it = set[i].begin(); it != set[i].end(); ++it) {
            hash = HashType{}(*it) % new_bucket_count;
            new_set[hash].push_back(*it);
        }
    }
    std::swap(set, new_set);
    bucket_count = new_bucket_count;
    delete[] new_set;
}

template <class KeyType, class HashType, class EqualType>
void UnorderedSet<KeyType, HashType, EqualType>::Reserve(const std::size_t& new_bucket_count) {
    if (!set) {
        bucket_count = new_bucket_count;
        set = new std::list<KeyType>[bucket_count + 1] {};
    } else if (new_bucket_count > size) {
        auto* new_set = new std::list<KeyType>[new_bucket_count + 1];
        for (std::size_t i = 0l; i < bucket_count; i++) {
            new_set[i] = std::move(set[i]);
        }
        std::swap(set, new_set);
        bucket_count = new_bucket_count;
        delete[] new_set;
    }
}

template <class KeyType, class HashType, class EqualType>
std::size_t UnorderedSet<KeyType, HashType, EqualType>::BucketCount() const {
    return bucket_count;
}

template <class KeyType, class HashType, class EqualType>
std::size_t UnorderedSet<KeyType, HashType, EqualType>::BucketSize(const std::size_t& id) const {
    if (!set || id > bucket_count) {
        return 0l;
    }
    return static_cast<int64_t>(set[id].size());
}

template <class KeyType, class HashType, class EqualType>
std::size_t UnorderedSet<KeyType, HashType, EqualType>::Bucket(const KeyType& key) const {
    return HashType{}(key) % bucket_count;
}

template <class KeyType, class HashType, class EqualType>
long double UnorderedSet<KeyType, HashType, EqualType>::LoadFactor() const {
    if (!set) {
        return 0.0f;
    }
    return static_cast<double>(size) / static_cast<double>(bucket_count);
}

#endif  // UNORDERED_SET_UNORDERED_SET_H