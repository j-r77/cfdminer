#include <fstream>
#include <iostream>
#include "data/tabulardatabase.h"
#include "algorithms/ccfdminer.h"

int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cout << "Usage: ./CFDMiner infile minsupp maxsize" << std::endl;
		std::cout << "\t where infile is a dataset in csv format, minsupp a positive integer specifying the minimum support of the discovered rules, and maxsize a positive integer specifying the maximum size of the rules, i.e., the maximum number of attributes occurring in the rule" << std::endl;
    }
    else {
        std::ifstream ifile(argv[1]);
        Database db = TabularDatabase::fromFile(ifile, ',');
        int supp = atoi(argv[2]);
        int size = atoi(argv[3]);
        CCFDMiner m(db, supp, size);
        m.run();
    }
}
