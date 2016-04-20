#ifndef SRC_ALGORITHMS_GENMAPENTRY_H_
#define SRC_ALGORITHMS_GENMAPENTRY_H_

#include "../data/database.h"

struct GenMapEntry {
    GenMapEntry(const Itemset& items, int supp, int hash = 0)
        : fItems(items), fSupp(supp), fHash(hash) {

    }

    std::vector<int> fItems;
    std::vector<int> fClosure;
	int fSupp;
    int fHash;
};

#endif //SRC_ALGORITHMS_GENMAPENTRY_H_
