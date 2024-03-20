#include "naptar.h"

int hetNapja(Datum d){
    std::tm time_in = { 0, 0, 0, // másodperc, perc, óra
                        d.day, d.month-1, d.year - 1900 }; // nap, hónap, év

    std::mktime(&time_in);
    return time_in.tm_wday;
}

int hetNapja(int y, int m, int d){
    return hetNapja(Datum(y,m,d));
}

Datum maiDatum(){
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time_t);

    int year = now_tm->tm_year + 1900; // Az év (1900-tól számolva)
    int month = now_tm->tm_mon + 1;   // A hónap (0-tól 11-ig van)
    int day = now_tm->tm_mday;        // A nap (1-től 31-ig)

    return Datum(year,month,day);
}
