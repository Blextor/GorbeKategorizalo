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

	void napNegyedSet(int i) {onlyQuarter=(i&4); onlyDaily=(i&2); onlyFloat=(i&8); comperator=(i&1);}
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


#endif // CIMKE_H_INCLUDED
