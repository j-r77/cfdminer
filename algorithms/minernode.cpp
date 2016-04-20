#include "minernode.h"

MinerNode::MinerNode()
    : fItem(-1), fSupp(-1), fHash(-1) {

}

MinerNode::MinerNode(int item, int supp)
    : fItem(item), fSupp(supp), fHash(0) {
        fTids.reserve(supp);
}

MinerNode::MinerNode(int item)
    : fItem(item), fSupp(0), fHash(0) {

}

MinerNode::MinerNode(int item, const TidList& tids)
    : fItem(item), fTids(tids), fSupp(tids.size()), fHash(0) {
    hashTids();
}

MinerNode::MinerNode(int item, const TidList& tids, int supp)
    :fItem(item), fTids(tids), fSupp(supp), fHash(0) {
    hashTids();
}

MinerNode::MinerNode(int item, const TidList& tids, int supp, int hash)
    :fItem(item), fTids(tids), fSupp(supp), fHash(hash) {

}

MinerNode::MinerNode(MinerNode&& rhs)
    : fItem(std::move(rhs.fItem)),
      fTids(std::move(rhs.fTids)),
      fSupp(std::move(rhs.fSupp)),
      fHash(std::move(rhs.fHash)) {

}

MinerNode& MinerNode::operator=(MinerNode&& rhs) {
    fItem = std::move(rhs.fItem);
    fTids = std::move(rhs.fTids);
    fSupp = std::move(rhs.fSupp);
    fHash = std::move(rhs.fHash);
    return *this;
}

void MinerNode::computeSupp() {
    fSupp = fTids.size();
}

void MinerNode::hashTids() {
    fHash = 0;
    for (int t : fTids) {
        fHash += (t+1);
    }
}
