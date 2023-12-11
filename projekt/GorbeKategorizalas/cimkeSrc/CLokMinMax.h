#ifndef CLOKMINMAX_H_INCLUDED
#define CLOKMINMAX_H_INCLUDED

#include "../cimke.h"


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

#endif // CLOKMINMAX_H_INCLUDED
