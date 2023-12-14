#ifndef LEKERDEZES_H_INCLUDED
#define LEKERDEZES_H_INCLUDED

#include "cimke.h"
#include "adatBeolvas.h"
#include "stock.h"
#include "button.h"

using namespace std;

struct Ertek {
    string cimkeNeve;
    bool siker;
};

struct Pelda {
    string reszNev;
    vector<vector<Datum>> felteteliDatumok; /// külön a feltételeknek és az elemezendõeknek
    vector<vector<Datum>> datumok; /// dupla, mert a viszonyított dátum miatt lehet kettõ is
    vector<vector<Ertek>> elemezendoekTeljesulese; /// a dátum(páros)okhoz tartozó címke értéke
};

struct Feltetel{
    bool komper = false, nap=false, negyed = false, oFloat = false;
    int hanyadik, hanyadikHoz;
    vector<Cimke*> cimkek;
};


struct LekFelt{
    set<Negyed>::iterator negyed;
    set<Negyed>::iterator negyedhez;
    set<Nap>::iterator nap;
    set<Nap>::iterator naphoz;
    bool day = true, komper = false;
    int index1=0, index2=0;

    bool felt=true;
    int feltetelIdx = -1, elemezendoIdx = -1;
};

struct Lekerdezes {
    vector<Feltetel> feltetelek;
    vector<Feltetel> elemezendoek;
    Datum mettol, meddig;
    string reszveny = "", reszvenyCsoport = "";

    vector<string> reszvenyek;
    Stock stock; bool bStock = false;
    vector<Pelda> joPeldak;

    vector<LekFelt> vizsgaltNegy;
    vector<LekFelt> vizsgaltNap;
    vector<vector<int>> napXek, negyedXek;
    int naXPMax=-999, naXPMin=999, neXPMax=-999, neXPMin=999;
    int nullNegyedIdx=0; bool hez = false;
    bool csakNapok = true;

    void process();
    void lStock(int reszIdx);
    void feltetelekLekFeltek();
    void elemezendoLekFeltekHozzaadasa();
    void eltolasMertekeLekFelt();
    void eltolasLekFelt();
    bool tenylegesEltolasLekFelt();

    void naposVizsgalat();
    void negyedevesVizsgalat();
};

void lekerdezesProc(Lekerdezes lek, vector<Pelda> &peldak, ProgressBar &progBar);

#endif // LEKERDEZES_H_INCLUDED
