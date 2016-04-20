#include <fstream>
#include <iostream>
#include "data/tabulardatabase.h"
#include "algorithms/ccfdminer.h"

int main(int argc, char *argv[])
{
    if (argc == 1) {
        std::cout << "Usage: ./CFDMiner infile minsupp maxsize" << std::endl;
    }
    else if (argc == 4) {
        std::ifstream ifile(argv[1]);
        Database db = TabularDatabase::fromFile(ifile, ',');
        int supp = atoi(argv[2]);
        int size = atoi(argv[3]);
        CCFDMiner m(db, supp, size);
        m.run();
    }
}
