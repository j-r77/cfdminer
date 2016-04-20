#ifndef SRC_DATA_DBTOKEN_H_
#define SRC_DATA_DBTOKEN_H_

#include <string>

class DbToken {
public:
	DbToken();
	DbToken(const std::string&, const std::string&);
	DbToken(const DbToken&);
	size_t hash() const;
	bool operator==(const DbToken&) const;
	const std::string& getAttr() const;
	const std::string& getValue() const;

private:
	std::string fAttr;
	std::string fVal;
};

namespace std {
    template<>
    struct hash<DbToken> {
        size_t operator()(const DbToken &t) const {
            return t.hash();
        }
    };
}

#endif // SRC_DATA_DBTOKEN_H_
