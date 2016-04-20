#include "setutil.h"

std::vector<std::vector<int>> cartesianProduct(const std::vector<int>& sizes) {
    std::vector<int> counters(sizes.size());
    std::vector<std::vector<int>> cartprod;
    cartprod.reserve(product(sizes));

    while (counters[0] < sizes[0]) {
        std::vector<int> row(sizes.size());
        for (unsigned i = 0; i < sizes.size(); i++) {
            row[i] = counters[i];
        }
        int incIx = counters.size() - 1;
        counters[incIx]++;
        while (incIx > 0 && counters[incIx] == sizes[incIx]) {
            counters[incIx] = 0;
            counters[--incIx]++;
        }
        cartprod.push_back(row);
    }
    return cartprod;
}

void subsetsLengthK(const int size, const int k, std::vector<std::bitset<32>>& subs) {
    const int maxB = pow(2, size);
    int x = pow(2, k) - 1;
    while (x < maxB) {
        subs.push_back(std::bitset<32>(x));
        int u = x & (-x);
        int v = x + u;
        x = v + (((v ^ x) / u) >> 2);
    }
}

std::vector<std::bitset<32>> allSubsets(const int size) {
    std::vector<std::bitset<32>> subs;
    subs.reserve(pow(2, size)-2);
    for (int i = 1; i < size; i++) {
        subsetsLengthK(size, i, subs);
    }
    return subs;
}

std::vector<std::bitset<32>> allSubsetsIncl(const int size) {
    std::vector<std::bitset<32>> subs;
    subs.reserve(pow(2, size)-2);
    for (int i = 1; i <= size; i++) {
        subsetsLengthK(size, i, subs);
    }
    return subs;
}
