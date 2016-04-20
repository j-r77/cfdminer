#ifndef SRC_DATA_ITEMSETDATABASE_H_
#define SRC_DATA_ITEMSETDATABASE_H_

#include "database.h"

class ItemsetDatabase: public Database {
public:
	static ItemsetDatabase fromFile(std::ifstream&, char delim=' ');
};

#endif // SRC_DATA_ITEMSETDATABASE_H_
