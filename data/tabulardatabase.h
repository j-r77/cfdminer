#ifndef SRC_DATA_TABULARDATABASE_H_
#define SRC_DATA_TABULARDATABASE_H_

#include "database.h"

class TabularDatabase: public Database {
public:
	static TabularDatabase fromFile(std::ifstream&, char delim, bool headers=true);
};

#endif /* SRC_DATA_TABULARDATABASE_H_ */
