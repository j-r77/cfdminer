#ifndef SRC_ALGORITHMS_MINERNODE_H_
#define SRC_ALGORITHMS_MINERNODE_H_

#include "../data/database.h"

struct MinerNode {
    MinerNode();
    MinerNode(int);
    MinerNode(int, int);
    MinerNode(int, const TidList&);
    MinerNode(int, const TidList&, int);
    MinerNode(int, const TidList&, int, int);
    MinerNode(MinerNode&&);
    MinerNode& operator=(MinerNode&&);
    void computeSupp();
    void hashTids();

    int fItem;
    TidList fTids;
    int fSupp;
    int fHash;
};
#endif //SRC_ALGORITHMS_MINERNODE_H_
