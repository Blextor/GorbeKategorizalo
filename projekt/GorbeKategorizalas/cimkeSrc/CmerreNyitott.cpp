#include "CmerreNyitott.h"

int CmerreNyitott::check(Stock* stock, Datum datum){
    Nap azNap(datum);
    set<Nap>::iterator it = stock->mindenNap.find(azNap);
    if (it==stock->mindenNap.end()) return 0;
    azNap = *it;
    Nap elozo(azNap.elozoNap);
    it = stock->mindenNap.find(elozo);
    if (it==stock->mindenNap.end()) return 0;
    elozo = *it;
    if (elozo.zaras<azNap.nyitas) return 1;
    return -1;
}

int CmerreNyitott::check(Stock* stock, Negyed datum){
    Negyed negyed(datum);
    set<Negyed>::iterator it = stock->negyedevek.find(negyed);
    if (it==stock->negyedevek.end()) return 0;
    negyed = *it;
    set<Negyed>::iterator it2 = it;
    if (it2==stock->negyedevek.begin()) return 0; //Negyed.negyedevVege
    --it2;
    set<Nap>::iterator nit = stock->mindenNap.find(Nap(Datum((*it).korrigaltTenylegesJelentes)));
    set<Nap>::iterator nit2 = stock->mindenNap.find(Nap(Datum((*it2).negyedevVege)));
    if (nit==stock->mindenNap.end() || nit2 == stock->mindenNap.end()) return 0;
    if ((*nit).nyitas>(*nit2).zaras) return 1;
    return -1;
}

float CmerreNyitott::getValue(Stock* stock, Datum datum) {
    Nap azNap(datum);
    set<Nap>::iterator it = stock->mindenNap.find(azNap);
    if (it==stock->mindenNap.end()) return 0;
    azNap = *it;
    Nap elozo(azNap.elozoNap);
    it = stock->mindenNap.find(elozo);
    if (it==stock->mindenNap.end()) return 0;
    elozo = *it;
    return azNap.nyitas/elozo.zaras;
}

float CmerreNyitott::getDiffValue(Stock* stock, Datum from, Datum to){
    Nap azNap(to);
    set<Nap>::iterator it = stock->mindenNap.find(azNap);
    if (it==stock->mindenNap.end()) return 0;
    azNap = *it;
    Nap elozo(from);
    it = stock->mindenNap.find(elozo);
    if (it==stock->mindenNap.end()) return 0;
    elozo = *it;
    return azNap.nyitas/elozo.zaras;
}


float CmerreNyitott::getValue(Stock* stock, Negyed datum) {

    return 0;
}

float CmerreNyitott::getDiffValue(Stock* stock, Negyed from, Negyed to){

    return 0;
}

