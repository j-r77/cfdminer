#include "database.h"
#include <sstream>
#include <iostream>
#include <algorithm>

Database::Database()
    :fNumTokens(1), fAttrCounter(1) {

}

unsigned Database::size() const {
    return fData.size();
}

unsigned Database::nrAttrs() const {
    return fAttributes.size();
}

unsigned Database::nrItems() const {
    return fTokenToIntMap.size();
}

const Transaction& Database::getRow(int row) const {
    return fData.at(row);
}

std::pair<std::string,std::string> Database::getToken(int i) const {
    auto tok = fIntToTokenMap.find(i)->second;
    std::pair<std::string,std::string> res(tok.getAttr(), tok.getValue());
    return res;
}

void Database::addRow(const std::vector<int>& vec) {
    fData.push_back(vec);
}

void Database::initDomains(int nrDoms) {
    fDomains.resize(nrDoms);
}

std::set<int>& Database::getDomain(int item) {
    int attr = fAttrMap[item];
    return fDomains[attr-1];
}

int Database::translateToken(const DbToken& token) {
    int& val = fTokenToIntMap[token];
    if (val > 0) {
        return val;
    }
    else {
        std::string attr = token.getAttr();
        if (std::find(fAttributes.begin(), fAttributes.end(), attr) == fAttributes.end()) {
            fAttributes.push_back(attr);
        }
        int& attrTok = fAttrStrMap[token.getAttr()];
        if (attrTok <= 0) {
            attrTok = fAttrCounter++;
        }
        fIntToTokenMap[fNumTokens] = token;
        val = fNumTokens;
        fAttrMap[val] = attrTok;
        int item = fNumTokens++;
        if (fFrequencies.size() < (unsigned)item) {
            fFrequencies.resize(item);
        }
        if (fDomains.size() < (unsigned)attrTok) {
            fDomains.push_back(std::set<int>());
        }
        fDomains[attrTok-1].insert(item);
        fFrequencies[item-1] = 0;
        return item;
    }
}

void Database::incFreq(int i) {
    fFrequencies[i-1]++;
}

int Database::frequency(int i) const {
    return fFrequencies[i-1];
}

int Database::getAttrToken(int t) const {
    return fAttrMap.at(t);
}

void Database::write(std::ofstream& file, char delim) const {
    for (const auto& row : fData) {
        for (const int& i : row) {
            file << i << delim;
        }
        file << std::endl;
    }
}

const std::vector<std::string>& Database::getAttrNames() const {
    return fAttributes;
}
