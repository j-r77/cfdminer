#ifndef SRC_DATA_DATABASE_H_
#define SRC_DATA_DATABASE_H_

#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <set>
#include "dbtoken.h"

typedef std::vector<int> Itemset;
typedef std::vector<int> Transaction;
typedef std::vector<int> TidList;

class Database {
public:
	Database();
	unsigned size() const;
	unsigned nrAttrs() const;
    unsigned nrItems() const;

    const Transaction& getRow(int) const;
	int translateToken(const DbToken&);

	void write(std::ofstream& file, char delim=' ') const;
	std::pair<std::string,std::string> getToken(int i) const;
	int getAttrToken(int t) const;
	int frequency(int i) const;
	std::set<int>& getDomain(int);
    const std::vector<std::string>& getAttrNames() const;

protected:
    void initDomains(int);
    void incFreq(int i);
    void addRow(const Transaction&);

private:
    std::vector<Transaction> fData; //array of data represented as rows of integers
	std::vector<int> fFrequencies;
	std::unordered_map<DbToken, int> fTokenToIntMap; //maps integers back to their original tokens
	std::unordered_map<int, DbToken> fIntToTokenMap;
	std::unordered_map<std::string, int> fAttrStrMap;
	std::unordered_map<int, int> fAttrMap;
	std::vector<std::set<int>> fDomains;
    std::vector<std::string> fAttributes;
	int fNumTokens;
	int fAttrCounter;
};

#endif // SRC_DATA_DATABASE_H_
