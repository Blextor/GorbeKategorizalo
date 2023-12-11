#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include "common.h"

#define Config AppConfig::getInstance()

#define threadCount (string)"threadCount"
#define APICallsLimit (string)"APICallsLimit"
#define dataFolder (string)"dataFolder"

int getCimkeType(bool comp, bool daily, bool quarterly, bool onlyFloat);
std::vector<bool> getCimkeType(int type);
bool cimkeTypeCompatible(int t1, int t2);

class AppConfig {
private:
    // Privát konstruktor, hogy más osztályok ne tudják közvetlenül létrehozni
    AppConfig() {}

    // Adattagok tárolása
    std::string rootDirectory;
    int maxThreads;
    int maxAPICalls;

public:
    // Törli a másoló és értékadó mûveleteket
    AppConfig(const AppConfig&) = delete;
    AppConfig& operator=(const AppConfig&) = delete;

    // Statikus függvény a Singleton példány eléréséhez
    static AppConfig& getInstance() {
        static AppConfig instance; // Statikus változó garantálja az egyedi példány létrehozását
        return instance;
    }

    void loadConfig();

    // Getterek és setterek a konfigurációs adatokhoz
    void setRootDirectory(const std::string& dir);
    const std::string& getRootDirectory() const { return rootDirectory; }

    bool setMaxThreads(int threads);
    int getMaxThreads() const { return maxThreads; }

    void setMaxAPICalls(int calls) ;
    int getMaxAPICalls() const { return maxAPICalls; }
};

#endif // CONFIG_H_INCLUDED
