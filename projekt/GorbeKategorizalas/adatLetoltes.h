#ifndef ADATLETOLTES_H_INCLUDED
#define ADATLETOLTES_H_INCLUDED

#include "common.h"

using namespace std;

struct Letolto {
    static clock_t utolsoLetoltes;
    const clock_t frissitesKoz = 2100; /// 1/30-ad perc + kicsi

    bool napiAdatLetoltes(string stock){

    }

    bool earningsLetoltes(string stock){

    }

    bool incomeLetoltes(string stock){

    }
};

string reszvenyLetoltesChk(string stock);
string reszvenyFrissitesChk(string stock);
string csoportLetoltesChk(string group);
string csoportFrissitesChk(string group);

#endif // ADATLETOLTES_H_INCLUDED
