#include "dbtoken.h"

size_t DbToken::hash() const {
    return std::hash<std::string>()(fAttr) ^ std::hash<std::string>()(fVal);
}

DbToken::DbToken() {

}

DbToken::DbToken(const std::string& attr, const std::string& val)
    :fAttr(attr), fVal(val) {
}

DbToken::DbToken(const DbToken& rhs) {
    if (this != &rhs) {
        fAttr = rhs.fAttr;
        fVal = rhs.fVal;
    }
}

bool DbToken::operator==(const DbToken& rhs) const {
    return fAttr == rhs.fAttr && fVal == rhs.fVal;
}

const std::string& DbToken::getAttr() const {
    return fAttr;
}

const std::string& DbToken::getValue() const {
    return fVal;
}
