#include "CLokMinMax.h"

LokMinMax::LokMinMax(){
    comperator=false;
    IDname="lokMinMax";
    inputok = {"min/max","tol","ig","hol","kornyezet"};
    Cimke::categoryID = 123;
}


int LokMinMax::getI(){return categoryID;};

bool LokMinMax::readIn(ifstream ifs){
    int napNegy = 0;
    ifs>>name>>napNegy>>minimum>>tol>>ig>>hol>>kornyezet;
    napNegyedSet(napNegy);
    return true;
}
bool LokMinMax::writeOut(ofstream ofs){
    int tipus = getCimkeType(comperator,onlyDaily,onlyQuarter,onlyFloat);
    ofs<<" "<<name<<" "<<tipus<<" "<<minimum<<" "<<tol<<" "<<ig<<" "<<hol<<" "<<kornyezet;
    return true;
}


bool LokMinMax::readIn(vector<string> param) {
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

    return true;
}
bool LokMinMax::writeOut() {
    AppConfig& config = AppConfig::getInstance();
    string rtDir = config.getRootDirectory();
    string fajlName = rtDir+"cimkek.txt";
    ofstream ofs;
    ofs.open(fajlName,std::ios::app);
    ofs<<IDname<<" "<<name<<" "<<getCimkeType(comperator,onlyDaily,onlyQuarter,onlyFloat)
    <<" "<<minimum<<" "<<tol<<" "<<ig<<" "<<hol<<" "<<kornyezet<<endl;
    ofs.close();
    return true;
}

int LokMinMax::check(Stock* stock, Datum datum){
    Nap nap(datum);
    if (!stock->getNap(nap,datum)) return 0;
    int from=tol, to=ig, where=hol, r=kornyezet;
    if (onlyFloat){
        from = tol * 390.0f; to = ig * 390.0f; where = hol * 390.0f; r = kornyezet * 390.0f;
    }
    from = max(0,from);
    float szelsoErtek = 0; int szelsoIndex = -1;
    if (minimum) szelsoErtek=999999;
    set<Arfolyam>::iterator it = nap.percek.begin();
    for (int i=0; i<from && i<390;i++) it++;
    for (int i=from; i<390 && i<=to;i++){
        if (minimum && it->minimum<szelsoErtek){
            szelsoErtek=it->minimum;
            szelsoIndex=i;
        }
        if (!minimum && it->maximum>szelsoErtek){
            szelsoErtek=it->maximum;
            szelsoIndex=i;
        }
        it++;
    }
    if (where-r<=szelsoIndex && szelsoIndex<=where+r) return 1;
    return 0;
}		/// nézzük meg, hogy igaz-e az adott dátumra
float LokMinMax::getValue(Stock* stock, Datum datum){return 0;}	/// mi az aznapra az értéke
float LokMinMax::getDiffValue(Stock* stock, Datum from, Datum to){return 0;} /// mi a két nap közötti érték különöbzet

int LokMinMax::check(Stock* stock, Negyed datum){

    if (!stock->getNegyed(datum,datum.idoszakVege)) return 0;
    int from=tol, to=ig, where=hol, r=kornyezet;
    if (onlyFloat){
        from = tol * 390.0f; to = ig * 390.0f; where = hol * 390.0f; r = kornyezet * 390.0f;
    }
    from = max(0,from);
    float szelsoErtek = 0; int szelsoIndex = -1;
    if (minimum) szelsoErtek=999999;
    set<Nap>::iterator it = stock->mindenNap.find(datum.korrigaltTenylegesJelentes);
    if (it==stock->mindenNap.end()) return 0;
    for (int i=0; i<from && i<390;i++) it++;
    for (int i=from; it!=stock->mindenNap.end() && i<=to;i++){
        if (minimum && it->minimum<szelsoErtek){
            szelsoErtek=it->minimum;
            szelsoIndex=i;
        }
        if (!minimum && it->maximum>szelsoErtek){
            szelsoErtek=it->maximum;
            szelsoIndex=i;
        }
        it++;
    }
    if (where-r<=szelsoIndex && szelsoIndex<=where+r) return 1;
    return 0;
}
float LokMinMax::getValue(Stock* stock, Negyed datum){return 0;}
float LokMinMax::getDiffValue(Stock* stock, Negyed from, Negyed to){return 0;}
