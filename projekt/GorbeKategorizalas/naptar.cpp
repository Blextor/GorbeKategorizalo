#include "naptar.h"

int hetNapja(Datum d){
    std::tm time_in = { 0, 0, 0, // másodperc, perc, óra
                        d.day, d.month, d.year - 1900 }; // nap, hónap, év

    std::mktime(&time_in);
    return time_in.tm_wday;
}

int hetNapja(int y, int m, int d){
    return hetNapja(Datum(y,m,d));
}
