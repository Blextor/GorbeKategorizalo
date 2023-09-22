#ifndef ADATBEOLVAS_H_INCLUDED
#define ADATBEOLVAS_H_INCLUDED

#include "common.h"

using namespace std;

vector<string> osszesReszveny();
vector<string> osszesCsoport();
vector<string> csoportListaja(string csoport);

string createNewReszveny(string str);
string createNewCsoport(string str);

string deleteReszveny(string str);
string deleteCsoport(string str);

bool benneVanAzStr (string miben, string mi);
bool elemeAzStr ( vector<string> v, string mi);

vector<string> csoportReszvenyei(string str);

#endif // ADATBEOLVAS_H_INCLUDED
