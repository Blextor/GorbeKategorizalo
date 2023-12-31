#ifndef ADATBEOLVAS_H_INCLUDED
#define ADATBEOLVAS_H_INCLUDED

#include "common.h"
#include "config.h"

using namespace std;

bool isLocked(const std::mutex& mtx);
int egyReszvenyhezAPIMax();

int getActMin();
int getActMonth();
int getActYear();

vector<string> osszesReszveny();
vector<string> osszesCsoport();
vector<string> osszesCimke(int type=15);

string createNewReszveny(string str);
string createNewCsoport(string str);

string deleteReszveny(string str);
string deleteCsoport(string str);

vector<string> getSubdirectories(const string& directoryPath);
vector<string> getFiles(const string& directoryPath);

bool benneVanAzStr (string miben, string mi);
bool elemeAzStr (vector<string> v, string mi);

vector<string> csoportReszvenyei(string str);
string csoportFrissites(vector<string> vec, string csop);


#endif // ADATBEOLVAS_H_INCLUDED
