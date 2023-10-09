#ifndef ADATLETOLTES_H_INCLUDED
#define ADATLETOLTES_H_INCLUDED

#include "common.h"

using namespace std;

struct Letolto {
    static clock_t utolsoLetoltes;
    const clock_t frissitesKoz = 2100; /// 1/30-ad perc + kicsi


};

string reszvenyLetoltesChk(string stock);
string reszvenyFrissitesChk(string stock);
string csoportLetoltesChk(string group);
string csoportFrissitesChk(string group);


void reszvenyAPILetoltes(string stock, bool &stopped, bool &inProc, function<void(int x)> func);
void csoportAPILetoltes(string group, bool &stopped, bool &inProc, function<void(int x)> func);
void reszvenyAPIFrissites(string stock, bool &stopped, bool &inProc, function<void(int x)> func);
void csoportAPIFrissites(string group, bool &stopped, bool &inProc, function<void(int x)> func);

#endif // ADATLETOLTES_H_INCLUDED
