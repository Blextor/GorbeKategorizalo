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
    // Priv�t konstruktor, hogy m�s oszt�lyok ne tudj�k k�zvetlen�l l�trehozni
    AppConfig() {}

    // Adattagok t�rol�sa
    std::string rootDirectory;
    int maxThreads;
    int maxAPICalls;

public:
    // T�rli a m�sol� �s �rt�kad� m�veleteket
    AppConfig(const AppConfig&) = delete;
    AppConfig& operator=(const AppConfig&) = delete;

    // Statikus f�ggv�ny a Singleton p�ld�ny el�r�s�hez
    static AppConfig& getInstance() {
        static AppConfig instance; // Statikus v�ltoz� garant�lja az egyedi p�ld�ny l�trehoz�s�t
        return instance;
    }

    void loadConfig();

    // Getterek �s setterek a konfigur�ci�s adatokhoz
    void setRootDirectory(const std::string& dir);
    const std::string& getRootDirectory() const { return rootDirectory; }

    bool setMaxThreads(int threads);
    int getMaxThreads() const { return maxThreads; }

    void setMaxAPICalls(int calls) ;
    int getMaxAPICalls() const { return maxAPICalls; }
};

#endif // CONFIG_H_INCLUDED
