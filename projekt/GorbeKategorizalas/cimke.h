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

	int napNegyedGet() {return (onlyDaily&1 + onlyQuarter&2 + onlyFloat&4);}
	void napNegyedSet(int i) {onlyQuarter=(i&2); onlyDaily=(i%1); onlyFloat=(i%4);}
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

    int check(Stock* stock, Datum datum) override;		/// n�zz�k meg, hogy igaz-e az adott d�tumra
	float getValue(Stock* stock, Datum datum) override;	/// mi az aznapra az �rt�ke
	float getDiffValue(Stock* stock, Datum from, Datum to) override; /// mi a k�t nap k�z�tti �rt�k k�l�n�bzet

	int check(Stock* stock, Negyed datum) override;
    float getValue(Stock* stock, Negyed datum) override;
    float getDiffValue(Stock* stock, Negyed from, Negyed to) override;
};

bool ujCimkeLetrehozasa(vector<string> param);
#endif // CIMKE_H_INCLUDED
