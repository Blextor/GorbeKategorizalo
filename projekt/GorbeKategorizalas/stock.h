#ifndef STOCK_H_INCLUDED
#define STOCK_H_INCLUDED

#include "common.h"
#include "adatBeolvas.h"
#include "naptar.h"

using namespace std;


struct Arfolyam {
    Idopont idopont;
    float open, close, minimum, maximum, volumen;

    Arfolyam(int h=0, int m=0,
             float n=0, float z=0, float mi=0, float ma=0, float vol=0
             ){idopont.ora=h; idopont.perc=m;
                open=n; close=z; minimum=mi; maximum=ma; volumen=vol;
             }

    bool operator<(const Arfolyam& other) const {
        return idopont < other.idopont;
    }
};

float arfolyamGetMaxErtek(vector<Arfolyam> &v);
float arfolyamGetMinErtek(vector<Arfolyam> &v);
float getPrecFloat(float f, int prec);

struct Nap{
    mutable bool valid=false;
    mutable bool closedMarket = false;
    mutable bool unnep=false;
    Datum datum;
    mutable Datum elozoNap, kovetkezoNap;
    mutable float nyitas, zaras, minimum, maximum, volumen;
    mutable float elozohozKiugras;
    mutable Idopont idoNyitas, idoZaras;
    mutable int hetMelyikNapja;
    mutable set<Arfolyam> percek;

    Nap(int y=0, int m=0, int d=0){
        datum=Datum(y,m,d);
        idoNyitas=Idopont(9,30);
        idoZaras=Idopont(15,59);
    }

    Nap(Datum d){
        datum=d;
        idoNyitas=Idopont(9,30);
        idoZaras=Idopont(15,59);
    }

    bool operator<(const Nap& other) const {
        return datum < other.datum;
    }

    bool operator==(const Nap& other) const {
        return datum == other.datum;
    }
};

struct Negyed{
    Datum idoszakVege;
    /// k�z�lt d�tum
    mutable Datum tenylegesJelentes;
    /// amelyik nap nyit�sa el�tt j�tt ki
    mutable Datum korrigaltTenylegesJelentes;
    /// k�vetkez� negyed�v korrigaltTenylegesJelentes -e
    mutable Datum negyedevVege;

    mutable float jelentettEPS=0, becsultEPS=0;
    mutable float meglepetes=0, meglepetesSzazalek=0;
    mutable long long earn=0, income=0;

    mutable bool nyitasElotti = true;
    mutable bool postMarket = true;
    mutable bool havePostPre = false;

    Negyed(int y=-1, int m=-1, int d=-1){idoszakVege=Datum(y,m,d);}
    Negyed(Datum d){idoszakVege=d;}

    bool operator<(const Negyed& other) const {
        return idoszakVege < other.idoszakVege;
    }

    bool operator==(const Negyed& other) const {
        return idoszakVege == other.idoszakVege;
    }
};

struct Stock{
    string name="teszt";

    set<Negyed> negyedevek;
    set<Nap> mindenNap;

    void adatokKiirasaFajlba (string fajlNev);
    bool adatokBetoltese(string stock);
    bool adatokBetolteseOld(string stock);

    void adatokFeldolgozasa();
    void negyedevekKorrigalasa();




    bool getNap(Nap &ret, Datum datum );
    bool getNegyed(Negyed &ret, Datum datum );

    bool getMin(float &ret, Nap &nap, bool valid = false);
    bool getMin(float &ret, Negyed &negyed, bool valid = false);

    bool getMax(float &ret, Nap &nap, bool valid = false);
    bool getMax(float &ret, Negyed &negyed, bool valid = false);

    bool getNyit(float &ret, Nap &nap, bool valid = false);
    bool getNyit(float &ret, Negyed &negyed, bool valid = false);

    bool getZar(float &ret, Nap &nap, bool valid = false);
    bool getZar(float &ret, Negyed &negyed, bool valid = false);

    bool getNapOdebb(Nap &ret, Nap &nap, int mennyivelOdebb);
    bool getNegyedOdebb(Negyed &ret, Negyed &negyed, int mennyivelOdebb);
};

void loadStock(string name, Stock &stock, bool &sor);


#endif // STOCK_H_INCLUDED
