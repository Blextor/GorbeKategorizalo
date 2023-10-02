#include "config.h"

using namespace std;

void writeConfig(string folder="C:\\stockData\\", int thrCnt=1, int apiC=30){
    ofstream of("config.cfg");
    of<<dataFolder<<" "<<folder<<endl;
    of<<threadCount<<" "<<thrCnt<<endl;
    of<<APICallsLimit<<" "<<apiC;
    of.close();
}

bool checkConfigFile(){
    ifstream file("config.cfg");
    if (file.is_open()){file.close(); return true;}
    file.close();
    writeConfig();
    return false;
}

void AppConfig::loadConfig(){
    checkConfigFile();
    string str;
    int threads;
    int APIC = 0;
    string folder;
    ifstream file("config.cfg");
    while (!file.eof()){
        file>>str;
        if (str==threadCount){
            file>>threads;
        }
        if (str==APICallsLimit){
            file>>APIC;
        }
        if (str==dataFolder){
            file>>folder;
        }
    }
    rootDirectory=folder;
    maxThreads=threads;
    maxAPICalls=APIC;
}

bool AppConfig::setMaxThreads(int threads){
    checkConfigFile();
    if (!(threads>0 && threads<=32)) {
        int fthr = 1;
        if (threads>32) fthr=32;
        maxThreads=fthr;
        writeConfig(rootDirectory,maxThreads,maxAPICalls);
        return false;
    }
    maxThreads=threads;
    writeConfig(rootDirectory,maxThreads,maxAPICalls);
    return true;
}

void AppConfig::setRootDirectory(const std::string& dir){
    checkConfigFile();
    rootDirectory=dir;
    writeConfig(rootDirectory,maxThreads,maxAPICalls);
}

void AppConfig::setMaxAPICalls(int calls){
    checkConfigFile();
    maxAPICalls=calls;
    writeConfig(rootDirectory,maxThreads,maxAPICalls);
}

