#ifndef NAPTAR_H_INCLUDED
#define NAPTAR_H_INCLUDED

#include "common.h"

struct Idopont{
    int ora=-1,perc=-1;
    Idopont(int o=-1, int p=-1){ora=o; perc=p;}

    bool operator<(const Idopont& other) const {
        if (ora==other.ora)
            return perc < other.perc;
        return ora<other.ora;
    }

    int operator-(const Idopont& other) const {
        return (ora-other.ora)*60+perc-other.perc;
    }

    void kovetkezoPerc(){
        perc++;
        if (perc>59){
            perc=0;
            ora++;
        }
        if (ora>23) ora=0;
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


int hetNapja(Datum d);

int hetNapja(int y, int m, int d);

#endif // NAPTAR_H_INCLUDED
