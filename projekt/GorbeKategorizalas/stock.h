#ifndef STOCK_H_INCLUDED
#define STOCK_H_INCLUDED

#include "common.h"
#include "adatBeolvas.h"

using namespace std;

struct Idopont{
    int ora=-1,perc=-1;
    Idopont(int o=-1, int p=-1){ora=o; perc=p;}

    bool operator<(const Idopont& other) const {
        if (ora==other.ora)
            return perc < other.perc;
        return ora<other.ora;
    }
};

struct Datum{
    int year=-1,month=-1,day=-1;
    Datum(int y=-1, int m=-1, int d=-1){year=y; month=m; day=d;}

    bool operator<(const Datum& other) const {
        if (year != other.year)
            return year < other.year;
        if (month != other.month)
            return month < other.month;
        return day < other.day;
    }

    int tavolsag(Datum other){
        // Létrehozza a kezdeti dátumot: 1990. január 1.
        std::tm start_date = {};
        start_date.tm_year = other.year - 1900; // tm_year: évszám 1900-tól
        start_date.tm_mon = other.month-1; // tm_mon: 0-tól indexelt hónap (január = 0)
        start_date.tm_mday = other.day; // tm_mday: nap a hónapban

        // Létrehozza az adott dátumot
        std::tm given_date = {};
        given_date.tm_year = year - 1900;
        given_date.tm_mon = month - 1; // A hónap 1-tõl indexelt, ezért 1-et kell kivonnunk belõle
        given_date.tm_mday = day;

        // Átalakítja a tm struktúrákat time_t típussá (másodpercek 1970. január 1. óta)
        std::time_t start_time_t = std::mktime(&start_date);
        std::time_t given_time_t = std::mktime(&given_date);

        // Kiszámolja a napok számát a két dátum között
        int days = (given_time_t - start_time_t) / (24 * 60 * 60);
        return abs(days);
    }


};

struct Idopillanat{
    Datum datum;
    Idopont idopont;
};

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

struct Idosor{
    vector<Arfolyam> idosor;
};

struct Nap{
    mutable bool valid=false;
    mutable bool closedMarket = false;
    mutable bool unnep=false;
    Datum datum;
    mutable Datum elozoNap, kovetkezoNap;
    mutable float nyitas, zaras, minimum, maximum, volumen;
    mutable Idopont idoNyitas, idoZaras;
    mutable int hetMelyikNapja;
    mutable set<Arfolyam> percek;

    Nap(int y=0, int m=0, int d=0){
        datum=Datum(y,m,d);
        idoNyitas=Idopont(9,30);
        idoZaras=Idopont(16,0);
    }

    bool operator<(const Nap& other) const {
        return datum < other.datum;
    }
};

struct Negyed{
    Datum idoszakVege;
    mutable Datum tenylegesJelentes;
    mutable Datum negyedevVege;
    mutable float jelentettEPS=0, becsultEPS=0;
    mutable float meglepetes=0, meglepetesSzazalek=0;
    mutable long long earn=0, income=0;

    Negyed(int y=-1, int m=-1, int d=-1){idoszakVege=Datum(y,m,d);}

    bool operator<(const Negyed& other) const {
        return idoszakVege < other.idoszakVege;
    }
};

struct Stock{
    string name="teszt";

    set<Negyed> negyedevek;
    set<Nap> mindenNap;
    vector<Nap> mindenNapVec;

    void adatokKiirasaFajlba (string fajlNev);
    bool adatokBetoltese(string stock);
    bool adatokBetoltese2Teszt(string stock);
    bool adatokBetoltese3Teszt(string stock);
};


#endif // STOCK_H_INCLUDED
