#include "ccfdminer.h"
#include "../util/setutil.h"
#include <iostream>
#include <sstream>

CCFDMiner::CCFDMiner(Database& db, int minsupp, int maxsize)
    :fDb(db), fMinSupp(minsupp), fMaxSize(maxsize) {
}

void CCFDMiner::run() {
    mine(Itemset(), getSingletons(fMinSupp), Itemset());
    print();
}

void CCFDMiner::mine(const Itemset& prefix, const std::vector<MinerNode>& items, const Itemset& parentClosure) {
    // Reverse pre-order traversal of items
    if (prefix.size() == fMaxSize) return;
    for (int ix = items.size()-1; ix >= 0; ix--) {
        const MinerNode& node = items[ix];
        const Itemset iset = join(prefix, node.fItem);
        GenMapEntry* newset = addMinGen(GenMapEntry(iset, node.fSupp, node.fHash));
        // Build suffix for next level
        Itemset joins;
        std::vector<MinerNode> suffix;
        if (items.size() - ix - 1 > 2 * fDb.nrAttrs()) {
            std::unordered_map<int,TidList> ijtidMap = bucketTids(items, ix+1, node.fTids);
            for (uint jx = ix + 1; jx < items.size(); jx++) {
                int jtem = items[jx].fItem;
                TidList& ijtids = ijtidMap[jtem];
                int ijsupp = ijtids.size();
                if (ijsupp == node.fSupp) {
                    joins.push_back(jtem);
                }
                else if (ijsupp >= fMinSupp) {
                    suffix.push_back(MinerNode(jtem, std::move(ijtids)));
                }
            }
        }
        else {
            for (uint jx = ix + 1; jx < items.size(); jx++) {
                const MinerNode& j = items[jx];
                TidList ijtids = intersection(node.fTids, j.fTids);
                int ijsupp = ijtids.size();
                if (ijsupp == node.fSupp) {
                    joins.push_back(j.fItem);
                }
                else if (ijsupp >= fMinSupp) {
                    suffix.push_back(MinerNode(j.fItem, std::move(ijtids)));
                }
            }
        }
        if (joins.size() > 1) {
            std::sort(joins.begin(), joins.end());
        }
        Itemset newClosure = join(joins, parentClosure);
        Itemset cset = join(iset, newClosure);
        std::list<GenMapEntry*> postMinGens = getMinGens(cset, node.fSupp, node.fHash);
        for (GenMapEntry* ge : postMinGens) {
            if (ge != newset) {
                Itemset add;
                std::set_difference(cset.begin(), cset.end(), ge->fItems.begin(), ge->fItems.end(), std::inserter(add, add.begin()));
                if (add.size()) {
                    Itemset uni(ge->fClosure.size() + add.size());
                    auto it = std::set_union(add.begin(), add.end(), ge->fClosure.begin(), ge->fClosure.end(), uni.begin());
                    uni.resize((int)(it - uni.begin()));
                    ge->fClosure.swap(uni);
                }
            }
        }
        if (newset) {
            newset->fClosure = newClosure;
            HashStorer<Itemset> h(&newset->fItems);
            fGenerators[h] = newset;
        }
        // Sort suffix and recurse
        if (suffix.size()) {
            sortNodes(suffix);
            mine(iset, suffix, newClosure);
        }
    }
}

void CCFDMiner::print() {
    for (auto it = fGenerators.begin(); it != fGenerators.end(); ++it ) {
            Itemset& items = it->second->fItems;
            Itemset rhs = it->second->fClosure;
            if (rhs.empty()) continue;
            for (int leaveOut : items) {
                Itemset sub = subset(items, leaveOut);
                if (contains(fGenerators, sub)) {
                    const auto& subClosure = fGenerators[sub]->fClosure;
                    std::vector<int> diff(std::max(rhs.size(), subClosure.size()));
                    auto it = std::set_difference(rhs.begin(), rhs.end(), subClosure.begin(), subClosure.end(), diff.begin());
                    if (diff.empty()) {
                        rhs.clear();
                        break;
                    }
                    diff.resize((int)(it - diff.begin()));
                    rhs.swap(diff);
                }
            }
            if (rhs.size()) {
                std::stringstream ssH;
                std::stringstream ssV;
                ssH << "[";
                ssV << "(";
                bool first = true;
                for (int i : *it->first.getData()) {
                    const auto tok = fDb.getToken(i);
                    if (first) {
                        first = false;
                        ssH << tok.first;
                        ssV << tok.second;
                    }
                    else {
                        ssH << ", " << tok.first;
                        ssV << ", " << tok.second;
                    }
                }

                ssH << "] => ";
                ssV << " || ";

                std::string headH = ssH.str();
                std::string headV = ssV.str();
                for (int i : rhs) {
                    const auto tok = fDb.getToken(i);
                    std::cout << headH << tok.first << ", " << headV << tok.second << ")" << std::endl;
                }
            }
        }
}

void CCFDMiner::sortNodes(std::vector<MinerNode>& nodes) const {
    // Sort itemsets by increasing support
    std::sort(nodes.begin(), nodes.end(),
        [](const MinerNode& a, const MinerNode& b)
        {
            return a.fTids.size() < b.fTids.size();
        });
}

std::vector<MinerNode> CCFDMiner::getSingletons(int minsup) const {
    // Build a list of (item, tidlist) pairs
    std::unordered_map<int, int> nodeIndices;
    std::vector<MinerNode> singletons;
    for (unsigned item = 1; item <= fDb.nrItems(); item++) {
        if (fDb.frequency(item) >= minsup) {
            singletons.push_back(MinerNode(item, fDb.frequency(item)));
            nodeIndices[item] = singletons.size()-1;
        }
    }

    for (unsigned row = 0; row < fDb.size(); row++) {
        for (int item : fDb.getRow(row)) {
            if (contains(nodeIndices, item)) {
                singletons[nodeIndices.at(item)].fTids.push_back(row);
            }
        }
    }
    
    for (MinerNode& node : singletons) {
        node.hashTids();
    }
    
    sortNodes(singletons);
    return singletons;
}

GenMapEntry* CCFDMiner::addMinGen(const GenMapEntry& newset) {
    if (fGenMap.find(newset.fHash) != fGenMap.end()) {
        for (const GenMapEntry& g : fGenMap.at(newset.fHash)) {
            if (g.fSupp == newset.fSupp && isSubsetOf(g.fItems, newset.fItems)) {
                return 0;
            }
        }
    }
    fGenMap[newset.fHash].push_back(newset);
    return &fGenMap[newset.fHash].back();
}

std::list<GenMapEntry*> CCFDMiner::getMinGens(const Itemset& items, int supp, int hash) {
    std::list<GenMapEntry*> minGens;
    if (fGenMap.find(hash) != fGenMap.end()) {
        for (GenMapEntry& g : fGenMap.at(hash)) {
            if (g.fSupp == supp && isSubsetOf(g.fItems, items)) {
                minGens.push_back(&g);
            }
        }
    }
    return minGens;
}

std::unordered_map<int,TidList> CCFDMiner::bucketTids(const std::vector<MinerNode>& items,
                                                      uint jx, const TidList& nodeTids) const {
    std::unordered_map<int,TidList> ijtidMap;
    for (; jx < items.size(); jx++) {
        ijtidMap[items[jx].fItem].reserve(nodeTids.size());
    }
    for (int t : nodeTids) {
        const Transaction& tup = fDb.getRow(t);
        for (uint vi = 0; vi < tup.size(); vi++) {
            if (ijtidMap.find(tup[vi]) != ijtidMap.end()) {
                ijtidMap.at(tup[vi]).push_back(t);
            }
        }
    }
    return ijtidMap;
}
