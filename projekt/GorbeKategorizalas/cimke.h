#ifndef CIMKE_H_INCLUDED
#define CIMKE_H_INCLUDED

#include "common.h"
#include "stock.h"

struct Cimke {
	string name = "NA";   /// kell egy elnevez�s
	string IDname = "cimke";
	bool comperator = false; /// hogy lehet-e k�t alkalom �sszehasonl�t�s�ra haszn�lni
	bool onlyDaily = false; /// hogy csak a napi 390 percre lehet haszn�lni (mert pl pontos)
	bool onlyQuarter = false; /// valami�rt csak a napi mozg�sokra lehet csak haszn�lni
	bool onlyFloat = false; /// kapott �rt�kek eg�szek vagy t�rtek

	int categoryID = 0; /// ha egy kateg�ri�ba esik egy c�mke, akkor azokat lehessen egy�tt kezelni

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

	virtual int check(Stock* stock, Datum datum);		/// n�zz�k meg, hogy igaz-e az adott d�tumra
	virtual float getValue(Stock* stock, Datum datum);	/// mi az aznapra az �rt�ke
	virtual float getDiffValue(Stock* stock, Datum from, Datum to); /// mi a k�t nap k�z�tti �rt�k k�l�n�bzet

	virtual int check(Stock* stock, Negyed datum);
    virtual float getValue(Stock* stock, Negyed datum);
    virtual float getDiffValue(Stock* stock, Negyed from, Negyed to);

};


#endif // CIMKE_H_INCLUDED
