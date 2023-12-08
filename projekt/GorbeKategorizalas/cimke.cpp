#include "cimke.h"

std::string toLower(const std::string& str) {
    std::string lowerCase;
    for (char ch : str) {
        lowerCase += std::tolower(ch);
    }
    return lowerCase;
}

std::string toUpper(const std::string& str) {
    std::string upperCase;
    for (char ch : str) {
        upperCase += std::toupper(ch);
    }
    return upperCase;
}

int Cimke::getI(){return 14;};

bool Cimke::readIn(ifstream ifs){return false;};
bool Cimke::writeOut(ofstream ofs){return false;};
bool Cimke::readIn(vector<string> param){return false;};
bool Cimke::writeOut(){return false;};

int Cimke::check(Stock* stock, Datum datum){return 0;};
float Cimke::getValue(Stock* stock, Datum datum){return 0;};
float Cimke::getDiffValue(Stock* stock, Datum from, Datum to){return 0;};

int Cimke::check(Stock* stock, Negyed datum){return 0;};
float Cimke::getValue(Stock* stock, Negyed datum){return 0;};
float Cimke::getDiffValue(Stock* stock, Negyed from, Negyed to){return 0;};


bool ujCimkeLetrehozasa(vector<string> param){

}

