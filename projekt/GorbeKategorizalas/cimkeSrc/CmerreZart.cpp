#include "CmerreZart.h"

int CmerreZart::getI(){return categoryID;};

bool CmerreZart::readIn(ifstream ifs){
    /*
    int napNegy = 0;
    ifs>>name>>napNegy>>minimum>>tol>>ig>>hol>>kornyezet;
    napNegyedSet(napNegy);
    */
    return true;
}
bool CmerreZart::writeOut(ofstream ofs){
    /*
    int tipus = getCimkeType(comperator,onlyDaily,onlyQuarter,onlyFloat);
    ofs<<" "<<name<<" "<<tipus<<" "<<minimum<<" "<<tol<<" "<<ig<<" "<<hol<<" "<<kornyezet;
    */
    return true;
}


bool CmerreZart::readIn(vector<string> param) {
    /*
    name = param[1];
    stringstream ss; ss<<param[2];
    int t;
    ss>>t; if (ss.fail()) return false;

    napNegyedSet(t);

    bool b;
    stringstream ssb; ssb<<param[3];
    ssb>>b; if (ssb.fail()) return false;
    minimum=b;

    stringstream ss2; ss2<<param[4];
    float f;
    ss2>>f; if (ss2.fail()) return false;
    tol=f;

    stringstream ss3; ss3<<param[5];
    ss3>>f; if (ss3.fail()) return false;
    ig=f;

    stringstream ss4; ss4<<param[6];
    ss4>>f; if (ss4.fail()) return false;
    hol=f;

    stringstream ss5; ss5<<param[7];
    ss5>>f; if (ss5.fail()) return false;
    kornyezet=f;
    */

    return true;
}

bool CmerreZart::writeOut() {
    AppConfig& config = AppConfig::getInstance();
    /*
    string rtDir = config.getRootDirectory();
    string fajlName = rtDir+"cimkek.txt";
    ofstream ofs;
    ofs.open(fajlName,std::ios::app);
    ofs<<IDname<<" "<<name<<" "<<getCimkeType(comperator,onlyDaily,onlyQuarter,onlyFloat)
    <<" "<<minimum<<" "<<tol<<" "<<ig<<" "<<hol<<" "<<kornyezet<<endl;
    ofs.close();
    */
    return true;
}

bool CmerreZart::check(Stock& stock, Datum datum, Datum datumhoz){
    Nap azNap(datum);
    set<Nap>::iterator it = stock.mindenNap.find(azNap);
    if (it==stock.mindenNap.end()) return 0;
    azNap = *it;
    if (azNap.zaras>azNap.nyitas) return 1;
    return -1;
}

float CmerreZart::getValue(Stock& stock, Datum datum) {

    return 0;
}

float CmerreZart::getDiffValue(Stock& stock, Datum from, Datum to){

    return 0;
}

bool CmerreZart::check(Stock& stock, Negyed datum, Negyed datumhoz){

    return 0;
}


float CmerreZart::getValue(Stock& stock, Negyed datum) {

    return 0;
}

float CmerreZart::getDiffValue(Stock& stock, Negyed from, Negyed to){

    return 0;
}

