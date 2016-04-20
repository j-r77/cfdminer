#include <fstream>
#include <sstream>
#include <iostream>
#include "tabulardatabase.h"

TabularDatabase TabularDatabase::fromFile(std::ifstream& infile,
		char delim, bool headers) {
	TabularDatabase db = TabularDatabase();
	std::string line;
	std::vector<std::string> heads;
	std::getline(infile, line);
	std::istringstream iss(line);
	std::string val;
	int i = 1;
	while (std::getline(iss, val, delim)) {
		if (headers)
			heads.push_back(val);
		else
            heads.push_back("Attr" + std::to_string(i));
		i++;
	}
	db.initDomains(heads.size());
	if (headers)
		std::getline(infile, line);
	int size = i - 1;
	bool eof = false;
	while (!eof)
	{
	    std::istringstream iss(line);
	    std::string val;
        std::vector<int> row(size);
    	int i = 0;
	    while (std::getline(iss, val, delim)) {
	    	int it = db.translateToken(DbToken(heads[i], val));
	    	db.incFreq(it);
            row[i] = it;
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
