#ifndef SETUTIL_H
#define SETUTIL_H

#include <cmath>
#include <bitset>
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> cartesianProduct(const std::vector<int>&);
std::vector<std::bitset<32>> allSubsets(const int);
std::vector<std::bitset<32>> allSubsetsIncl(const int);
void subsetsLengthK(const int, const int, std::vector<std::bitset<32>>&);

template <typename T>
typename T::value_type product(const T& items) {
   typename T::value_type prod{};
   for (const typename T::value_type& i : items) {
       prod *= i;
   }
   return prod;
}

template <typename T>
typename T::value_type implode(const T& collection) {
    typename T::value_type result;
    for (const auto& c : collection) {
        result = join(result, c);
    }
    return result;
}

template<typename T>
bool isSubsetOf(const T& sub, const T& super) {
    return std::includes(super.begin(), super.end(), sub.begin(), sub.end());
}

template <typename T>
bool containsSubsetOf(const T& collection, const typename T::value_type& item) {
    for (const typename T::value_type& s : collection) {
        if (isSubsetOf(s, item)) return true;
    }
    return false;
}

template <typename T>
bool containsSupersetOf(const T& collection, const typename T::value_type& item) {
    for (const typename T::value_type& s : collection) {
        if (isSubsetOf(item, s)) return true;
    }
    return false;
}

template<typename T>
T subset(const T& items, const std::bitset<32> mask) {
    T sub;
    sub.reserve(mask.count());
    for (unsigned mi = 0; mi < items.size(); mi++) {
        if (mask[mi]) {
            sub.push_back(items[mi]);
        }
    }
    return sub;
}

template<typename T>
T subset(const T& items, const typename T::value_type leaveOut) {
    T sub;
    sub.reserve(items.size() - 1);
    for (unsigned mi = 0; mi < items.size(); mi++) {
        if (items[mi] != leaveOut) {
            sub.push_back(items[mi]);
        }
    }
    return sub;
}

template<typename T>
void insertSorted(const T& in, const typename T::value_type item, T& out) {
    auto it = in.begin();
    while (it != in.end() && *it < item) { it++; }
    if (it != in.begin()) {
        out.insert(out.begin(), in.begin(), it);
    }
    int offset = (int)(it - in.begin());
    out.insert(out.begin() + offset, item);
    if (in.size() && it != in.end()) {
        out.insert(out.begin() + offset + 1, it, in.end());
    }
}

template<typename T>
T* intersection(const T* lhs, const T* rhs) {
    T* isect = new T(lhs->size());
    auto it = std::set_intersection(lhs->begin(), lhs->end(), rhs->begin(), rhs->end(), isect->begin());
    isect->resize((int)(it - isect->begin()));
    return isect;
}

template<typename T>
T intersection(const T lhs, const T rhs) {
    T isect(lhs.size());
    auto it = std::set_intersection(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), isect.begin());
    isect.resize((int)(it - isect.begin()));
    return isect;
}

template<typename T>
T* join(const T* lhs, const T* rhs) {
    T* uni = new T(lhs->size() + rhs->size());
    auto it = std::set_union(lhs->begin(), lhs->end(), rhs->begin(), rhs->end(), uni->begin());
    uni->resize((int)(it - uni->begin()));
    return uni;
}

template<typename T>
T join(const T& lhs, const T& rhs) {
    T uni(lhs.size() + rhs.size());
    auto it = std::set_union(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), uni.begin());
    uni.resize((int)(it - uni.begin()));
    return uni;
}

template<typename T>
T join(const T& lhs, const typename T::value_type& rhsItem) {
    T res;
    res.reserve(lhs.size() + 1);
    insertSorted(lhs, rhsItem, res);
    return res;
}

template<typename T>
T* join(const T* lhs, const typename T::value_type& rhsItem) {
    T* res = new T();
    res->reserve(lhs->size() + 1);
    insertSorted(*lhs, rhsItem, *res);
    return res;
}

template <typename T>
bool contains(const T& collection, const typename T::value_type& item) {
    return collection.find(item) != collection.end();
}

template <typename T>
bool contains(const T& collection, const typename T::key_type& item) {
    return collection.find(item) != collection.end();
}
#endif // SETUTIL_H
