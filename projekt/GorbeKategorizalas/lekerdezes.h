#ifndef LEKERDEZES_H_INCLUDED
#define LEKERDEZES_H_INCLUDED

#include "cimkeSrc/OsszesCimke.h"

using namespace std;

struct Ertek {
    string cimkeNeve;
    int ih;
    float ertek;
};

struct Pelda {
    string reszNev;
    Datum datum;
    vector<Ertek> cimkekErteke;
};

struct Lekerdezes {
    vector<Cimke> feltetelek;
    vector<Cimke> vizsgalandok;
    Datum mettol, meddig;

    vector<string> reszvenyek;

    vector<Pelda> joPeldak;

    void addFeltetel(Cimke cimke){feltetelek.push_back(cimke);}
    void addVizsgalando(Cimke cimke){vizsgalandok.push_back(cimke);}
};

#endif // LEKERDEZES_H_INCLUDED
