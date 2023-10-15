#ifndef CIMKE_H_INCLUDED
#define CIMKE_H_INCLUDED

#include "common.h"
#include "stock.h"

struct Cimke {
	string name;   /// kell egy elnevezés
	bool comperator = false; /// hogy lehet-e két alkalom összehasonlítására használni
	bool only390 = false; /// hogy csak a napi 390 percre lehet használni (mert pl pontos)
	bool onlyDaily = false; // valamiért csak a napi mozgásokra lehet csak használni

	int categoryID = 0; /// ha egy kategóriába esik egy címke, akkor azokat lehessen együtt kezelni

	bool lowerLimitB = false; 	/// van valami alsó határa
	float lowerLimit = 0; 		/// és mi az
	bool upperLimitB = false;	/// és fölsõ
	float upperLimit = 0;		/// és mi az

	Cimke(string n="", bool c=false, bool o3=false, bool oD=false){
        name=n; comperator=c; only390=o3; onlyDaily=oD;
	}

	virtual int check(Stock* stock, Datum datum);		/// nézzük meg, hogy igaz-e az adott dátumra
	virtual float getValue(Stock* stock, Datum datum);	/// mi az aznapra az értéke
	virtual float getDiffValue(Stock* stock, Datum from, Datum to); /// mi a két nap közötti érték különöbzet

	virtual int check(Stock* stock, Negyed datum);
    virtual float getValue(Stock* stock, Negyed datum);
    virtual float getDiffValue(Stock* stock, Negyed from, Negyed to);

};

#endif // CIMKE_H_INCLUDED
