#ifndef CIMKE_H_INCLUDED
#define CIMKE_H_INCLUDED

#include "common.h"
#include "stock.h"

struct Cimke {
	string name;   /// kell egy elnevez�s
	bool comperator = false; /// hogy lehet-e k�t alkalom �sszehasonl�t�s�ra haszn�lni
	bool only390 = false; /// hogy csak a napi 390 percre lehet haszn�lni (mert pl pontos)
	bool onlyDaily = false; // valami�rt csak a napi mozg�sokra lehet csak haszn�lni

	int categoryID = 0; /// ha egy kateg�ri�ba esik egy c�mke, akkor azokat lehessen egy�tt kezelni

	bool lowerLimitB = false; 	/// van valami als� hat�ra
	float lowerLimit = 0; 		/// �s mi az
	bool upperLimitB = false;	/// �s f�ls�
	float upperLimit = 0;		/// �s mi az

	Cimke(string n="", bool c=false, bool o3=false, bool oD=false){
        name=n; comperator=c; only390=o3; onlyDaily=oD;
	}

	virtual int check(Stock* stock, Datum datum);		/// n�zz�k meg, hogy igaz-e az adott d�tumra
	virtual float getValue(Stock* stock, Datum datum);	/// mi az aznapra az �rt�ke
	virtual float getDiffValue(Stock* stock, Datum from, Datum to); /// mi a k�t nap k�z�tti �rt�k k�l�n�bzet

	virtual int check(Stock* stock, Negyed datum);
    virtual float getValue(Stock* stock, Negyed datum);
    virtual float getDiffValue(Stock* stock, Negyed from, Negyed to);

};

#endif // CIMKE_H_INCLUDED
