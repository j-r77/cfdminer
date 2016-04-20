#ifndef SRC_ALGORITHMS_CCFDMINER_H_
#define SRC_ALGORITHMS_CCFDMINER_H_

#include "minernode.h"
#include "genmapentry.h"
#include "../util/hashstorer.h"
#include <list>

class CCFDMiner {
public:
    CCFDMiner(Database&, int, int);
	void run();

protected:
    void mine(const Itemset&, const std::vector<MinerNode>&, const Itemset&);
    void print();

    std::vector<MinerNode> getSingletons(int) const;
    void sortNodes(std::vector<MinerNode>&) const;
    std::unordered_map<int,TidList> bucketTids(const std::vector<MinerNode>&, uint, const TidList&) const;

    GenMapEntry* addMinGen(const GenMapEntry&);
    std::list<GenMapEntry*> getMinGens(const Itemset&, int, int);

private:
    Database& fDb;
    std::unordered_map<int, std::list<GenMapEntry>> fGenMap;
    std::unordered_map<HashStorer<Itemset>, GenMapEntry*> fGenerators;
	int fMinSupp;
    int fMaxSize;
};
#endif //SRC_ALGORITHMS_CCFDMINER_H_
