#ifndef IDOSOR_H_INCLUDED
#define IDOSOR_H_INCLUDED

#include "common.h"

using namespace std;

struct Idopont{
    int ora,perc;
};

struct Datum{
    int year,month,day;
};

struct Idopillanat{
    Datum datum;
    Idopont idopont;
};

struct Arfolyam {
    Idopillanat idopillanat;
    float ertek;
};

struct Idosor{
    vector<Arfolyam> idosor;
};

struct PenzugyiJelentes {
    bool esti = true;
    Idopillanat idopillanat;
    float jelentettEPS;
    float tenylegesEPS;
    float jelentettBevetel;
    float tenylegesBevetel;
};

struct Reszveny {
    string nev;
    Idosor napiAdatok;
    Idosor percenkentiAdatok;
    vector<PenzugyiJelentes> penzugyiJelentesek;

    vector<Idosor> negyedEvek;

    void beolvasasPercAdatok(string mappa);
    void beolvasasNapiAdatok(string fajl);
};

#endif // IDOSOR_H_INCLUDED
