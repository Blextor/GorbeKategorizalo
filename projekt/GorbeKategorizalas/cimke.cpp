#include "cimke.h"

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

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

bool Cimke::check(Stock& stock, Datum datum, Datum datumhoz){return 0;};
float Cimke::getValue(Stock& stock, Datum datum){return 0;};
float Cimke::getDiffValue(Stock& stock, Datum from, Datum to){return 0;};

bool Cimke::check(Stock& stock, Negyed datum, Negyed datumhoz){return 0;};
float Cimke::getValue(Stock& stock, Negyed datum){return 0;};
float Cimke::getDiffValue(Stock& stock, Negyed from, Negyed to){return 0;};


