#ifndef LEKERDEZES_H_INCLUDED
#define LEKERDEZES_H_INCLUDED

#include "cimke.h"

using namespace std;

struct Ertek {
    string cimkeNeve;
    bool siker;
};

struct Pelda {
    string reszNev;
    Datum datum;
    vector<Ertek> elemezendoekTeljesulese;
};

struct Feltetel{
    bool komper = false, nap=false, negyed = false, oFloat = false;
    int hanyadik, hanyadikIg;
    vector<Cimke> cimkek;
};

struct Lekerdezes {
    vector<Feltetel> feltetelek;
    vector<Feltetel> elemezendoek;
    Datum mettol, meddig;
    string reszveny = "", reszvenyCsoport = "";

    vector<string> reszvenyek;

    vector<Pelda> joPeldak;

};

#endif // LEKERDEZES_H_INCLUDED
