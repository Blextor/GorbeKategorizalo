#ifndef OSSZESCIMKE_H_INCLUDED
#define OSSZESCIMKE_H_INCLUDED

#include "../cimke.h"
#include "CmerreNyitott.h"
#include "CmerreZart.h"
#include "CLokMinMax.h"


class MindenCimke {
private:
    // Privát konstruktor, hogy más osztályok ne tudják közvetlenül létrehozni
    MindenCimke() {loadCimkek();}

    void loadCimkek(){
        mindenCimke.push_back(new LokMinMax());
        mindenCimke.push_back(new CmerreNyitott());
        mindenCimke.push_back(new CmerreZart());
    }

    // Adattagok tárolása
    vector<Cimke*> mindenCimke;

public:
    // Törli a másoló és értékadó mûveleteket
    MindenCimke(const MindenCimke&) = delete;
    MindenCimke& operator=(const MindenCimke&) = delete;

    // Statikus függvény a Singleton példány eléréséhez
    static MindenCimke& getInstance() {
        static MindenCimke instance; // Statikus változó garantálja az egyedi példány létrehozását
        return instance;
    }

    const vector<Cimke*> getMindenCimke() const { return mindenCimke; }

    bool getCimkeByNameAndType(string name, int type, Cimke *&cimke){
        AppConfig &app=AppConfig::getInstance();
        string cimkeFajl = app.getRootDirectory()+"cimkek.txt";
        ifstream ifs(cimkeFajl);
        string line, reszvenyIDname, rName;
        int t = 0; bool found = false;
        while (getline(ifs,line)){
            stringstream ss(line);
            ss>>reszvenyIDname>>rName>>t;
            if (name==rName && cimkeTypeCompatible(t,type)){
                found=true; break;
            }
        }
        if (!found) return false;

        for (int i=0; i<(int)mindenCimke.size(); i++){
            if (mindenCimke[i]->IDname==reszvenyIDname){
                cimke=mindenCimke[i]->clone();
                cimke->readIn(split(line,' '));
                return true;
            }
        }
        return false;
    }
    // Getterek és setterek a konfigurációs adatokhoz
    /**
    void setRootDirectory(const std::string& dir);
    const std::string& getRootDirectory() const { return rootDirectory; }

    bool setMaxThreads(int threads);
    int getMaxThreads() const { return maxThreads; }

    void setMaxAPICalls(int calls) ;
    int getMaxAPICalls() const { return maxAPICalls; }
    */
};

#endif // OSSZESCIMKE_H_INCLUDED
