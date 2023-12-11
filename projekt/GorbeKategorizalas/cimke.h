#ifndef CIMKE_H_INCLUDED
#define CIMKE_H_INCLUDED

#include "common.h"
#include "stock.h"

struct Cimke {
	string name = "NA";   /// kell egy elnevezés
	string IDname = "cimke";
	bool comperator = false; /// hogy lehet-e két alkalom összehasonlítására használni
	bool onlyDaily = false; /// hogy csak a napi 390 percre lehet használni (mert pl pontos)
	bool onlyQuarter = false; /// valamiért csak a napi mozgásokra lehet csak használni
	bool onlyFloat = false; /// kapott értékek egészek vagy törtek

	int categoryID = 0; /// ha egy kategóriába esik egy címke, akkor azokat lehessen együtt kezelni

	Cimke(string n="NA", bool c=false, bool o3=false, bool oD=false){
        name=n; comperator=c; onlyDaily=o3; onlyQuarter=oD;
	}

	int napNegyedGet() {return (onlyDaily&1 + onlyQuarter&2 + onlyFloat&4);}
	void napNegyedSet(int i) {onlyQuarter=(i&2); onlyDaily=(i%1); onlyFloat=(i%4);}
	vector<string> inputok;


    virtual int getI();

	virtual bool readIn(ifstream ifs);
	virtual bool readIn(vector<string> param);
	virtual bool writeOut(ofstream ofs);
	virtual bool writeOut();

	virtual int check(Stock* stock, Datum datum);		/// nézzük meg, hogy igaz-e az adott dátumra
	virtual float getValue(Stock* stock, Datum datum);	/// mi az aznapra az értéke
	virtual float getDiffValue(Stock* stock, Datum from, Datum to); /// mi a két nap közötti érték különöbzet

	virtual int check(Stock* stock, Negyed datum);
    virtual float getValue(Stock* stock, Negyed datum);
    virtual float getDiffValue(Stock* stock, Negyed from, Negyed to);

};

struct LokMinMax : Cimke {

    bool minimum = false;
    float tol = 0, ig = 0, hol = 0, kornyezet = 0;
    int alma = 13;

    LokMinMax();

    int getI() override; // {return alma;}

	bool readIn(ifstream ifs) override;
	bool writeOut(ofstream ofs) override;
	bool readIn(vector<string> param) override;
	bool writeOut() override;

    int check(Stock* stock, Datum datum) override;		/// nézzük meg, hogy igaz-e az adott dátumra
	float getValue(Stock* stock, Datum datum) override;	/// mi az aznapra az értéke
	float getDiffValue(Stock* stock, Datum from, Datum to) override; /// mi a két nap közötti érték különöbzet

	int check(Stock* stock, Negyed datum) override;
    float getValue(Stock* stock, Negyed datum) override;
    float getDiffValue(Stock* stock, Negyed from, Negyed to) override;
};

bool ujCimkeLetrehozasa(vector<string> param);
#endif // CIMKE_H_INCLUDED
