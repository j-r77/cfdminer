#include <fstream>
#include <sstream>
#include <iostream>
#include "itemsetdatabase.h"

ItemsetDatabase ItemsetDatabase::fromFile(std::ifstream& infile, char delim) {
	ItemsetDatabase db = ItemsetDatabase();
    db.initDomains(1);
	std::string line;
	std::getline(infile, line);
	bool eof = false;
	while (!eof)
	{
	    std::istringstream iss(line);
	    std::string val;
        std::vector<int> row;
    	int i = 0;
	    while (std::getline(iss, val, delim)) {
            row.push_back(db.translateToken(DbToken(val,"1")));
    		i++;
	    }
	    if (i > 0) {
	    	db.addRow(row);
	    }
	    if (infile.eof()) {
	    	eof = true;
	    }
    	std::getline(infile, line);
	}
	return db;
}
